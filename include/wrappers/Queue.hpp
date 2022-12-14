#pragma once
#include <FreeRTOS.h>
#include <core/SafeAssert.h>
#include <cstdint>
#include <optional>
#include <queue.h>
#include <utility>

namespace util::wrappers
{
template <typename T>
class Queue
{
public:
    explicit Queue(size_t length) : queueHandle(xQueueCreate(length, sizeof(T)))
    {
        SafeAssert(queueHandle != nullptr);
    };

    ~Queue()
    {
        vQueueDelete(queueHandle);
    }

    Queue(const Queue &other) = delete;
    void operator=(const Queue &other) = delete;

    Queue(Queue &&other) noexcept
    {
        (*this) = std::forward<Queue>(other);
    };

    Queue &operator=(Queue &&other) noexcept
    {
        queueHandle = std::exchange(other.queueHandle, nullptr);
        return *this;
    };

    [[nodiscard]] BaseType_t getMessagesWaiting() const
    {
        return uxQueueMessagesWaiting(queueHandle);
    }
    [[nodiscard]] BaseType_t getMessagesWaitingFromISR() const
    {
        return uxQueueMessagesWaitingFromISR(queueHandle);
    }

    void sendOverwrite(const T &item) const
    {
        xQueueOverwrite(queueHandle, static_cast<const void *>(&item));
    }
    void sendOverwriteFromISR(const T &item, BaseType_t &pxHigherPriorityTaskWoken) const
    {
        xQueueOverwriteFromISR(queueHandle, static_cast<const void *>(&item),
                               &pxHigherPriorityTaskWoken);
    }
    bool send(const T &item, TickType_t ticksToWait) const
    {
        return xQueueSend(queueHandle, static_cast<const void *>(&item), ticksToWait) == pdPASS;
    }
    bool sendFromISR(const T &item, BaseType_t &pxHigherPriorityTaskWoken) const
    {
        return xQueueSendFromISR(queueHandle, static_cast<const void *>(&item),
                                 &pxHigherPriorityTaskWoken) == pdPASS;
    }

    std::optional<T> peek(TickType_t ticksToWait) const
    {
        T data;
        const auto Res = xQueuePeek(queueHandle, static_cast<void *>(&data), ticksToWait);
        if (Res == pdPASS)
        {
            return {data};
        }
        return {};
    }
    std::optional<T> peekFromISR() const
    {
        T data;
        const auto Res = xQueuePeekFromISR(queueHandle, static_cast<void *>(&data));
        if (Res == pdPASS)
        {
            return {data};
        }
        return {};
    }

    std::optional<T> receive(TickType_t ticksToWait) const
    {
        T data;
        const auto Res = xQueueReceive(queueHandle, static_cast<void *>(&data), ticksToWait);
        if (Res == pdPASS)
        {
            return {data};
        }
        return {};
    }
    std::optional<T> receiveFromISR(BaseType_t &pxHigherPriorityTaskWoken) const
    {
        T data;
        const auto Res = xQueueReceiveFromISR(queueHandle, static_cast<void *>(&data),
                                              &pxHigherPriorityTaskWoken);
        if (Res == pdPASS)
        {
            return {data};
        }
        return {};
    }

    void reset() const
    {
        xQueueReset(queueHandle);
    }

private:
    QueueHandle_t queueHandle{nullptr};
};

} // namespace util::wrappers