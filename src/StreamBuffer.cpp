#include "wrappers/StreamBuffer.hpp"
#include <utility>

namespace util::wrappers
{

StreamBuffer::StreamBuffer(size_t BufferSize, size_t TriggerLevel)
    : stream(xStreamBufferCreate(BufferSize, TriggerLevel))
{
    SafeAssert(stream != nullptr);
}

StreamBuffer::~StreamBuffer()
{
    if (stream != nullptr)
    {
        vStreamBufferDelete(stream);
    }
}
bool StreamBuffer::isEmpty()
{
    return xStreamBufferIsEmpty(stream) == pdTRUE;
}
bool StreamBuffer::isFull()
{
    return xStreamBufferIsFull(stream) == pdTRUE;
}
size_t StreamBuffer::receive(std::span<uint8_t> buffer, const TickType_t ticksToWait)
{
    return xStreamBufferReceive(stream, buffer.data(), buffer.size(), ticksToWait);
}
size_t StreamBuffer::receiveFromISR(std::span<uint8_t> buffer,
                                    BaseType_t *pxHigherPriorityTaskWoken)
{
    return xStreamBufferReceiveFromISR(stream, buffer.data(), buffer.size(),
                                       pxHigherPriorityTaskWoken);
}
bool StreamBuffer::reset()
{
    // this is not interrupt safe for some versions of freertos
    // https://forums.freertos.org/t/xstreambufferreset-is-not-interrupt-safe/7202/2
    portENTER_CRITICAL();
    const auto res = xStreamBufferReset(stream) == pdPASS;
    portEXIT_CRITICAL();
    return res;
}
size_t StreamBuffer::send(std::span<uint8_t> buffer, const TickType_t ticksToWait)
{
    return xStreamBufferSend(stream, buffer.data(), buffer.size(), ticksToWait);
}
size_t StreamBuffer::sendFromISR(std::span<uint8_t> buffer, BaseType_t *pxHigherPriorityTaskWoken)
{
    return xStreamBufferSendFromISR(stream, buffer.data(), buffer.size(),
                                    pxHigherPriorityTaskWoken);
}
bool StreamBuffer::setTriggerLevel(const size_t triggerLevel)
{
    return xStreamBufferSetTriggerLevel(stream, triggerLevel) == pdTRUE;
}
size_t StreamBuffer::getSpaceAvailable()
{
    return xStreamBufferSpacesAvailable(stream);
}

StreamBuffer::StreamBuffer(StreamBuffer &&other) noexcept
{
    (*this) = std::forward<StreamBuffer>(other);
}

StreamBuffer &StreamBuffer::operator=(StreamBuffer &&other) noexcept
{
    stream = std::exchange(other.stream, nullptr);
    return *this;
}
size_t StreamBuffer::getBytesAvailable()
{
    return xStreamBufferBytesAvailable(stream);
}

} // namespace util::wrappers