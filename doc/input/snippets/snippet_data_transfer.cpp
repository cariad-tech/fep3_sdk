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
#include <vector>

#include <fep3/base/data_registry/data_registry.h>
#include <fep3/base/sample/data_sample.h>
#include <fep3/base/stream_type/default_stream_type.h>

#include <fep3/core/element_base.h>
#include <fep3/core/element_configurable.h>
#include <fep3/core/element_factory.h>
#include <fep3/core/job.h>
#include <fep3/core/participant.h>

using namespace std::chrono_literals;

namespace stream {
//Begin(own meta type implementation)
fep3::base::StreamMetaType meta_type_image{"image", std::list<std::string>{ "height", "width", "color_space", "number_of_pixels"}};
//End(own meta type implementation)

//Begin(own stream type implementation)
// Implementation of meta_type_image 
class StreamTypeImage : fep3::base::StreamType
{
    StreamTypeImage
        (const uint32_t height
        , const uint32_t width
        , const int64_t number_of_pixels
        , const std::string& color_space = "rgb"
        )
        : fep3::base::StreamType(meta_type_image)
    {
        // assign name, value and type of properties
        setProperty("height", std::to_string(height), "uint32_t");
        setProperty("width", std::to_string(width), "uint32_t");
        setProperty("number_of_pixels", std::to_string(number_of_pixels), "int64_t");
        setProperty("color_space", color_space, "string");
    }
};

// auto image_data_type = StreamTypeImage(1081, 1920, 2073600, "cmyk");
//End(own stream type implementation)

//Begin(basic example for Stream Type)
const fep3::base::StreamMetaType meta_type_plain{"plain-ctype", std::list<std::string>{ "datatype" }};

class StreamTypePlainInt32 : public fep3::base::StreamType
{
public:
    /**
     * @brief Construct a new Stream Type Plain object
     *
     */
    StreamTypePlainInt32() : fep3::base::StreamType(meta_type_plain)
    {
        // set name, value and type of a property
        setProperty("datatype", "int32_t", "string");
    }
};
//End(basic example for Stream Type)
}

namespace receiver
{
//Begin(data receiver class)
template<typename T>
class DataReceiver
    : public fep3::ISimulationBus::IDataReceiver
{
public:
    DataReceiver()
    {}

    virtual void operator()(const fep3::data_read_ptr<const fep3::IStreamType>& type)
    {
    }
    virtual void operator()(const fep3::data_read_ptr<const fep3::IDataSample>& sample)
    {
        // fep3::base::RawMemoryStandardType holds reference of DataReceiver::value member
        auto raw_memory = fep3::base::RawMemoryStandardType<T>(value);
        // The data in sample object is tranferred to DataReceiver::value
        sample->read(raw_memory);
    }
    T value;
};
//End(data receiver class)
}

class DataTransferJob : public fep3::IJob
{
public:
    using ExecuteCall = std::function<fep3::Result(fep3::Timestamp)>;

    explicit DataTransferJob(
        const ExecuteCall& ex_in,
        const ExecuteCall& ex,
        const ExecuteCall& ex_out) :
        _execute_func_data_in(ex_in),
        _execute_func(ex),
        _execute_func_data_out(ex_out)
    {
    }

    fep3::Result executeDataIn(fep3::Timestamp time_of_execution)
    {
        return _execute_func_data_in(time_of_execution);
    }
    fep3::Result execute(fep3::Timestamp time_of_execution)
    {
        return _execute_func(time_of_execution);
    }
    fep3::Result executeDataOut(fep3::Timestamp time_of_execution)
    {
        return _execute_func_data_out(time_of_execution);
    }
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_in;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func;
    std::function<fep3::Result(fep3::Timestamp)> _execute_func_data_out;
};

template<typename T>
class DataReceiver
    : public fep3::ISimulationBus::IDataReceiver
    , public  fep3::base::RawMemoryStandardType<T>
{
public:
    DataReceiver() : fep3::base::RawMemoryStandardType<T>(value)
    {}

    virtual void operator()(const fep3::data_read_ptr<const fep3::IStreamType>& type)
    {
    }
    virtual void operator()(const fep3::data_read_ptr<const fep3::IDataSample>& sample)
    {
        sample->read(*this);
    }
    T value;
};

