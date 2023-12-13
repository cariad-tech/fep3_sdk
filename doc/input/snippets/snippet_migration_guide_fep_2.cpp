/**
 * @file
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

This Source Code Form is subject to the terms of the Mozilla
Public License, v. 2.0. If a copy of the MPL was not distributed
with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
@endverbatim
 */



#include <fep_participant_sdk.h>

using namespace fep;

/**************************************************
 * Writing an element                             *
 **************************************************/

namespace element
{
//Begin(Declaration)
class cMyModule : public fep::cModule
//End(Declaration)
, public fep::cStateExitListener
{
public:
    cMyModule() = default;

    //Begin(Create)
    fep::Result Create(const cModuleOptions& oModuleOptions)
    //End(Create)
    {
        return{};
    }

    void stateTransitions()
    {
        //Begin(Load)
        ProcessStartupEntry(fep::tState::FS_STARTUP);
        ProcessStartupExit(fep::tState::FS_IDLE);
        ProcessIdleEntry(fep::tState::FS_STARTUP);
        //End(Load)

        //Begin(Initialize)
        ProcessIdleExit(fep::tState::FS_INITIALIZING);
        ProcessInitializingEntry(fep::tState::FS_IDLE);
        ProcessInitializingExit(fep::tState::FS_READY);
        ProcessReadyEntry(fep::tState::FS_INITIALIZING);
        //End(Initialize)

        //Begin(Run)
        ProcessReadyExit(fep::tState::FS_RUNNING);
        ProcessRunningEntry(fep::tState::FS_READY);
        //End(Run)

        //Begin(Stop)
        ProcessRunningExit(fep::tState::FS_IDLE);
        ProcessIdleEntry(fep::tState::FS_RUNNING);
        //End(Stop)

        //Begin(Deinitialize)
        ProcessReadyExit(fep::tState::FS_IDLE);
        ProcessInitializingExit(fep::tState::FS_IDLE);
        ProcessIdleEntry(fep::tState::FS_READY);
        ProcessIdleEntry(fep::tState::FS_INITIALIZING);
        //End(Deinitialize)

        //Begin(Unload)
        ProcessStartupEntry(fep::tState::FS_IDLE); // Restart event
        //End(Unload)
    }

    //Begin(Unload2)
    fep::Result ProcessShutdownEntry(const fep::tState eOldState);
    fep::Result CleanUp(const fep::tState eOldState);
    //End(Unload2)


    //Begin(Error)
    fep::Result ProcessErrorEntry(const fep::tState eOldState);
    //End(Error)
};

fep::Result cMyModule::ProcessShutdownEntry(const fep::tState) { return{}; }
fep::Result cMyModule::CleanUp(const fep::tState) { return{}; }
fep::Result cMyModule::ProcessErrorEntry(const fep::tState) { return{}; }

} // namespace element



/**************************************************
 * Reading and writing signals                    *
 **************************************************/

namespace signals
{
class cMyModule : public fep::cModule,
                  public fep::IUserDataListener,
                  public fep::ISyncListener
{
public:
    cMyModule()
    {
        //Begin(StepListener)
        GetTimingInterface()->RegisterStepListener(
           "StepListenerName",
           fep::StepConfig(10 * 1000 /*us*/),
           &pCallbackFunction,
           this);
        //End(StepListener)

        {
        //Begin(OutputSignal)
        fep::IUserDataSample* pSample;
        handle_t hHandle;

        cUserSignalOptions oOptions(
            "SignalName",
            fep::SD_Output,
            "SignalType");

        cModule::GetSignalRegistry()->RegisterSignal(oOptions, hHandle);
        cModule::GetUserDataAccess()->CreateUserDataSample(pSample, hHandle);
        //End(OutputSignal)
        }

        {
        //Begin(InputSignal)
        handle_t hHandle;

        cUserSignalOptions oOptions(
            "SignalName",
            fep::SD_Input,
            "SignalType");

        cModule::GetSignalRegistry()->RegisterSignal(oOptions, hHandle);
        cModule::GetUserDataAccess()->RegisterDataListener(this, hHandle);
        //End(InputSignal)
        }

        {
        size_t max_depth{0};
        //Begin(SyncListener)
        handle_t hHandle;
        fep::IDDBAccess* ddb_access;

        cModule::InitDDBEntry(
            "SignalName",
            "SignalType",
            max_depth,
            fep::tDDBDeliveryStrategy::DDBDS_DumpIncomplete,
            hHandle,
            &ddb_access);

        ddb_access->RegisterSyncListener(this);
        //End(SyncListener)
        }
    }

    static void pCallbackFunction(void* pCallee, timestamp_t tmSimulation, IStepDataAccess* pStepDataAccess)
    {
        reinterpret_cast<cMyModule*>(pCallee)->pCallbackFunctionImpl(tmSimulation, pStepDataAccess);
    }

    void pCallbackFunctionImpl(timestamp_t, IStepDataAccess*)
    {
        fep::IUserDataSample* pSample;
        const void* data{nullptr};
        size_t data_size{0};
        //Begin(Callback)
        pSample->CopyFrom(data, data_size);
        cModule::GetUserDataAccess()->TransmitData(pSample, true);
        //End(Callback)

        bool bSync{};
        //Begin(DDBCallback)
        cModule::GetUserDataAccess()->TransmitData(pSample, bSync);
        //End(DDBCallback)
    }

    fep::Result Update(fep::IUserDataSample* pSample)
    {
        handle_t hHandle;
        //Begin(Update)
        if(pSample->GetSignalHandle() == hHandle)
        {
            int32_t* data = static_cast<int32_t*>(pSample->GetPtr());
        }
        //End(Update)
    }

    fep::Result ProcessDDBSync(const handle_t hSignal, const fep::IDDBFrame& oDDBFrame)
    {
        handle_t hHandle;
        size_t sampleIndex{0};
        //Begin(ProcessDDB)
        if(hSignal == hHandle)
        {
            const fep::IUserDataSample* pSample = oDDBFrame.GetSample(sampleIndex);
            int32_t* data = static_cast<int32_t*>(pSample->GetPtr());
        }
        //End(ProcessDDB)
    }
};
} // namespace signals



