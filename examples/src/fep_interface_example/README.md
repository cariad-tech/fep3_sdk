<!--
  Copyright 2023 CARIAD SE.
  
  This Source Code Form is subject to the terms of the Mozilla
  Public License, v. 2.0. If a copy of the MPL was not distributed
  with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
  
  -->
# FEP Interface Example

This example showcases a simple FEP Element interfacing with a thirdparty simulation tool.
It uses the "integrated architecture" (See Application Lifecycle Types in docs) and "strict simulation coupling" (see Simulation Coupling in docs). 
The thirdparty simulation tool is a dummy implementation in the custom_sim_tool.hpp, which is inspired by FMI and similar frameworks.

It showcases the following features:
 - Configuration handling with properties in all states (See also Configuration Service in docs)
 - Signal handling with the usage of the ddl library and codecs (See also Communication in Advanced topics in docs)
 - Using the state-machine in the recommended way (See also State Machine in FEP Element chapter in docs)

The demo can be used to simulate against a second instance of itself.
To do that, start the executable twice with the following arguments:

    fep_interface_example  -n Element1 -m inModel.model -i sig_in_1:tInput -o sig_out_1:tOutput -d fep_interface_example.description
    fep_interface_example  -n Element2 -m outModel.model -i sig_out_1:tOutput -o sig_in_1:tInput -d fep_interface_example.description
mm3_system_description_path = Path(__file__).parents[1].joinpath("data", "system", "system_multi_elems_integration_valid.mm3_system.yml")