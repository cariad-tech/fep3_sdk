/**
 * @copyright
 * @verbatim
 * Copyright 2023 CARIAD SE.
 *
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */
#include <chrono>

#include <fep3/core.h>
#include <fep3/cpp.h>
#include <fep3/core/element_configurable.h>
#include <fep3/components/logging/easy_logger.h>

#include <ddl/ddl.h>
#include <ddl/codec/codec.h>
#include <a_util/system.h>

#include "fep_interface_example.h"

using namespace std::chrono;

/*Example for:
 - Integrated architecture
 - Strictly Coupled simulation coupling
 - Fully dynamically typed ddl communication
 Error handling is simplified for easier readability and only used, where it has showcase value.
 */

InterfaceExampleElement::InterfaceExampleElement(const std::string& model_path,
    const std::map<std::string, std::string>& signals_in,
    const std::map<std::string, std::string>& signals_out,
    const std::string& ddl_path)
    : fep3::core::ElementConfigurable("InterfaceExample Type", "1.0.0"),
      _sim_tool(std::make_unique<CustomSimulationToolApi>()),
      _model_path(model_path),
      _signals_in(signals_in),
      _signals_out(signals_out),
      _ddl_path(ddl_path),
      _my_delay(10)
{
}

fep3::Result InterfaceExampleElement::load()
{
    // Evaluate properties that were configured in unloaded
    FEP3_LOG_INFO(
        a_util::strings::format(
            "Updated sim_tool_interface property node: model_path_to_load='%s', signals_in='%s', signals_out='%s', ddl_path='%s'",
            _model_path.c_str(),
            StringMapConverter::toString(_signals_in).c_str(),
            StringMapConverter::toString(_signals_out).c_str(),
            _ddl_path.c_str()));
    _sim_tool->loadSimModel(_model_path);

    // Create custom property node under 'element' which will be evaluated in initialize
    auto config_service = getComponents()->getComponent<fep3::arya::IConfigurationService>();
    initConfiguration(*config_service);

    // Check path to DDL file
    if (_ddl_path.is_relative())
    {
        fs::path exe_path(a_util::system::getExecutablePath().getParent().toString());
        _ddl_path = exe_path /= _ddl_path;
    }
    if (!fs::exists(_ddl_path))
    {
        RETURN_ERROR_DESCRIPTION(fep3::ERR_FILE_NOT_FOUND, "Given path to DDL file '%s' does not point to a file.", _ddl_path.c_str());
        return fep3::ERR_FILE_NOT_FOUND;
    }

    // Create job and signals
    _sim_job = std::make_shared<fep3::cpp::DataJob>("sim_job", 500ms, 
        [this](fep3::Timestamp sim_time) -> fep3::Result { return process(sim_time); });

    std::string ddl_content;
    auto ddl_data = ddl::DDFile::fromXMLFile(_ddl_path.string());
    for (const auto &[name, ddl_type] : _signals_in)
    {
        auto res = addInputSignal(name, ddl_type, ddl_data);
        if (!res)
        {
            cleanupSignals(); // Don't forget to cleanup the already registered signals!
            return res;
        }
    }
    for (const auto &[name, ddl_type] : _signals_out)
    {
        auto res = addOutputSignal(name, ddl_type, ddl_data);
        if (!res)
        {
            cleanupSignals(); // Don't forget to cleanup the already registered signals!
            return res;
        }
    }

    // Register job and signals
    auto res = _sim_job->addDataToComponents(*getComponents());
    if (!res)
    {
        cleanupSignals();
        return res;
    }

    res = fep3::core::addToComponents({ _sim_job }, *getComponents());
    if(!res)
    {
        cleanupSignals();
        _sim_job->removeDataFromComponents(*getComponents());
        return res;
    }

    return {};
}

void InterfaceExampleElement::unload()
{
    // clean up everything that was create in load (properties, jobs, signals)
    _sim_job->removeDataFromComponents(*getComponents());
    fep3::core::removeFromComponents(_sim_job->getJobInfo().getName(), *getComponents());
    cleanupSignals();
    deinitConfiguration();
    _sim_tool.reset();
}

fep3::Result InterfaceExampleElement::initialize()
{
    // Log what has changed under element property
    try
    {   // default property conversion for Delay throws exception, if value is not formatted correctly
        updatePropertyVariables();
    }
    catch (std::exception &e)
    {
        FEP3_LOG_ERROR(a_util::strings::format("Property 'my_delay' under 'element' was not set correctly: %s", e.what()));
        return fep3::ERR_FAILED;
    }
    FEP3_LOG_INFO("Updated custom properties: my_delay=" + _my_delay.toString());

    _sim_tool->initSimModel();

    return {};
}

