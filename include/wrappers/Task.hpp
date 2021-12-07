#pragma once
#include <FreeRTOS.h>
#include <array>
#include <event_groups.h>
#include <limits>
#include <task.h>

namespace util::wrappers
{

class Task
{
public:
    Task(TaskFunction_t taskCode, const char *name, uint16_t stackDepth, void *parameter,
         UBaseType_t priority);
    ~Task();

    Task(const Task &) = delete;
    Task(Task &&) = delete;
    Task &operator=(const Task &) = delete;
    Task &operator=(Task &&) = delete;

    static constexpr uint32_t ClearAllBits = std::numeric_limits<uint32_t>::max();
    BaseType_t notifyWait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit,
                          uint32_t *pulNotificationValue, TickType_t xTicksToWait);

    BaseType_t notify(uint32_t ulValue, eNotifyAction eAction);

    BaseType_t notifyFromISR(uint32_t ulValue, eNotifyAction eAction,
                             BaseType_t *pxHigherPriorityTaskWoken);

    void notifyGive();
    void notifyTake(TickType_t waittime);

    static constexpr uint8_t MaxTasks = 6;
    static std::array<TaskHandle_t, MaxTasks> &getAllTaskHandles()
    {
        return taskList;
    }
    static void registerTask(TaskHandle_t);

    static void applicationIsReadyStartAllTasks();
private:
    TaskHandle_t _handle{nullptr};
    TaskFunction_t _taskCode;
    void *_parameter;

    [[noreturn]] static void taskMain(void *);

    static std::array<TaskHandle_t, MaxTasks> taskList;
    static size_t taskListIndex;
    static EventGroupHandle_t syncEventGroup;
    static constexpr EventBits_t AllTasksWaitFlag = 1<<0;
};

} // namespace util::wrappers