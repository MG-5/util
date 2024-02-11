#pragma once

/// Priority values.
using osPriority_t = enum {
    osPriorityNone = 0,              ///< No priority (not initialized).
    osPriorityIdle = 1,              ///< Reserved for Idle thread.
    osPriorityLow = 2,               ///< Priority: low
    osPriorityLow1 = 2 + 1,          ///< Priority: low + 1
    osPriorityLow2 = 2 + 2,          ///< Priority: low + 2
    osPriorityLow3 = 2 + 3,          ///< Priority: low + 3
    osPriorityBelowNormal = 6,       ///< Priority: below normal
    osPriorityBelowNormal1 = 6 + 1,  ///< Priority: below normal + 1
    osPriorityBelowNormal2 = 6 + 2,  ///< Priority: below normal + 2
    osPriorityBelowNormal3 = 6 + 3,  ///< Priority: below normal + 3
    osPriorityNormal = 10,           ///< Priority: normal
    osPriorityNormal1 = 10 + 1,      ///< Priority: normal + 1
    osPriorityNormal2 = 10 + 2,      ///< Priority: normal + 2
    osPriorityNormal3 = 10 + 3,      ///< Priority: normal + 3
    osPriorityAboveNormal = 14,      ///< Priority: above normal
    osPriorityAboveNormal1 = 14 + 1, ///< Priority: above normal + 1
    osPriorityAboveNormal2 = 14 + 2, ///< Priority: above normal + 2
    osPriorityAboveNormal3 = 14 + 3, ///< Priority: above normal + 3
    osPriorityHigh = 18,             ///< Priority: high
    osPriorityHigh1 = 18 + 1,        ///< Priority: high + 1
    osPriorityHigh2 = 18 + 2,        ///< Priority: high + 2
    osPriorityHigh3 = 18 + 3,        ///< Priority: high + 3
    osPriorityRealtime = 22,         ///< Priority: realtime
    osPriorityRealtime1 = 22 + 1,    ///< Priority: realtime + 1
    osPriorityRealtime2 = 22 + 2,    ///< Priority: realtime + 2
};