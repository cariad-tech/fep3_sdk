/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.

@endverbatim
 */



#include <chrono>
#include <memory>

#include <fep3/cpp.h>
#include <fep3/core.h>
#include <fep3/core/participant_executor.hpp>

using namespace std::chrono_literals;

/**************************************************
 * Writing an element                             *
 **************************************************/

namespace element {
//Begin(Declaration)
class MyElement : public fep3::core::ElementBase
//End(Declaration)
{
public:
    //Begin(Constructor)
    MyElement() : fep3::core::ElementBase("MyElementType", "1.0.0")
    //End(Constructor)
    {
    }

    //Begin(Load)
    fep3::Result load() override; // For the transition from Unloaded to Loaded
    //End(Load)

    //Begin(Initialize)
    fep3::Result initialize() override; // For the transition from Loaded to Initialized
    //End(Initialize)

    //Begin(Run)
    fep3::Result run() override; // For the transition from Initialized to Running
    //End(Run)

    //Begin(Stop)
    void stop() override; // For the transition from Running to Initialized
    //End(Stop)

    //Begin(Deinitialize)
    void deinitialize() override; // For the transition from Initialized to Loaded
    //End(Deinitialize)

    //Begin(Unload)
    void unload(); // For the transition from Loaded to Unloaded
    ~MyElement() override;
    //End(Unload)

};

fep3::Result MyElement::load()
{
    return {};
}
fep3::Result MyElement::initialize()
{
    return {};
}
fep3::Result MyElement::run()
{
    return {};
}
void MyElement::stop()
{
}
void MyElement::deinitialize()
{
}
void MyElement::unload()
{
}
MyElement::~MyElement() = default;

} // namespace element

/**************************************************
 * Reading and writing signals                    *
 **************************************************/

namespace signals
{
class MyElement : public fep3::core::ElementBase
{
public:
    MyElement() : fep3::core::ElementBase("MyElementType", "1.0.0")
    {
        //Begin(Job)
        std::shared_ptr<fep3::core::Job> my_job = std::make_shared<fep3::core::Job>(
            "JobName",
            10ms,
            [this](fep3::Timestamp sim_time) -> fep3::Result {
                return callbackFunction(sim_time);
            });

        // getComponents() is a function of fep3::core::ElementBase
        fep3::core::addToComponents({ my_job }, *getComponents());
        //End(Job)
    }

    fep3::Result callbackFunction (fep3::Timestamp) {return{};}
};

class MyDataJob : public fep3::cpp::DataJob
{
public:
    MyDataJob() : fep3::cpp::DataJob("my_data_job", 1s)
    {
        size_t queue_size{0};
        //Begin(DataWriter)
        fep3::core::DataWriter* _data_writer;

        _data_writer = addDataOut("SignalName", fep3::base::StreamTypePlain<int32_t>(), queue_size);
        //End(DataWriter)

        //Begin(DataReader)
        fep3::core::DataReader* _data_reader;

        _data_reader = addDataIn("SignalName", fep3::base::StreamTypePlain<int32_t>(), queue_size);
        //End(DataReader)
    }

    void process()
    {
        fep3::core::DataWriter* _data_writer;
        fep3::core::DataReader* _data_reader;
        int32_t data{};
        //Begin(ProcessWriter)
        // Template parameter of the streaming operator instead of void*
        // ensures type correctness at compile time.
        *_data_writer << data;
        //End(ProcessWriter)

        //Begin(ProcessReader)
        // Template parameter of the streaming operator instead of static_cast
        // removes the need for a signal handle.
        fep3::Optional<int32_t> optional_data;
        *_data_reader >> optional_data;
        if (optional_data.has_value())
        {
            // Use the data retrieved
        }
        //End(ProcessReader)

        {
        //Begin(ProcessVectorOut)
        std::vector<int32_t> my_data_array{};
        // Data sample helper class to wrap a std::vector
        fep3::base::StdVectorSampleType<int32_t> array_sample {my_data_array};

        _data_writer->write(array_sample);
        //End(ProcessVectorOut)
        }

        {
        //Begin(ProcessVectorIn)
        std::vector<int32_t> my_data_array{};
        // Data sample helper class to wrap a std::vector
        fep3::base::StdVectorSampleType<int32_t> array_sample {my_data_array};

        // Get pointer to the sample if available
        if(auto sample_ptr = _data_reader->popSampleOldest())
        {
            // Copy the data into the vector my_data_array
            sample_ptr->read(array_sample);
        }
        //End(ProcessVectorIn)
        }
    }
};
} // namespace signals



/**************************************************
 * Properties                                     *
 **************************************************/

