/*
 * Generated by erpcgen 1.9.0 on Wed Jun  1 03:44:04 2022.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "erpc_client_manager.h"
#if ERPC_ALLOCATION_POLICY == ERPC_ALLOCATION_POLICY_DYNAMIC
#include "erpc_port.h"
#endif
#include "erpc_codec.h"
extern "C"
{
#include "bbcar_control.h"
}

#if 10900 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif

using namespace erpc;
using namespace std;

extern ClientManager *g_client;


// BBCarService interface info function client shim.
void info(void)
{
    erpc_status_t err = kErpcStatus_Success;

    // Get a new request.
    RequestContext request = g_client->createRequest(false);

    // Encode the request.
    Codec * codec = request.getCodec();

    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }
    else
    {
        codec->startWriteMessage(kInvocationMessage, kBBCarService_service_id, kBBCarService_info_id, request.getSequence());

        // Send message to server
        // Codec status is checked inside this function.
        g_client->performRequest(request);

        err = codec->getStatus();
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    // Invoke error handler callback function
    g_client->callErrorHandler(err, kBBCarService_info_id);

    return;
}