namespace signal_exchange
{
namespace writer
{
class Element1 : public fep3::core::ElementBase
{
public:
    //Begin(first participant) 
    Element1() : fep3::core::ElementBase("writing_participant", "demo_version")
    {
        fep3::base::addDataOut(*getComponents()->getComponent<fep3::IDataRegistry>(), _uint32_writer, "uint32_signal", fep3::base::StreamTypePlain<uint32_t>());
        fep3::base::addDataOut(*getComponents()->getComponent<fep3::IDataRegistry>(), _uint64_writer, "uint64_signal", fep3::base::StreamTypePlain<uint64_t>());
    }
    //End(first participant) 
private:
    std::unique_ptr<fep3::IDataRegistry::IDataWriter> _uint32_writer;
    std::unique_ptr<fep3::IDataRegistry::IDataWriter> _uint64_writer;
};
} //end namespace writer
namespace first_reader
{
class Element2 : public fep3::core::ElementBase
{
public:
    //Begin(second participant) 
    Element2() : fep3::core::ElementBase("mix_participant", "demo_version")
    {
        fep3::base::addDataIn(*getComponents()->getComponent<fep3::IDataRegistry>(), _uint32_reader, "uint32_signal", fep3::base::StreamTypePlain<uint32_t>());
        fep3::base::addDataIn(*getComponents()->getComponent<fep3::IDataRegistry>(), _uint64_reader, "uint64_signal", fep3::base::StreamTypePlain<uint64_t>());
        fep3::base::addDataOut(*getComponents()->getComponent<fep3::IDataRegistry>(), _float_writer, "float_signal", fep3::base::StreamTypePlain<float>());
    }
    //End(second participant) 
private:
    std::unique_ptr<fep3::IDataRegistry::IDataReader> _uint32_reader;std::unique_ptr<fep3::IDataRegistry::IDataReader> _uint64_reader;
    std::unique_ptr<fep3::IDataRegistry::IDataWriter> _float_writer;
};

} //end namespace first_reader

namespace second_reader
{
class Element3 : public fep3::core::ElementBase
{
public:
    //Begin(third participant) 
    Element3() : fep3::core::ElementBase("reading_participant", "demo_version")
    {
        fep3::base::addDataIn(*getComponents()->getComponent<fep3::IDataRegistry>(), _uint64_reader, "uint64_signal", fep3::base::StreamTypePlain<uint64_t>());
        fep3::base::addDataIn(*getComponents()->getComponent<fep3::IDataRegistry>(), _float_reader, "float_signal", fep3::base::StreamTypePlain<float>());
    }
    //End(third participant)  
private:
    std::unique_ptr<fep3::IDataRegistry::IDataReader> _uint64_reader;
    std::unique_ptr<fep3::IDataRegistry::IDataReader> _float_reader;
};
} //end namespace second_reader
} //end namespace signal_exhange

namespace multiple_samples
{
namespace writer
{
//Begin(write array of samples)
class WriterElement : public fep3::core::ElementConfigurable
{
public:
    WriterElement() 
        : fep3::core::ElementConfigurable("writer_element", "1.0.1")
        , _counter(1)
    {
        _my_job = std::make_shared<DataTransferJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataOut(sim_time); });
    }

    fep3::Result load() override
    {
        _data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        return fep3::core::addToComponents("writer_job", _my_job, { 1s }, *getComponents());
    };

    fep3::Result initialize() override
    {
        return fep3::base::addDataOut(*_data_registry, _data_writer, "data_with_queue_size_10", fep3::base::StreamTypePlain<uint32_t>(), 10);
    }

    void deinitialize() override
    {
        fep3::base::removeDataOut(*_data_registry, "data_with_queue_size_10");
    }

    void unload() override
    {
        fep3::core::removeFromComponents("writer_job", *getComponents());
    }
    //Begin(write multiple sample)
    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        if (_data_writer)
        {
            // create Data Sample pool to store containers
            std::vector<std::shared_ptr<fep3::base::DataSampleType<uint32_t>>> sample_pool;
            // transfer 10 items in a simulation step
            for (int i=0; i < _queue_capacity ; i++)
            {
                sample_pool.push_back(std::make_shared<fep3::base::DataSampleType<uint32_t>>(_counter));
                _data_writer->write(*sample_pool.back());
                _counter++;
            }
        }
        return {};
    }
    //End(write multiple sample)
    fep3::Result processDataOut(fep3::Timestamp sim_time_of_execution)
    {
        if (_data_writer)
        {
            _data_writer->flush();
        }
        return {};
    }

