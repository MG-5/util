#include "wrappers/StreamBuffer.hpp"
#include <utility>

namespace util::wrappers
{

StreamBuffer::StreamBuffer(const size_t BufferSize, const size_t TriggerLevel)
    : streamHandle(xStreamBufferCreate(BufferSize, TriggerLevel))
{
    assert(streamHandle != nullptr);
}

StreamBuffer::~StreamBuffer()
{
    if (streamHandle != nullptr)
    {
        vStreamBufferDelete(streamHandle);
    }
}

bool StreamBuffer::isEmpty()
{
    return xStreamBufferIsEmpty(streamHandle) == pdTRUE;
}

bool StreamBuffer::isFull()
{
    return xStreamBufferIsFull(streamHandle) == pdTRUE;
}

size_t StreamBuffer::receive(uint8_t *buffer, size_t bufferSize, const TickType_t ticksToWait)
{
    return xStreamBufferReceive(streamHandle, buffer, bufferSize, ticksToWait);
}

size_t StreamBuffer::receiveFromISR(uint8_t *buffer, size_t bufferSize,
                                    BaseType_t *pxHigherPriorityTaskWoken)
{
    return xStreamBufferReceiveFromISR(streamHandle, buffer, bufferSize, pxHigherPriorityTaskWoken);
}

bool StreamBuffer::reset()
{
    const auto res = xStreamBufferReset(streamHandle) == pdPASS;
    return res;
}

size_t StreamBuffer::send(uint8_t *buffer, size_t bufferSize, const TickType_t ticksToWait)
{
    return xStreamBufferSend(streamHandle, buffer, bufferSize, ticksToWait);
}

size_t StreamBuffer::sendFromISR(uint8_t *buffer, size_t bufferSize,
                                 BaseType_t *pxHigherPriorityTaskWoken)
{
    return xStreamBufferSendFromISR(streamHandle, buffer, bufferSize, pxHigherPriorityTaskWoken);
}

bool StreamBuffer::setTriggerLevel(const size_t triggerLevel)
{
    return xStreamBufferSetTriggerLevel(streamHandle, triggerLevel) == pdTRUE;
}

size_t StreamBuffer::getSpaceAvailable()
{
    return xStreamBufferSpacesAvailable(streamHandle);
}

StreamBuffer::StreamBuffer(StreamBuffer &&other) noexcept
{
    (*this) = std::forward<StreamBuffer>(other);
}

StreamBuffer &StreamBuffer::operator=(StreamBuffer &&other) noexcept
{
    streamHandle = std::exchange(other.streamHandle, nullptr);
    return *this;
}

size_t StreamBuffer::getBytesAvailable()
{
    return xStreamBufferBytesAvailable(streamHandle);
}

} // namespace util::wrappers