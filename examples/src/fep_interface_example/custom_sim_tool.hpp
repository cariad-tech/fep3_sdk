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
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

// Inspired by FMI and other tools
// Can load a model, which defines its inputs and outputs. All values are doubles internally.
// No array or struct support.
// Hardcoded for demo.
class CustomSimulationToolApi
{

public:
    CustomSimulationToolApi() = default;
    ~CustomSimulationToolApi() = default;

    struct SimSignal
    {
        std::string name;
        double value{0};
    };

    int loadSimModel(const std::string& model_path)
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // loading takes a little bit ;)
        if (model_path.find("inModel") != std::string::npos)
        {
            _input_signals.emplace(0, SimSignal{"x1", 0});
            _input_signals.emplace(1, SimSignal{"x2", 0});

            _output_signals.emplace(0, SimSignal{"y1", 0});
            _output_signals.emplace(1, SimSignal{"y2", 0});
        }
        else
        {
            _input_signals.emplace(0, SimSignal{"y1", 0});
            _input_signals.emplace(1, SimSignal{"y2", 0});

            _output_signals.emplace(0, SimSignal{"x1", 0});
            _output_signals.emplace(1, SimSignal{"x2", 0});
        }
        return 0;
    }

    int initSimModel()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 0;
    }

    int resetSimModel()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        _input_signals[0].value = 0;
        _input_signals[1].value = 0;
        _output_signals[0].value = 0;
        _output_signals[1].value = 0;
        return 0;
    }

    int deinitSimModel()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 0;
    }

    int unloadSimModel()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 0;
    }

    std::vector<std::string> getInputSimSignals() const
    {
        std::vector<std::string> signal_names;
        for (const auto &[_, sig] : _input_signals)
        {
            signal_names.emplace_back(sig.name);
        }
        return signal_names;
    }

    std::vector<std::string> getOutputSimSignals() const
    {
        std::vector<std::string> signal_names;
        for (const auto &[_, sig] : _output_signals)
        {
            signal_names.emplace_back(sig.name);
        }
        return signal_names;
    }

    uint32_t getInputSimSignalHandle(const std::string& name) const
    {
        for (const auto &[handle, sig] : _input_signals)
        {
            if (sig.name == name)
            {
                return handle;
            }
        }
        return -1;
    }

    uint32_t getOutputSimSignalHandle(const std::string& name)
    {
        for (const auto &[handle, sig] : _output_signals)
        {
            if (sig.name == name)
            {
                return handle;
            }
        }
        return -1;
    }

    void writeSimValue(uint8_t handle, double value)
    {
        // don't care about errors here
        _input_signals.at(handle).value = value;
    }

    double readSimValue(uint8_t handle)
    {
        // don't care about errors here
        return _output_signals.at(handle).value;
    }

    void executeSimStep()
    {
        // Hardcoded sim function
        _output_signals.at(0).value = _input_signals.at(1).value + 1;
        _output_signals.at(1).value += 1; // just count up
    }

private:
    // uint8_t simnulates a handle - usually for faster access or C-based APIs - only want to show here, that the SimTools are rarely name based
    std::map<uint8_t, SimSignal> _input_signals;
    std::map<uint8_t, SimSignal> _output_signals;
};