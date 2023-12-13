/**
 * @copyright
 * @verbatim
 * Copyright @ 2023 VW Group. All rights reserved.
 *
This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */
#pragma once

#include <fep3/core.h>
#include <fep3/cpp.h>
#include <fep3/core/element_configurable.h>
#include <fep3/components/participant_info/participant_info_intf.h>
#include <fep3/components/clock/clock_base.h>

#include <ddl/ddl.h>
#include <a_util/filesystem.h>

#include "string_map_property_type.hpp"
#include "delay_property_type.hpp"
#include "custom_sim_tool.hpp"

constexpr auto OWN_PROP_NODE = "sim_tool_interface";

constexpr auto SIG_IN_PROP_PATH = "signals_in";
constexpr auto SIG_OUT_PROP_PATH = "signals_out";
constexpr auto MODEL_PATH_PROP_PATH = "model_path_to_load";
constexpr auto DDL_PATH_PROP_PATH = "ddl_path";

constexpr auto CUSTOM_NODE = "custom_properties";

constexpr auto DELAY_PATH = "my_delay";

namespace fs = a_util::filesystem;

using StringMapConverter = fep3::base::DefaultPropertyTypeConversion<std::map<std::string, std::string>>;


/**
 * Example of a PropertyVariable container that is registered
 * in the configuration service outside of the participant
 */
struct SimToolInterfaceProperties : public fep3::base::Configuration
{
public:
    using StringVariable = fep3::base::PropertyVariable<std::string>;
    using StringMapVariable = fep3::base::PropertyVariable<std::map<std::string, std::string>>;


    SimToolInterfaceProperties(const std::string& model_path,
                               const std::string& signals_in,
                               const std::string& signals_out,
                               const std::string& ddl_path) : Configuration(OWN_PROP_NODE)
    {
        _model_path = std::make_shared<StringVariable>(model_path);
        _signals_in = std::make_shared<StringMapVariable>(StringMapConverter::fromString(signals_in));
        _signals_out = std::make_shared<StringMapVariable>(StringMapConverter::fromString(signals_out));
        _ddl_path = std::make_shared<StringVariable>(ddl_path);
        
    }
    ~SimToolInterfaceProperties() = default;

public:
    fep3::Result registerPropertyVariables()
    {
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(*_model_path, MODEL_PATH_PROP_PATH));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(*_signals_in, SIG_IN_PROP_PATH));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(*_signals_out, SIG_OUT_PROP_PATH));
        FEP3_RETURN_IF_FAILED(registerPropertyVariable(*_ddl_path, DDL_PATH_PROP_PATH));

        return {};
    }

    fep3::Result unregisterPropertyVariables()
    {
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(*_model_path, MODEL_PATH_PROP_PATH));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(*_signals_in, SIG_IN_PROP_PATH));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(*_signals_out, SIG_OUT_PROP_PATH));
        FEP3_RETURN_IF_FAILED(unregisterPropertyVariable(*_ddl_path, DDL_PATH_PROP_PATH));
        return {};
    }

public:
    std::shared_ptr<StringVariable> _model_path;
    std::shared_ptr<StringMapVariable> _signals_in;
    std::shared_ptr<StringMapVariable> _signals_out;
    std::shared_ptr<StringVariable> _ddl_path;
};

struct DDLSignalIn
{
    // This can be used to slow down logging on a per signal basis
    fep3::Timestamp last_log_time_ns;
    std::string name = "";
    std::string ddl_struct_type = "";

    fep3::core::DataReader* data_reader_ddl;

    std::shared_ptr<ddl::codec::Decoder> decoder;
    std::shared_ptr<fep3::base::DataSample> sample;

    // handle based access for quick runtime lookup between ddl and model elements
    std::map<uint32_t, ddl::codec::CodecIndex> model_handle_2_ddl_index;
};

struct DDLSignalOut
{
    std::string name = "";
    std::string ddl_struct_type = "";

    fep3::core::DataWriter* data_writer_ddl;

    std::shared_ptr<ddl::codec::Codec> encoder;
    std::shared_ptr<fep3::base::DataSample> sample;

    // handle based access for quick runtime lookup between ddl and model elements
    std::map<uint32_t, ddl::codec::CodecIndex> model_handle_2_ddl_index;
    ;
};


// Example for:
// - Integrated architecture
// - Strictly Coupled simulation coupling
// - Fully dynamically typed ddl communication
class InterfaceExampleElement : public fep3::core::ElementConfigurable
{
public:
    InterfaceExampleElement(const std::string& model_path,
        const std::map<std::string, std::string>& signals_in,
        const std::map<std::string, std::string>& signals_out,
        const std::string& ddl_path);
    virtual ~InterfaceExampleElement() = default;

    fep3::Result load() override;

    void unload() override;

    fep3::Result initialize() override;

    void deinitialize() override;

    fep3::Result run() override;

    void stop() override;

    fep3::Result registerPropertyVariables() override;
    fep3::Result unregisterPropertyVariables() override;

    fep3::Result readSample(DDLSignalIn& sig, fep3::Timestamp sim_time);

    fep3::Result process(fep3::Timestamp sim_time);

private:
    std::shared_ptr<fep3::cpp::DataJob> _sim_job; // synchronous r/w with one job

    std::unique_ptr<CustomSimulationToolApi> _sim_tool;
    std::string _model_path;
    std::map<std::string, std::string> _signals_in;
    std::map<std::string, std::string> _signals_out;
    fs::Path _ddl_path;


    // Properties
    fep3::base::PropertyVariable<Delay> _my_delay;

    // Signals
    std::vector<DDLSignalIn> _data_sigs_in;
    std::vector<DDLSignalOut> _data_sigs_out;

    void cleanupSignals();

    fep3::Result addInputSignal(const std::string& name, const std::string& ddl_type, const ddl::dd::DataDefinition& ddl_data_def);
    fep3::Result addOutputSignal(const std::string& name, const std::string& ddl_type, const ddl::dd::DataDefinition& ddl_data_def);
    void createDecoder(DDLSignalIn* sig, const ddl::dd::DataDefinition& ddl_data_def);
    void createEncoder(DDLSignalOut* sig, const ddl::dd::DataDefinition& ddl_data_def);
};

class ExampleInterfaceElementFactory : public fep3::base::IElementFactory
{
public:
    ExampleInterfaceElementFactory(SimToolInterfaceProperties& sim_tool_if_props)
        : _sim_tool_if_props(sim_tool_if_props)
    {
    }

    std::unique_ptr<fep3::base::IElement> createElement(const fep3::arya::IComponents& components) const
    {
        _sim_tool_if_props.updatePropertyVariables();
        return std::make_unique<InterfaceExampleElement>(
            *_sim_tool_if_props._model_path, *_sim_tool_if_props._signals_in,
            *_sim_tool_if_props._signals_out, *_sim_tool_if_props._ddl_path);
    }

private:
    SimToolInterfaceProperties& _sim_tool_if_props;
};
