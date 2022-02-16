#include "wrappers/StreamBuffer.hpp"

namespace util::wrappers
{

StreamBuffer::StreamBuffer(size_t BufferSize, size_t TriggerLevel)
{
    stream = xStreamBufferCreate(BufferSize, TriggerLevel);
    SafeAssert(stream != nullptr);
}

StreamBuffer::~StreamBuffer()
{
    vStreamBufferDelete(stream);
}
bool StreamBuffer::isEmpty()
{
    SafeAssert(stream != nullptr);
    return xStreamBufferIsEmpty(stream) == pdTRUE;
}
bool StreamBuffer::isFull()
{
    SafeAssert(stream != nullptr);
    return xStreamBufferIsFull(stream) == pdTRUE;
}
size_t StreamBuffer::receive(std::span<uint8_t> buffer, TickType_t ticksToWait)
{
    SafeAssert(stream != nullptr);
    return xStreamBufferReceive(stream, buffer.data(), buffer.size(), ticksToWait);
}
size_t StreamBuffer::receiveFromISR(std::span<uint8_t> buffer, BaseType_t *pxHigherPriorityTaskWoken)
{
    SafeAssert(stream != nullptr);
    return xStreamBufferReceiveFromISR(stream, buffer.data(), buffer.size(),
                                       pxHigherPriorityTaskWoken);
}
bool StreamBuffer::reset()
{
    SafeAssert(stream != nullptr);
    return xStreamBufferReset(stream) == pdPASS;
}
size_t StreamBuffer::send(std::span<uint8_t> buffer, TickType_t ticksToWait)
{
    SafeAssert(stream != nullptr);
    return xStreamBufferSend(stream, buffer.data(), buffer.size(), ticksToWait);
}
size_t StreamBuffer::sendFromISR(std::span<uint8_t> buffer, BaseType_t *pxHigherPriorityTaskWoken)
{
    SafeAssert(stream != nullptr);
    return xStreamBufferSendFromISR(stream, buffer.data(), buffer.size(),
                                    pxHigherPriorityTaskWoken);
}
bool StreamBuffer::setTriggerLevel(size_t triggerLevel)
{
    SafeAssert(stream != nullptr);
    return xStreamBufferSetTriggerLevel(stream, triggerLevel) == pdTRUE;
}
size_t StreamBuffer::getSpaceAvailable()
{
    SafeAssert(stream != nullptr);
    return xStreamBufferSpacesAvailable(stream);
}

StreamBuffer::StreamBuffer(StreamBuffer &&other) noexcept
{
    (*this) = std::move(other);
}

StreamBuffer& StreamBuffer::operator=(StreamBuffer &&other) noexcept
{
    stream = other.stream;
    other.stream = nullptr;
    return *this;
}

} // namespace util::wrappers