/**
 * Copyright 2023 CARIAD SE.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <fep_system/fep_system.h>
#include <iostream>
#include <cassert>
#include <string>

using namespace std::chrono_literals;

void check_execution_error(const fep3::JobHealthiness::ExecuteResult& job_exec_result, const std::string& job_call, const std::string& job_name)
{
    if (job_exec_result.error_code != 0)
    {
        std::cout << "Problem in execution of job " << job_name << " in " << job_call << " error, : " << job_exec_result.error_description << "\n";
    }
}

int main(int argc, const char* argv[])
{
    auto my_system = fep3::discoverSystem("my_system");

    //start_monitor_participant_getParticipantState
    auto participant_state = my_system.getParticipantState("test_participant");
    if (participant_state == fep3::SystemAggregatedState::unreachable)
    {
        std::cout << "Problem, test_participant is offline\n";
    }
    //end_monitor_participant_getParticipantState

    //start_monitor_participant_checkParticipantHealth
    auto participants_health = my_system.getParticipantsHealth();
    assert(participants_health.count("test_participant") == 1);
    if (participants_health.at("test_participant").running_state == fep3::ParticipantRunningState::offline)
    {
        std::cout << "Problem, test_participant is offline\n";
    }

    for (const auto& job_health : participants_health.at("test_participant").jobs_healthiness)
    {
        check_execution_error(job_health.execute_data_in_error.last_error, "executeDataIn", job_health.job_name);
        check_execution_error(job_health.execute_error.last_error, "execute", job_health.job_name);
        check_execution_error(job_health.execute_data_out_error.last_error, "executeDataOut", job_health.job_name);
    }
    //end_monitor_participant_checkParticipantHealth

    //start_monitor_participant_check_hanging_job
    std::map<std::string, std::chrono::nanoseconds> last_job_execution;
    for (const auto& job_health : participants_health.at("test_participant").jobs_healthiness)
    {
        if (last_job_execution[job_health.job_name] == job_health.simulation_time)
        {
            std::cout << "Problem in execution of job " << job_health.job_name << " , job seems to be hanging\n";
        }
        last_job_execution[job_health.job_name] = job_health.simulation_time;
    }
    //end_monitor_participant_check_hanging_job

    return 0;
}