private:
    const size_t _queue_capacity{10};
    uint32_t _counter;
    fep3::IDataRegistry* _data_registry;
    std::shared_ptr<DataTransferJob> _my_job;
    std::unique_ptr<fep3::arya::IDataRegistry::IDataWriter> _data_writer;
};
int main(int, const char**)
{
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<WriterElement>>(
        "writer",
        "1.0.0",
        "demo_system");
    return participant.exec();
}
//End(write array of samples)
}

namespace reader
{
//Begin(read array of samples)
class ReaderElement : public fep3::core::ElementConfigurable
{
public:
    ReaderElement() 
        : fep3::core::ElementConfigurable("reader_element", "1.0.1")
        , _counter(1)
    {
        _my_job = std::make_shared<DataTransferJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataIn(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return {}; });
    }

    fep3::Result load() override
    {
        _data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        return fep3::core::addToComponents("reader_job", _my_job, { 1s }, *getComponents());
    };
    fep3::Result initialize() override
    {
        return fep3::base::addDataIn(*_data_registry, _data_reader, "data_with_queue_size_10", fep3::base::StreamTypePlain<uint32_t>(), 10);
    }

    void deinitialize() override
    {
        fep3::base::removeDataOut(*_data_registry, "data_with_queue_size_10");
    }

    void unload() override
    {
        fep3::core::removeFromComponents("reader_job", *getComponents());
    }
    //Begin(read multiple sample)
    fep3::Result processDataIn(fep3::Timestamp sim_time_of_execution)
    {   
        std::vector<uint32_t> data_pool;
        // if data reader is registered
        if (_data_reader)
        {
            auto receiver = DataReceiver<uint32_t>();
            // Get front item from queue until queue is empty
            // data will be tranfered to callback of receiver
            while (fep3::isOk(_data_reader->pop(receiver)))
            {
                data_pool.push_back(receiver.value);
            }
        }
        return {};
    }
    //End(read multiple sample)
   
private:
    const size_t _queue_capacity{10};
    uint32_t _counter;
    fep3::IDataRegistry* _data_registry;
    std::shared_ptr<DataTransferJob> _my_job;
    std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> _data_reader;
};
int main(int, const char**)
{
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<ReaderElement>>(
        "reader",
        "1.0.0",
        "demo_system");
    return participant.exec();
}
//End(read array of samples)
}
}

namespace basic
{
class TransferElement : public fep3::core::ElementConfigurable
{
public:
    TransferElement() : fep3::core::ElementConfigurable("TransferElement", "1.0.0")
    {
        _my_job = std::make_shared<DataTransferJob>(
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataIn(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return process(sim_time); },
            [this](fep3::Timestamp sim_time)-> fep3::Result { return processDataOut(sim_time); });
    }

    fep3::Result load() override
    {
        _data_registry = getComponents()->getComponent<fep3::IDataRegistry>();
        fep3::core::addToComponents("tranfer_job", _my_job, { 1s }, *getComponents());
        return {};
    };
    //Begin(register data signal)
    fep3::Result initialize() override
    {
        fep3::Result res = _data_registry->registerDataIn("my_signal_in", fep3::base::StreamTypePlain<int64_t>());
        if (fep3::isOk(res)) {
            _data_reader = _data_registry->getReader("my_signal_in");
        }
        res = _data_registry->registerDataOut("my_signal_out", fep3::base::StreamTypePlain<int64_t>());
        if (fep3::isOk(res)) {
            _data_writer = _data_registry->getWriter("my_signal_out");
        }
        return res;
    }
    //End(register data signal)

    //Begin(unregister data signal)
    void deinitialize() override
    {
        _data_registry->unregisterDataIn("my_signal_in");
        _data_registry->unregisterDataOut("my_signal_out");
    }
    //End(unregister data signal)

    void unload() override
    {
        // unregister job
        fep3::core::removeFromComponents("tranfer_job", *getComponents());
    }

