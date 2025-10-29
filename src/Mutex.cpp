#include "wrappers/Mutex.hpp"
#include <utility>

namespace util::wrappers
{
Mutex::Mutex() : mutexHandle(xSemaphoreCreateMutex())
{
    configASSERT(mutexHandle != nullptr);
}

//-----------------------------------------------------------------
Mutex::~Mutex()
{
    if (mutexHandle != nullptr)
    {
        vSemaphoreDelete(mutexHandle);
    }
}

//-----------------------------------------------------------------
void Mutex::lock()
{
    xSemaphoreTake(mutexHandle, portMAX_DELAY);
}

//-----------------------------------------------------------------
void Mutex::lockFromISR()
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    xSemaphoreTakeFromISR(mutexHandle, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

bool Mutex::lockWithTimeout(const TickType_t timeToWait)
{
    return xSemaphoreTake(mutexHandle, timeToWait) == pdPASS;
}

bool Mutex::lockFromISRWithTimeout(const TickType_t timeToWait)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    return xSemaphoreTakeFromISR(mutexHandle, &higherPriorityTaskWoken) == pdPASS;
}

//-----------------------------------------------------------------
void Mutex::unlock()
{
    xSemaphoreGive(mutexHandle);
}

//-----------------------------------------------------------------
void Mutex::unlockFromISR()
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(mutexHandle, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

//-----------------------------------------------------------------
Mutex::Mutex(Mutex &&other) noexcept
{
    (*this) = std::move(other);
}

//-----------------------------------------------------------------
Mutex &Mutex::operator=(Mutex &&other) noexcept
{
    mutexHandle = std::exchange(other.mutexHandle, nullptr);
    return *this;
}

} // namespace util::wrappers
