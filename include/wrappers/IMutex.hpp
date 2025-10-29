#pragma once
#include <FreeRTOS.h>

namespace util::wrappers
{
class IMutex
{
public:
    virtual ~IMutex() = default;

    /// @brief  Locks the mutex, waits without timeout for acquisition.
    virtual void lock() = 0;

    /// @brief  Unlocks the mutex from an interrupt service routine, waits without timeout for
    /// acquisition.
    virtual void lockFromISR() = 0;

    /**
     * Attempts to lock the mutex within given time.
     * @param timeToWait maximum FreeRTOS ticks to wait
     * @return false if acquisition was not possible in time. true on success
     */
    virtual bool lockWithTimeout(TickType_t timeToWait) = 0;

    virtual bool lockFromISRWithTimeout(TickType_t timeToWait) = 0;

    /**
     * Unlocks the mutex.
     */
    virtual void unlock() = 0;

    /**
     * Unlocks the mutex from an interrupt service routine.
     */
    virtual void unlockFromISR() = 0;
};
} // namespace util::wrappers