    fep3::Result processDataIn(fep3::Timestamp sim_time_of_execution)
    {
        //Begin(receive input data)
        if (_data_reader)
        {
            auto receiver = DataReceiver<int64_t>();
            if (fep3::isOk(_data_reader->pop(receiver)))
            {
                _data = receiver.value;
            }
        }
        //End(receive input data)
        return {};
    }

    fep3::Result process(fep3::Timestamp sim_time_of_execution)
    {
        //Begin(send output data)
        if (_data_writer)
        {
            int64_t data = _data * 2;
            auto data_holder = std::make_shared<fep3::base::DataSampleType<int64_t>>(data);
            _data_writer->write(*data_holder);
        }
        //End(send output data)
        return {};
    }

    fep3::Result processDataOut(fep3::Timestamp sim_time_of_execution)
    {
        if (_data_writer)
        {
            _data_writer->flush();
        }
        return {};
    }

private:
    int64_t _data;
    fep3::IDataRegistry* _data_registry;
    std::shared_ptr<DataTransferJob> _my_job;
    std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> _data_reader;
    std::unique_ptr<fep3::arya::IDataRegistry::IDataWriter> _data_writer;
};

int main(int, const char**)
{
    auto participant = fep3::core::createParticipant<fep3::core::ElementFactory<TransferElement>>(
        "demo",
        "1.0.0",
        "demo_system");
    
    //Begin(register data signal via fep3::base)
    // Retrieve the data registry component
    auto data_registry = participant.getComponent<fep3::IDataRegistry>();
    if (data_registry)
    {
        std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> data_reader;
        fep3::base::addDataIn(*data_registry, data_reader, "data_in", fep3::base::StreamTypeDDL("",""));

        std::unique_ptr<fep3::arya::IDataRegistry::IDataWriter> data_writer;
        fep3::base::addDataOut(*data_registry, data_writer, "data_out", fep3::base::StreamTypeDDL("",""));
    }
    //End(register data signal via fep3::base)

    if (data_registry)
    {
        //Begin(addDataIn with queue capacity)
        std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> data_reader;
        auto res = fep3::base::addDataIn(*data_registry, data_reader, "data_in_with_queue_capacity_10", fep3::base::StreamTypeDDL("",""), 10);
        //End(addDataIn with queue capacity)
        if (fep3::isFailed(res))
        {
            //...
        }


        
        //Begin(addDataOut with queue capacity)
        std::unique_ptr<fep3::arya::IDataRegistry::IDataWriter> data_writer;
        res = fep3::base::addDataOut(*data_registry, data_writer, "data_out_with_queue_capacity_10", fep3::base::StreamTypeDDL("",""), 10);
        //End(addDataOut with queue capacity)
        if (fep3::isFailed(res))
        {
            //...
        }
        
        
        std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> data_reader_to_remove;
        fep3::base::addDataIn(*data_registry, data_reader_to_remove, "data_in_to_remove", fep3::base::StreamTypeDDL("",""));

        std::unique_ptr<fep3::arya::IDataRegistry::IDataWriter> data_writer_to_remove;
        fep3::base::addDataOut(*data_registry, data_writer_to_remove, "data_out_to_remove", fep3::base::StreamTypeDDL("",""));
        
        
        //Begin(unregister data signal via fep3::base)
        fep3::base::removeDataIn(*data_registry, "data_in_to_remove");
        fep3::base::removeDataOut(*data_registry, "data_out_to_remove");
        //End(unregister data signal via fep3::base)
        
        
        //Begin(data triggered behaviour)
        std::unique_ptr<fep3::arya::IDataRegistry::IDataReader> data_triggered_behavior_signal;
        fep3::base::addDataIn(*data_registry, data_triggered_behavior_signal, "data_triggered_behavior_signal", fep3::base::StreamTypePlain<int64_t>());
        
        auto listener = std::make_shared<DataReceiver<int64_t>>();
        data_registry->registerDataReceiveListener("data_triggered_behavior_signal", listener);
        //End(data triggered behaviour)

        //Begin(end data triggered behaviour)
        data_registry->unregisterDataReceiveListener("data_triggered_behavior_signal", listener);
        //End(end data triggered behaviour)

    }
    return participant.exec();
}

} // namespace

// Defined without namespace because of compilation/linking
int main(int argc, char* argv[]) {return 0;}