fep3::Result InterfaceExampleElement::registerPropertyVariables()
{
    FEP3_RETURN_IF_FAILED(registerPropertyVariable(_my_delay, "my_delay"));
    return {};
}

fep3::Result InterfaceExampleElement::unregisterPropertyVariables()
{
    FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(_my_delay));
    return {};
}

//Begin(ddl_setup)
fep3::Result InterfaceExampleElement::addInputSignal(const std::string& name, const std::string& ddl_type, const ddl::dd::DataDefinition& ddl_data_def)
{
    if (name.empty())
    {
        RETURN_ERROR_DESCRIPTION(fep3::ERR_FAILED, "A signal has no name.");
    }

    DDLSignalIn ddl_signal = DDLSignalIn();
    ddl_signal.last_log_time_ns = fep3::Timestamp(0);
    ddl_signal.name = name;
    ddl_signal.ddl_struct_type = ddl_type;

    createDecoder(&ddl_signal, ddl_data_def);

    // Map FEP DDL elements to model signals - TODO: Throw warning for not found struct elements
    // note: it is not in any case necessary to establish a mapping between DDL elements and model variables
    for (const auto &[struct_name, ddl_struct] : ddl_data_def.getStructTypes())
    {
        if (struct_name == ddl_type)
        {
            for (const auto &model_signal_name : _sim_tool->getInputSimSignals())
            {
                for (const auto &struct_element : ddl_struct->getElements())
                {
                    if (struct_element->getName() == model_signal_name)
                    {
                        // Normally we would now perform a type check, if the types are actually compatible, this is skipped here
                        auto index = ddl_signal.decoder->getElement(struct_element->getName()).getIndex();
                        ddl_signal.model_handle_2_ddl_index.emplace(_sim_tool->getInputSimSignalHandle(model_signal_name), index);
                        continue;
                    }
                }
            }
        }
    }

    // add signal to the job and set data reader
    auto data_reader =
        _sim_job->addDataIn(ddl_signal.name, fep3::base::StreamTypeDDL(ddl_signal.ddl_struct_type, ddl::DDString::toXMLString(ddl_data_def)));
    if (data_reader == nullptr)
    {
        RETURN_ERROR_DESCRIPTION(fep3::ERR_FAILED, "Could not create data reader for input signal '%s'.", ddl_signal.name.c_str());
    }
    ddl_signal.data_reader_ddl = data_reader;
    _data_sigs_in.emplace_back(ddl_signal);

    return {};
}

void InterfaceExampleElement::createDecoder(DDLSignalIn* sig, const ddl::dd::DataDefinition& ddl_data_def)
{
    auto codec_factory = ddl::codec::CodecFactory(ddl_data_def.getStructTypeAccess(sig->ddl_struct_type));
    // Get size of the serialized ddl struct and allocate a FEP Sample Buffer for it
    auto size = codec_factory.getStaticBufferSize();
    sig->sample = std::make_shared<fep3::base::DataSample>(size, true); // fixed_size = true
    // Now get the raw data pointer form the buffer and give it to the decoder, so it reads directly in the buffer
    sig->decoder = std::make_shared<ddl::codec::Decoder>(codec_factory.makeDecoderFor(sig->sample->cdata(), size, ddl::deserialized));
}
//End(ddl_setup)

fep3::Result InterfaceExampleElement::addOutputSignal(const std::string& name, const std::string& ddl_type, const ddl::dd::DataDefinition& ddl_data_def)
{
    if (name.empty())
    {
        FEP3_LOG_ERROR("A signal has no name.");
        return fep3::ERR_FAILED;
    }

    DDLSignalOut ddl_signal = DDLSignalOut();
    ddl_signal.name = name;
    ddl_signal.ddl_struct_type = ddl_type;

    createEncoder(&ddl_signal, ddl_data_def);
    
    // Map FEP DDL elements to model signals - TODO: Throw warning for not found struct elements
    // note: it is not in any case necessary to establish a mapping between DDL elements and model variables
    for (const auto& [struct_name, ddl_struct] : ddl_data_def.getStructTypes())
    {
        if (struct_name == ddl_type)
        {
            for (const auto &model_signal_name : _sim_tool->getOutputSimSignals())
            {
                for (const auto &struct_element : ddl_struct->getElements())
                {
                    if (struct_element->getName() == model_signal_name)
                    {
                        // Normally we would now perform a type check, if the types are actually compatible, this is skipped here
                        auto index = ddl_signal.encoder->getElement(struct_element->getName()).getIndex();
                        ddl_signal.model_handle_2_ddl_index.emplace(_sim_tool->getOutputSimSignalHandle(model_signal_name), index);
                        continue;
                    }
                }
            }
        }
    }

    // add signal to the job and set data writer
    auto data_writer =
        _sim_job->addDataOut(ddl_signal.name, fep3::base::StreamTypeDDL(ddl_signal.ddl_struct_type, ddl::DDString::toXMLString(ddl_data_def)));
    if (data_writer == nullptr)
    {
        RETURN_ERROR_DESCRIPTION(fep3::ERR_FAILED, "Could not create data reader for output signal '%s'.", ddl_signal.name.c_str());
    }
    ddl_signal.data_writer_ddl = data_writer;
    _data_sigs_out.emplace_back(ddl_signal);

    return {};
}

