//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef PROTOCOLBRIDGERESOURCE_H_
#define PROTOCOLBRIDGERESOURCE_H_

#include "BundleResource.h"
#include <map>
#include <string>

namespace OIC
{
    namespace Service
    {

        /**
        * @class    ProtocolBridgeResource
        * @brief    This class represents bundle resource template for Protocol Bridge
        *               to be registered in the container and make resource server
        *
        */
        class ProtocolBridgeResource: public BundleResource
        {
            public:

                /**
                * Constructor for ProtocolBridgeResource
                */
                ProtocolBridgeResource();

                /**
                * Virtual destructor for ProtocolBridgeResource
                */
                virtual ~ProtocolBridgeResource();

                virtual ResourceAttributes& getAttributes()=0;

                virtual void setAttribute(std::string key, ResourceAttributes::Value&&)=0;

                virtual ResourceAttributes::Value getAttribute(const std::string& key)=0;
        };
    }
}

#endif
