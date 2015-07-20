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

#include "HueSampleBundleActivator.h"
#include "HueLight.h"

#include <algorithm>
#include <vector>

using namespace OIC::Service;

HueSampleBundleActivator *bundle;

HueSampleBundleActivator::HueSampleBundleActivator()
{
}

HueSampleBundleActivator::~HueSampleBundleActivator()
{
}

void HueSampleBundleActivator::activateBundle(ResourceContainerBundleAPI *resourceContainer,
        std::string bundleId)
{

    m_pResourceContainer = resourceContainer;
    m_bundleId = bundleId;
    m_connector = new HueConnector();

    vector< resourceInfo > resourceConfig;

    resourceContainer->getResourceConfiguration(m_bundleId, &resourceConfig);

    for (vector< resourceInfo >::iterator itor = resourceConfig.begin();
         itor != resourceConfig.end(); itor++)
    {
        createResource(*itor);
    }
}

void HueSampleBundleActivator::deactivateBundle()
{
    std::cout << "HueSampleBundle::deactivateBundle called" << std::endl;

    for (unsigned int i = 0; i < m_vecResources.size(); i++)
    {
        destroyResource(m_vecResources.at(i));
    }

    delete m_connector;
}

void HueSampleBundleActivator::createResource(resourceInfo resourceInfo)
{

    if (resourceInfo.resourceType == "oic.light.control")
    {
        static int lightCount = 1;
        HueLight *hueLight = new HueLight(m_connector, resourceInfo.address);
        resourceInfo.uri = "/hue/light/" + std::to_string(lightCount++);
        std::cout << "Registering resource " << resourceInfo.uri << std::endl;
        hueLight->m_bundleId = m_bundleId;
        hueLight->m_uri = resourceInfo.uri;
        hueLight->m_resourceType = resourceInfo.resourceType;
        hueLight->m_name = resourceInfo.name;

        m_pResourceContainer->registerResource(hueLight);
        m_vecResources.push_back(hueLight);
    }
}

void HueSampleBundleActivator::destroyResource(BundleResource *pBundleResource)
{
    std::cout << "HueSampleBundle::destroyResource called" << pBundleResource->m_uri << std::endl;

    std::vector< BundleResource * >::iterator itor;

    itor = std::find(m_vecResources.begin(), m_vecResources.end(), pBundleResource);

    if (itor != m_vecResources.end())
    {
        m_pResourceContainer->unregisterResource(pBundleResource);
        m_vecResources.erase(itor);
    }

    //TODO
    /*std::cout << "Clearing up memory.\n";

    if (itor != m_vecResources.end())
        m_vecResources.erase(itor);*/

    // check
    //delete resource;

}

extern "C" void externalActivateBundle(ResourceContainerBundleAPI *resourceContainer,
                                       std::string bundleId)
{
    bundle = new HueSampleBundleActivator();
    bundle->activateBundle(resourceContainer, bundleId);
}

extern "C" void externalDeactivateBundle()
{
    bundle->deactivateBundle();
    delete bundle;
}

extern "C" void externalCreateResource(resourceInfo resourceInfo)
{
    bundle->createResource(resourceInfo);
}

extern "C" void externalDestroyResource(BundleResource *pBundleResource)
{
    bundle->destroyResource(pBundleResource);
}
