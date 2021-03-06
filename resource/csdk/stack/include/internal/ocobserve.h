//********************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

/**
 * @file
 *
 * This file contains the data structure and APIs for registered resource as an observer.
 *
 */


#ifndef OC_OBSERVE_H
#define OC_OBSERVE_H

#include "ocserverrequest.h"

/** Maximum number of observers to reach */

#define MAX_OBSERVER_FAILED_COMM         (2)

/** Maximum number of observers to reach for resources with low QOS */
#define MAX_OBSERVER_NON_COUNT           (3)

/**
 *  MAX_OBSERVER_TTL_SECONDS sets the maximum time to live (TTL) for notification.
 *  60 sec/min * 60 min/hr * 24 hr/day
 */
#define MAX_OBSERVER_TTL_SECONDS     (60 * 60 * 24)

#define MILLISECONDS_PER_SECOND   (1000)

/**
 * Data structure to hold informations for each registered observer.
 */
typedef struct ResourceObserver
{
    /** Observation Identifier for request.*/
    OCObservationId observeId;

    /** URI of observed resource.*/
    char *resUri;

    /** Query.*/
    char *query;

    /** token for the observe request.*/
    CAToken_t token;

    /** token length for the observe request.*/
    uint8_t tokenLength;

    /** Resource handle.*/
    OCResource *resource;

    /** Remote Endpoint. */
    OCDevAddr devAddr;

    /** Quality of service of the request.*/
    OCQualityOfService qos;

    /** number of times the server failed to reach the observer.*/
    uint8_t failedCommCount;

    /** number of times the server sent NON notifications.*/
    uint8_t lowQosCount;

    /** force the qos value to CON.*/
    uint8_t forceHighQos;

    /** The TTL for this callback. TTL is set to 24 hours.
     * A server send a notification in a confirmable message every 24 hours.
     * This prevents a client that went away or is no logger interested
     * from remaining in the list of observers indefinitely.*/
    uint32_t TTL;

    /** next node in this list.*/
    struct ResourceObserver *next;

    /** requested payload encoding format. */
    OCPayloadFormat acceptFormat;

    /** requested payload content version. */
    uint16_t acceptVersion;

} ResourceObserver;

#ifdef WITH_PRESENCE
/**
 * Create an observe response and send to all observers in the observe list.
 *
 * @param method          RESTful method.
 * @param resPtr          Observed resource.
 * @param maxAge          Time To Live (in seconds) of observation.
 * @param resourceType    Resource type.  Allows resource type name to be added to response.
 * @param qos             Quality of service of resource.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult SendAllObserverNotification (OCMethod method, OCResource *resPtr, uint32_t maxAge,
        OCPresenceTrigger trigger, OCResourceType *resourceType, OCQualityOfService qos);
#else
/**
 * Create an observe response and send to all observers in the observe list.
 *
 * @param method RESTful method.
 * @param resPtr Observed resource.
 * @param maxAge Time To Live (in seconds) of observation.
 * @param qos Quality of service of resource.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult SendAllObserverNotification (OCMethod method, OCResource *resPtr, uint32_t maxAge,
        OCQualityOfService qos);
#endif

/**
 * Notify specific observers with updated value of representation.
 *
 * @param resource                  Observed resource.
 * @param obsIdList                 List of observation ids that need to be notified.
 * @param numberOfIds               Number of observation ids included in obsIdList.
 * @param payload                   JSON encoded payload to send in notification.
 * @param maxAge                    Time To Live (in seconds) of observation.
 * @param qos                       Desired quality of service of the observation notifications.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult SendListObserverNotification (OCResource * resource,
        OCObservationId  *obsIdList, uint8_t numberOfIds,
        const OCRepPayload *payload, uint32_t maxAge,
        OCQualityOfService qos);

/**
 * Delete all observers in the observe list.
 */
void DeleteObserverList();

/**
 * Create a unique observation ID.
 *
 * @param observationId           Pointer to generated ID.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult GenerateObserverId (OCObservationId *observationId);

/**
 * Add observer for a resource.
 *
 * @param resUri          Resource URI string.
 * @param query           Query string.
 * @param obsId           Observation ID.
 * @param token           Request token.
 * @param tokenLength     Length of token.
 * @param resHandle       Resource handle.
 * @param qos             Quality of service of observation.
 * @param acceptFormat    Accept payload format.
 * @param acceptVersion   Accept payload version.
 * @param devAddr         Device address.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult AddObserver (const char         *resUri,
                           const char         *query,
                           OCObservationId    obsId,
                           CAToken_t          token,
                           uint8_t            tokenLength,
                           OCResource         *resHandle,
                           OCQualityOfService qos,
                           OCPayloadFormat    acceptFormat,
                           uint16_t           acceptVersion,
                           const OCDevAddr    *devAddr);

/**
 * Delete observer with specified token from list of observers.
 * Free memory that was allocated for the observer in the list.
 *
 * @param token Token to search for.
 * @param tokenLength Length of token.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
 OCStackResult DeleteObserverUsingToken (CAToken_t token, uint8_t tokenLength);

 /**
  * Delete observer with device address from list of observers.
  * Free memory that was allocated for the observer in the list.
  *
  * @param devAddr Device's address.
  *
  * @return ::OC_STACK_OK on success, some other value upon failure.
  */
OCStackResult DeleteObserverUsingDevAddr(const OCDevAddr *devAddr);

/**
 * Search the list of observers for the specified token.
 *
 * @param token            Token to search for.
 * @param tokenLength      Length of token.
 *
 * @return Pointer to found observer.
 */
ResourceObserver* GetObserverUsingToken (const CAToken_t token, uint8_t tokenLength);

/**
 * Search the list of observers for the specified observe ID.
 *
 * @param observeId        Observer ID to search for.
 *
 * @return Pointer to found observer.
 */
ResourceObserver* GetObserverUsingId (const OCObservationId observeId);

/**
 *  Add observe header option to a request.
 *
 * @param caHdrOpt        Target request CA header option.
 * @param ocHdrOpt        Pointer to existing options.
 * @param numOptions      Number of existing options.
 * @param observeFlag     Register/de-register observation.  Should be either
 *                        ::OC_OBSERVE_REGISTER or ::OC_OBSERVE_DEREGISTER.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult
CreateObserveHeaderOption (CAHeaderOption_t **caHdrOpt,
                           OCHeaderOption *ocHdrOpt,
                           uint8_t numOptions,
                           uint8_t observeFlag);

/**
 *  Copy the observe option from a received request.
 *
 * @param observationOption      Pointer to observe option value.  Should be either
 *                               ::OC_OBSERVE_REGISTER, ::OC_OBSERVE_DEREGISTER, or
 *                               ::OC_OBSERVE_NO_OPTION if observe not found.
 *
 * @param options                Options in received request.  Observe option is removed if found.
 * @param numOptions             Number of options in the received request.  Decremented if observe
 *                               option is extracted.
 *
 * @return ::OC_STACK_OK on success, some other value upon failure.
 */
OCStackResult
GetObserveHeaderOption (uint32_t * observationOption,
                        CAHeaderOption_t *options,
                        uint8_t * numOptions);

/**
 * Handle registering/deregistering of observers of virtual resources.  Currently only the
 * well-known virtual resource (/oic/res) may be observable.
 *
 * @param request a virtual resource server request
 *
 * @return ::OC_STACK_OK on success, ::OC_STACK_DUPLICATE_REQUEST when registering a duplicate
 *         observer, some other value upon failure.
 */
OCStackResult
HandleVirtualObserveRequest(OCServerRequest *request);


#endif //OC_OBSERVE_H