void InterfaceExampleElement::createEncoder(DDLSignalOut* sig, const ddl::dd::DataDefinition& ddl_data_def)
{
    auto codec_factory = ddl::codec::CodecFactory(ddl_data_def.getStructTypeAccess(sig->ddl_struct_type));
    // Get size of the serialized ddl struct and allocate a FEP Sample Buffer for it
    size_t size = codec_factory.getStaticBufferSize();
    sig->sample = std::make_shared<fep3::base::DataSample>(size, true); // fixed_size = true
    // Now get the raw data pointer from the buffer and give it to the encoder, so it writes directly into the buffer
    void *data_ptr = const_cast<void *>(sig->sample->cdata());
    sig->encoder = std::make_shared<ddl::codec::Codec>(codec_factory.makeCodecFor(data_ptr, size, ddl::deserialized));
}

void InterfaceExampleElement::cleanupSignals()
{
    // note: _sim_job (a fep3::cpp::DataJob) will handle a second call to addDataIn/addDataOut with the same 
    // signal name (will not be added twice), no need to cleanup here
    _data_sigs_in.clear();
    _data_sigs_out.clear();
}

void InterfaceExampleElement::deinitialize()
{
    _sim_tool->deinitSimModel();
}

fep3::Result InterfaceExampleElement::run()
{
    return {};
}

void InterfaceExampleElement::stop()
{
    _sim_tool->resetSimModel();
}

fep3::Result InterfaceExampleElement::readSample(DDLSignalIn& sig, fep3::Timestamp sim_time_ns)
{
    auto readSample = sig.data_reader_ddl->popSampleLatest();
    if (readSample == NULL)
    {
        // log only every second (simulation time) per signal
        auto delta = duration_cast<seconds>(sim_time_ns - sig.last_log_time_ns);
        if (delta > seconds(0))
        {
            FEP3_LOG_WARNING("No sample received for " + sig.name);
            sig.last_log_time_ns = sim_time_ns;
        }
        return fep3::ERR_EMPTY;
    }

    readSample->read(*sig.sample);
    return {};
}

fep3::Result InterfaceExampleElement::process(fep3::Timestamp sim_time_ns)
{
    // the _sim_job (fep3::cpp::DataJob) transfers data from the bus to our input signal data readers
    // and to the bus from our output signal data writers

    //Begin(ddl_signal_read)
    for (auto &sig : _data_sigs_in)
    {
        // receive input, copy to internal sample
        if (readSample(sig, sim_time_ns))
        {
            for (const auto &[model_index, ddl_index] : sig.model_handle_2_ddl_index)
            {
                auto val = sig.decoder->getElementVariantValue(ddl_index);
                _sim_tool->writeSimValue(model_index, val.asDouble());
                FEP3_LOG_INFO(a_util::strings::format("Read: %g", val.asDouble()));
            }
        }
    }
    //End(ddl_signal_read)

    // Normally, this would need synchronization with job cycle time
    _sim_tool->executeSimStep();

    //Begin(ddl_signal_write)
    for (auto &sig : _data_sigs_out)
    {
        {
            for (const auto &[model_index, ddl_index] : sig.model_handle_2_ddl_index)
            {
                double value = _sim_tool->readSimValue(model_index);
                sig.encoder->setElementValue(ddl_index, value);
                sig.data_writer_ddl->write(sim_time_ns, sig.sample->cdata(), sig.encoder->getBufferSize());
                FEP3_LOG_INFO(a_util::strings::format("Write %f", value));
            }
        }
    }
    //End(ddl_signal_write)
    return {};
}