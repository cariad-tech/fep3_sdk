#include <fep3/cpp.h>
#include <fep3/core/element_configurable.h>
#include <chrono>

using namespace fep3;
using namespace std::chrono_literals;

class SenderElement : public fep3::core::ElementConfigurable
{
public:

SenderElement() : ElementConfigurable("AsyncWaitSetSender", "1.0.0")
{
    _job1 = std::make_shared<fep3::cpp::DataJob>("job1", 1s, [this](fep3::Timestamp time) {
                return this->work1(time);
            });
    _writer1 = _job1->addDataOut("topic1", base::StreamTypePlain<int>(), 10);

    _job2 = std::make_shared<fep3::cpp::DataJob>("job2", 1s, [this](fep3::Timestamp time) {
                return this->work2(time);
            });
    _writer2 = _job2->addDataOut("topic2", base::StreamTypePlain<int>(), 10);
};

fep3::Result initialize() override
{
    FEP3_RETURN_IF_FAILED(fep3::core::addToComponents({ _job1, _job2 }, *getComponents()));
    FEP3_RETURN_IF_FAILED(_job1->addDataToComponents(*getComponents()));
    FEP3_RETURN_IF_FAILED(_job2->addDataToComponents(*getComponents()));
    return {};
}

fep3::Result run() override
{
    FEP3_RETURN_IF_FAILED(_job1->reset());
    FEP3_RETURN_IF_FAILED(_job2->reset());
    return {};
}

void deinitialize() override 
{
    _writer1 = nullptr;
    _writer2 = nullptr;

    _job1->removeDataFromComponents(*getComponents());
    _job2->removeDataFromComponents(*getComponents());
    core::removeFromComponents({_job1->getJobInfo().getName(), _job2->getJobInfo().getName()}
                              ,*getComponents());
}

private:

fep3::Result work1(Timestamp time)
{
    base::DataSampleType<int> sample{_out1};

    FEP3_LOG_INFO("Hi, writer 1");
    FEP3_RETURN_IF_FAILED(_writer1->write(sample))
    return {};
}

fep3::Result work2(Timestamp time)
{
    base::DataSampleType<int> sample{_out2};

    FEP3_LOG_INFO("Hi, writer 2");
    FEP3_RETURN_IF_FAILED(_writer2->write(sample))
    return {};
}

private:
    std::shared_ptr<cpp::DataJob> _job1, _job2;
    cpp::DataWriter* _writer1 = nullptr;
    cpp::DataWriter* _writer2 = nullptr;
    int _out1 = 1;
    int _out2 = 2;
};

int main(int argc, const char* argv[])
{
    using namespace fep3::core;
    //creating a participant with the element that will send the logs
    auto demo_participant = createParticipant<fep3::core::ElementFactory<SenderElement>>(
        "async_waitset_sender",
        "1.0.0",
        "async_waitset");
    //wait until exit is send
    return demo_participant.exec();
}

