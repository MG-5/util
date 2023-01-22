#include "wrappers/Task.hpp"
#include <algorithm>
#include <helpers/freertos.hpp>
#include <units/si/time.hpp>
#include <utility>

#include "esp_log.h"

namespace util::wrappers
{
size_t Task::taskListIndex{0};
std::array<TaskHandle_t, Task::MaxTasks> Task::taskList{};
EventGroupHandle_t Task::syncEventGroup = xEventGroupCreate();

Task::Task(TaskFunction_t taskCode, const char *name, uint16_t stackDepth, void *parameter,
           UBaseType_t priority)
    : IFreeRTOSTask(name, stackDepth, priority), taskCode(taskCode), taskParameter(parameter)
{
    assert(taskCode != nullptr);

    // ESP-IDF want bytes (uint8_t), not uint32_t as stack size
    constexpr auto Factor = sizeof(uint32_t) / sizeof(portSTACK_TYPE);
    const size_t StackSizeInBytes = stackDepth * Factor;

    xTaskCreate(&Task::taskMain, name, StackSizeInBytes, reinterpret_cast<void *>(this), priority,
                &taskHandle);

    assert(taskHandle != nullptr);

    ESP_LOGI("[Task]", "Created task \"%s\" with %d 32-bit frames stack and prio: %d.", name,
             stackDepth, priority);

    registerTask(taskHandle);
}

void Task::registerTask(TaskHandle_t handle)
{
    if (taskListIndex == 0)
    {
        std::fill(taskList.begin(), taskList.end(), nullptr);
    }
    assert(taskListIndex < taskList.size());
    taskList.at(taskListIndex++) = handle;
}

int32_t Task::notifyWait(const uint32_t ulBitsToClearOnEntry, const uint32_t ulBitsToClearOnExit,
                         uint32_t *pulNotificationValue, const uint32_t xTicksToWait)
{
    return xTaskNotifyWait(ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue,
                           xTicksToWait);
}

int32_t Task::notify(const uint32_t ulValue, const NotifyAction eAction)
{
    return xTaskNotify(taskHandle, ulValue, notifyActionConverter(eAction));
}

int32_t Task::notifyFromISR(const uint32_t ulValue, const NotifyAction eAction,
                            int32_t *pxHigherPriorityTaskWoken)
{
    return xTaskNotifyFromISR(taskHandle, ulValue, notifyActionConverter(eAction),
                              pxHigherPriorityTaskWoken);
}

Task::~Task()
{
    if (taskHandle != nullptr)
    {
        vTaskDelete(taskHandle);
    }
}

[[noreturn]] void Task::taskMain(void *instance)
{
    Task *task = reinterpret_cast<Task *>(instance);
    xEventGroupWaitBits(syncEventGroup, AllTasksWaitFlag, pdFALSE, pdFALSE, portMAX_DELAY);
    task->taskCode(task->taskParameter);
    for (;;)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

void Task::notifyGive()
{
    xTaskNotifyGive(taskHandle);
}

void Task::notifyTake(const uint32_t waittime)
{
    ulTaskNotifyTake(pdTRUE, waittime);
}

void Task::applicationIsReadyStartAllTasks()
{
    xEventGroupSetBits(syncEventGroup, AllTasksWaitFlag);
}

constexpr eNotifyAction Task::notifyActionConverter(const NotifyAction action)
{
    switch (action)
    {
    case NotifyAction::NoAction:
        return eNotifyAction::eNoAction;

    case NotifyAction::SetBits:
        return eNotifyAction::eSetBits;

    case NotifyAction::Increment:
        return eNotifyAction::eIncrement;

    case NotifyAction::SetValueWithOverwrite:
        return eNotifyAction::eSetValueWithOverwrite;

    default:
    case NotifyAction::SetValueWithoutOverwrite:
        return eNotifyAction::eSetValueWithoutOverwrite;
    }
}

void Task::delay(const units::si::Time time)
{
    vTaskDelay(toOsTicks(time));
}

Task::Task(Task &&other) noexcept
    : IFreeRTOSTask(other.taskName, other.taskStackDepth, other.taskPriority)
{
    *this = std::forward<Task>(other);
}

Task &Task::operator=(Task &&other) noexcept
{
    taskHandle = std::exchange(other.taskHandle, nullptr);
    taskCode = std::exchange(other.taskCode, nullptr);
    taskParameter = std::exchange(other.taskParameter, nullptr);
    return *this;
}

} // namespace util::wrappers