namespace properties
{
class MyElement : public fep3::core::ElementBase, public fep3::base::Configuration
{
public:
    MyElement() : fep3::core::ElementBase("MyElementType", "1.0.0"), Configuration("MyElementProperties")
    {
        {
        //Begin(SetProperty)
        int32_t property_value{};

        fep3::IConfigurationService* configuration_service =
            fep3::getComponent<fep3::IConfigurationService>(*getComponents());

        fep3::base::setPropertyValue<int32_t>(*configuration_service, "PropertyPath", property_value);
        //End(SetProperty)
        }

        {
        //Begin(GetProperty)
        fep3::Optional<int32_t> property_value{};

        fep3::IConfigurationService* configuration_service =
            fep3::getComponent<fep3::IConfigurationService>(*getComponents());

        property_value = fep3::base::getPropertyValue<int32_t>(*configuration_service, "PropertyPath");
        //End(GetProperty)
        }

        {
        int32_t default_value{};
        int32_t property_value{};
        //Begin(CreateProperty)
        // The declaration of a default value is optional
        fep3::cpp::PropertyVariable<int32_t> my_property{default_value};

        // This will add the property to the root node of the configuration
        // The root node for ElementConfigurable is called "element"
        // The root node for DataJob is called "job_<data_job_name>"
        // So the full path will be "<root_node>/PropertyPath"
        registerPropertyVariable(my_property, "PropertyPath");

        // Getting the property value
        // We need to update the property variables because the PropertyVariable<T>
        // is just an observer of the property node in the configuration service.
        updatePropertyVariables();
        property_value = static_cast<int32_t>(my_property);
        //End(CreateProperty)
        }

        {
        //Begin(PropertyStructure)
        fep3::IConfigurationService* configuration_service =
            fep3::getComponent<fep3::IConfigurationService>(*getComponents());

        std::shared_ptr<fep3::IPropertyNode> property = configuration_service->getNode("PropertyPath");

        // FEP 3 does not have a getParent() function
        std::vector<std::shared_ptr<fep3::IPropertyNode>> children = property->getChildren();
        //End(PropertyStructure)
        }
    }
};
} // namespace properties



/**************************************************
 * Logging                                        *
 **************************************************/

namespace logs // term logging already taken by fep3
{
//Begin(LoggingSink)
class MyLoggingSink : public fep3::base::Properties<fep3::ILoggingService::ILoggingSink>
//End(LoggingSink)
{
    fep3::Result log(fep3::LogMessage log) const
    {
        return {};
    }
};

class MyElement : public fep3::core::ElementBase
{
public:
    MyElement() : fep3::core::ElementBase("MyElementType", "1.0.0")
    {
        //Begin(Logging)
        // Use any of the predefined macros for the actual logging
        FEP3_LOG_ERROR("LogMessage");
        //End(Logging)

        //Begin(RegisterSink)
        fep3::ILoggingService* logging_service =
            fep3::getComponent<fep3::ILoggingService>(*getComponents());

        std::shared_ptr<MyLoggingSink> mySink = std::make_shared<MyLoggingSink>();

        //The first parameter is the reference used to configure the logging filters to this sink
        logging_service->registerSink("my_sink", mySink);
        //End(RegisterSink)
    }
};
} // namespace logs

/**************************************************
 * Automation                                     *
 **************************************************/

namespace automation
{
class MyDataJob : public fep3::cpp::DataJob
{
public:
    MyDataJob() : fep3::cpp::DataJob("my_data_job", 1s) {}

    void process()
    {
        fep3::core::DataWriter* _data_writer;
        int32_t data{};

        //Begin(MutingVariable)
        fep3::cpp::PropertyVariable<bool> _is_muted{ false };
        registerPropertyVariable(_is_muted, "Muted");
        //End(MutingVariable)
        //Begin(MuteSignal)
        updatePropertyVariables();
        if(!static_cast<bool>(_is_muted))
        {
            *_data_writer << data;
        }
        //End(MuteSignal)
    }
};
} // namespace automation



/**************************************************
 * Instantiating a participant                    *
 **************************************************/

namespace instance
{
using namespace signals; // For definition of MyElement and MyDataJob

int main(int argc, char* argv[])
{
    {
    //Begin(CppInstantiation)
    fep3::Participant my_participant = fep3::cpp::createParticipant<fep3::cpp::DataJobElement<MyDataJob>>(
        argc, argv,
        "ParticipantName",
        "SystemName");

    my_participant.exec();
    //End(CppInstantiation)
    }

    {
    //Begin(CoreInstantiation)
    fep3::Participant my_participant = fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(
        argc, argv,
        "1.2.3",
        {"ParticipantName", "SystemName", ""});

    my_participant.exec();
    //End(CoreInstantiation)
    }

    return 0;
}
} // namespace instance



/**************************************************
 * Testing                                        *
 **************************************************/

namespace testing
{
using namespace element; // For definition of MyElement

int main(int argc, char* argv[])
{
    //Begin(ParticipantExecutor)
    fep3::Participant my_test_participant = fep3::core::createParticipant<fep3::core::ElementFactory<MyElement>>(
        "ParticipantName",
        "1.2.3",
        "SystemName");

    fep3::core::ParticipantExecutor executor(my_test_participant);

    executor.exec();
    executor.load();
    executor.initialize();
    // ... and so on
    //End(ParticipantExecutor)

    return 0;
}
} // namespace testing

// Defined without namespace because of compilation/linking
int main(int argc, char* argv[]) {return 0;}