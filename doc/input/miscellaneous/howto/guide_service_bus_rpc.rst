.. Copyright @ 2021 VW Group. All rights reserved.
.. 
..     This Source Code Form is subject to the terms of the Mozilla
..     Public License, v. 2.0. If a copy of the MPL was not distributed
..     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
.. 
.. If it is not possible or desirable to put the notice in a particular file, then
.. You may include the notice in a location (such as a LICENSE file in a
.. relevant directory) where a recipient would be likely to look for such a notice.
.. 
.. You may add additional accurate notices of copyright ownership.


.. |br| raw:: html

  <br/>

.. _label_guide_rpc_service_rpc_client:

=======================================================================
How to create a RPC Service and a RPC Client within the FEP Service Bus
=======================================================================

RPC Service generation
======================

:term:`RPC Service` s are public component interfaces via the :term:`FEP Service Bus`.
An RPC Service itself will provide certain functionality by responding to service request messages.
To do so the messages are received in a single callback function via service bus
(rpc::IRPCService::handleRequest). These messages are serialized as json-rpc strings. The serialization
can be generated with the help of the delivered Package RPC (*pkg_rpc*).

The following example will show how to create a FEP RPC Service and how to register it to the
FEP Service Bus component.

(1) get the pkg_rpc and the service bus rpc helper classes
----------------------------------------------------------

The RPC Service helper classes have dependencies to the *pkg_rpc*. Following CMake code must be
added to your CMakeLists.txt:

::

  # find the package
  find_package(pkg_rpc REQUIRED)

  # ... define your target and other stuff

  # link against the fep3_participant_core, pkg_rpc
  target_link_libraries(demo_rpc_service PRIVATE
                        fep3_participant_core
                        pkg_rpc)


Add includes to your code:

::

  #include <fep3/components/service_bus/service_bus_intf.h>
  #include <fep3/components/service_bus/rpc/fep_rpc.h>

(2) create a json file and generate the service stub
-----------------------------------------------------

Each RPC Service is described via a json file.
This file is used to generate a service stub via the tool *jsonrpcstub* which comes with the *pkg_rpc*.
The *pkg_rpc* configuration file will add the macro *jsonrpc_generate_server_stub* to the cmake layer to use
it as follows within the CMakeLists.txt:

