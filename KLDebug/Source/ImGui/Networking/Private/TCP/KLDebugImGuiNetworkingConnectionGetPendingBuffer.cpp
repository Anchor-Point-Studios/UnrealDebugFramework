// Distributed under the MIT License (MIT) (see accompanying LICENSE file)

#include "TCP/KLDebugImGuiNetworkingConnectionGetPendingBuffer.h"

FKLDebugImGuiNetworkingConnectionGetPendingBuffer::FKLDebugImGuiNetworkingConnectionGetPendingBuffer(TArray<FKLDebugImGuiNetworkingPendingMessage>& _PendingMessages, bool& _HasReadData)
    : mPendingMessages(_PendingMessages)
    , mHasReadData(_HasReadData)
{
}

FKLDebugImGuiNetworkingConnectionGetPendingBuffer::~FKLDebugImGuiNetworkingConnectionGetPendingBuffer()
{
    if (mPendingMessages.IsEmpty())
    {
        mHasReadData = false;
    }
}