/**************************************************
 * Properties                                     *
 **************************************************/

namespace properties
{
//Begin(DeclarationListener)
class cMyModule : public fep::cModule, public fep::IPropertyListener
//End(DeclarationListener)
{
public:
    cMyModule()
    {
        //Begin(RegisterListener)
        GetPropertyTree()->GetProperty("PropertyPath")->RegisterListener(this);
        //End(RegisterListener)

        int32_t property_value{};
        //Begin(SetProperty)
        GetPropertyTree()->SetPropertyValue(
            "PropertyPath",
            property_value);
        //End(SetProperty)

        //Begin(GetProperty)
        GetPropertyTree()->GetPropertyValue(
            "PropertyPath",
            property_value);
        //End(GetProperty)

        //Begin(PropertyStructure)
        fep::IProperty* property = GetPropertyTree()->GetLocalProperty("PropertyPath");

        fep::IProperty* parent = property->GetParent();
        std::list<fep::IProperty*> children = property->GetSubProperties();
        //End(PropertyStructure)

        {
        const timestamp_t tmTimeout{0};
        IProperty** pProperty;
        //Begin(RemoteProperty)
        GetPropertyTree()->GetRemoteProperty(
            "ElementName",
            "PropertyPath",
            pProperty,
            tmTimeout);
        //End(RemoteProperty)
        }

        {
        const timestamp_t tmTimeout{0};
        //Begin(MirrorProperty)
        GetPropertyTree()->MirrorRemoteProperty(
            "ElementName",
            "RemotePropertyPath",
            "LocalPropertyPath",
            tmTimeout);
        //End(MirrorProperty)
        }
    }

    //Begin(PropertyChange)
    fep::Result ProcessPropertyChange(
        const IProperty* pProperty,
        const IProperty* pAffectedProperty,
        const char* strRelativePath)
    //End(PropertyChange)
    {}
};
} // namespace properties

/**************************************************
 * Logging                                        *
 **************************************************/

namespace incidents
{
//Begin(CustomStrategy)
class cMyIncidentStrategy : public fep::IIncidentStrategy
//End(CustomStrategy)
{};

class cMyModule : public fep::cModule
{
public:
    cMyModule()
    {
        int16_t nIncidentCode{0};
        //Begin(InvokeIncident)
        GetIncidentHandler()->InvokeIncident(
            nIncidentCode,
            fep::eFEPSeverityLevel::SL_Critical,
            "Description",
            "Origin",
            __LINE__,
            __FILE__);
        //End(InvokeIncident)

        //Begin(ConfigureLogging)
        GetIncidentHandler()->AssociateStrategy(
            nIncidentCode,
            fep::eIncidentStrategy::ES_LogConsole,
            fep::eStrategyAssociation::SA_APPEND);
        //End(ConfigureLogging)

        //Begin(AssociateStrategy)
        cMyIncidentStrategy* myStrategy;

        GetIncidentHandler()->AssociateStrategy(
            nIncidentCode,
            myStrategy,
            "ConfigurationPath",
            fep::eStrategyAssociation::SA_REPLACE);
        //End(AssociateStrategy)
    }
};
} // namespace incidents



/**************************************************
 * Automation                                     *
 **************************************************/

namespace automation
{
class cMyModule : public fep::cModule
{
public:
    cMyModule()
    {
        //Begin(Muting)
        fep::AutomationInterface oAI;
        oAI.MuteParticipant("ParticipantName");
        oAI.MuteSignal("SignalName","ParticipantName");
        //End(Muting)
    }
};
} // namespace automation



/**************************************************
 * Instantiating a participant                    *
 **************************************************/

int main(int argc, char* argv[])
{
    using element::cMyModule;

    //Begin(Instantiation)
    fep::cModuleOptions oOptions{"ParticipantName", fep::timing_FEP_30};
    cMyModule my_participant;
    my_participant.Create(oOptions);
    my_participant.WaitForShutdown();
    //End(Instantiation)
}