::

  # parameter1: the json file to generate the service stub for
  # parameter2: the class name + namespaces for the service stub
  # parameter3: the header file where to generate the service stub to
  jsonrpc_generate_server_stub(${CMAKE_CURRENT_SOURCE_DIR}/demo_rpc_service.json
                               demo_rpc_stubs::DemoService
                               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_service_stub.h)

  # ...
  # add the generated file to your cmake target
  add_executable(demo_rpc_service
                 demo_rpc_service.cpp
                 ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_service_stub.h)
  target_include_directories(demo_rpc_service PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

The json file *demo_rpc_service.json* must contain following information:

::

  [
      // definition of a method that will return a bool value
      {
          "name": "getABoolValue",
          "returns": true
      },
      // definition of a method that will return a integer value
      {
          "name": "getAIntegerValue",
          "returns": 123
      },
      // definition of a method that will return a float value
      {
          "name": "getAFloatValue",
          "returns": 1.0
      },
      // definition of a method that will return a more complex return value
      // the return value is divided into a bool and a string (where a 64 bit is serialized into)
      // additionally a integer parameter is given to the service functionality
      {
          "name": "getAOptionalTimestamp",
          "params": {
              "integer_parameter": 123
          },
          "returns": {
              "valid": true,
              "64bitTimeStamp": "value_of_type"
          }
      }
  ]

In your code you can now include the generated file *demo_rpc_service_stub.h*:

::

  //within this header we now find a demo_rpc_stubs::DemoServiceStub implementation
  #include <demo_rpc_service_stub.h>


(3) Define the interface identifier of the RPC service and implement the abstract methods of the stub
-----------------------------------------------------------------------------------------------------

The FEP Service Bus interface needs an object to a :cpp:class:`fep3::rpc::IRPCService` interface implementation to
forward any requests to.
A base class is provided within the template :cpp:class:`fep3::rpc::RPCService`.

This template needs two additional types:

+--------------------------+--------------------------------------------------------------------------------------------------+
| the stub class           | This is the stub class generated. In our example we use *demo_rpc_stubs::DemoServiceStub*.       |
+--------------------------+--------------------------------------------------------------------------------------------------+
| rpc interface definiton  | This interface type is used to get 2 important information for the service:  |br|                |
|                          | (1) A unique interface identifier and |br| (2) a default service name within the Server          |
+--------------------------+--------------------------------------------------------------------------------------------------+

Definition of the rpc interface within your code:

::

  //definition of the interface identifier
  class IDemoRPCService
  {
      public:
         FEP_RPC_IID("demo_service.iid", "demo_service_defaultname");
  };

Definition of the service:
::

  //definition and implementation of the service
  class DemoService :
      public fep3::rpc::RPCService<demo_rpc_stubs::DemoServiceStub,
                                   IDemoRPCService>
  {
      //the method declarations can be copied from the "demo_rpc_service_stub.h"
      virtual bool getABoolValue() = 0;
      virtual int getAIntegerValue() = 0;
      virtual double getAFloatValue() = 0;
      virtual Json::Value getAOptionalTimestamp(int integer_parameter) = 0;
  };

Implement the abstract function:
::

  //the method declarations can be copied from the "demo_rpc_service_stub.h"
  bool getABoolValue() override
  {
      return false;
  }
  int getAIntegerValue() override
  {
      return 42;
  }
  double getAFloatValue() override
  {
      return 1.23;
  }
  Json::Value getAOptionalTimestamp(int integer_parameter) override
  {
      Json::Value result;
      result["valid"] = true;
      result["value_of_timestamp"] = std::to_string(integer_parameter);
      return result;
  }

(4) instantiate the implementation and register it
--------------------------------------------------
The complete main implementation of that example can be found within the examples folder

::

  int main(int argc, const char* argv[])
  {
    using namespace fep3::core;
    //creating a participant with an empty element
    // in this case we FORCE the Url of the participant server to use!!!
    auto demo_participant = createParticipant<ElementFactory<EmptyElement>>(
                                "demo_rpc_service_participant",
                                "1.0.0",
                                std::string(),
                                "http://localhost:10025");

    //retrieve the service bus component
    auto service_bus = demo_participant.getComponent<fep3::IServiceBus>();
    if (service_bus)
    {
        //retrieve the default server of this participant
        auto server = service_bus->getServer();
        if (server)
        {
            //create and register your service under the default name
            auto result = server->registerService(IDemoRPCService::getRPCDefaultName(),
                                                  std::make_shared<DemoRPCService>());
            if (fep3::isFailed(result))
            {
                std::cout << "Error: could not register ther demo service because: " << result.getDescription();
                return -1;
            }
        }
        else
        {
            std::cout << "Error: no default server created within service bus found!";
            return -1;
        }
    }
    else
    {
        std::cout << "Error: no service bus found!";
        return -1;
    }
    //wait until exit is send
    return demo_participant.exec();
  }

RPC Service Client generation
=============================

(1) get the pkg_rpc and the service bus rpc helper classes
----------------------------------------------------------

The RPC Service Client helper classes have dependencies to the *pkg_rpc*. Following CMake code must be
added to your CMakeLists.txt:

::

  # find the package
  find_package(pkg_rpc REQUIRED)

  # ... define your target and other stuff

  # link against the fep3_participant_core, pkg_rpc
  target_link_libraries(demo_rpc_client PRIVATE
                        fep3_participant_core
                        pkg_rpc)


Add includes to your code:

::

  #include <fep3/components/service_bus/service_bus_intf.h>
  #include <fep3/components/service_bus/rpc/fep_rpc.h>

(2) create a json file and generate the service client stub
-----------------------------------------------------------

Each RPC Service is described via a json file.
As we are able to generate a service stub via the tool *jsonrpcstub* we now generate a client stub.
The client stub is able to create the serialized json message to request a method call and de-serialize the response.
Add the  macro *jsonrpc_generate_client_stub* to your CMakeLists.txt:

::

  # parameter1: the json file to generate the client stub for
  # parameter2: the class name + namespaces for the client stub
  # parameter3: the header file where to generate the client stub to
  jsonrpc_generate_client_stub(${CMAKE_CURRENT_SOURCE_DIR}/demo_rpc_service.json
                               demo_rpc_stubs::DemoClientStub
                               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_client_stub.h)

  # ...
  # add the generated file to your cmake target
  add_executable(demo_rpc_client
               demo_rpc_client.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/demo_rpc_client_stub.h)
  target_include_directories(demo_rpc_client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

We reuse the json file from the example above *demo_rpc_service.json* because we want a client for that service.

In your code you can now include the generated file *demo_rpc_client_stub.h*:

::

  //within this header we now find a demo_rpc_stubs::DemoClientStub implementation
  #include <demo_rpc_client_stub.h>


(3) Define the interface identifier of the RPC service and implement the CTOR of the Client Class
------------------------------------------------------------------------------------------------------

To connect the client stub implementation to a :cpp:class:`fep3::IServiceBus` you can use the provided
template :cpp:class:`fep3::rpc::RPCServiceClient` .

This template needs two additional types:

+--------------------------+--------------------------------------------------------------------------------------------------+
| the stub class           | This is the stub class generated. In our example we use *demo_rpc_stubs::DemoClientStub*.        |
+--------------------------+--------------------------------------------------------------------------------------------------+
| rpc interface definition | This interface type is used to get 2 important information for the service:  |br|                |
|                          | (1) A unique interface identifier and |br| (2) a default service name within the Server          |
+--------------------------+--------------------------------------------------------------------------------------------------+

Definition of the rpc interface within your code.
You may also reuse the definition of the service implementation in shared public header.

::

  //definition of the interface identifier
  class IDemoRPCService
  {
      public:
         FEP_RPC_IID("demo_service.iid", "demo_service_defaultname");
  };

Implement the CTOR to forward a requester and the service name you want to request to:

::

  //definition and implementation of the client
  class DemoRPCService :
    public fep3::rpc::RPCServiceClient<demo_rpc_stubs::DemoClientStub,
                                       IDemoRPCService>
  {
    typedef fep3::rpc::RPCServiceClient<demo_rpc_stubs::DemoClientStub,
        IDemoRPCService> super;
  public:
    DemoRPCService(const std::string& service_name,
        const std::shared_ptr<fep3::rpc::IRPCRequester>& rpc_requester)
        : super(service_name,
                rpc_requester)
    {
    }
  };

(4) instantiate the implementation and get a requester from the FEP Service Bus
-------------------------------------------------------------------------------

::

  int main(int argc, const char* argv[])
  {
    using namespace fep3::core;
    //creating a participant with an empty element
    auto demo_participant = createParticipant<ElementFactory<EmptyElement>>(
        "demo_rpc_client_participant",
        "1.0.0",
        std::string());

    //retrieve the service bus component
    auto service_bus = demo_participant.getComponent<fep3::IServiceBus>();
    if (service_bus)
    {
        //retrieve a requester for the participant you want to
        //get the service from
        // YES! You need to know the name or the address of the other participant!
        // by using the name only ... discovery must be switched on!
        auto requester = service_bus->getRequester("http://localhost:10025", true);
        if (requester)
        {
            DemoRPCClient client(IDemoRPCService::getRPCDefaultName(),
                requester);

            //the bool value
            auto bool_response = client.getABoolValue();
            std::cout << "calling getABoolValue - result: " << bool_response << std::endl;
            //the integer value
            auto int_response = client.getAIntegerValue();
            std::cout << "calling getAIntegerValue - result: " << int_response << std::endl;
            //the double value
            auto double_response = client.getAFloatValue();
            std::cout << "calling getAFloatValue - result: " << double_response << std::endl;
            //a complex return value and a paramer
            auto complex_response = client.getAOptionalTimestamp(123);
            auto is_valid = complex_response["valid"].asBool();
            auto timestamp_response = complex_response["64bitTimeStamp"].asInt64();
            std::cout << "calling getAOptionalTimestamp - result: " << is_valid << " -> " << timestamp_response  << std::endl;
        }
        else
        {
            std::cout << "Error: no default requester can be retrieved from the service bus!";
            return -1;
        }
    }
    else
    {
        std::cout << "Error: no service bus found!";
        return -1;
    }
    //in this example we use the participant to get an IServiceBus
    //usually you do that with the system library
  }
