cmake_minimum_required(VERSION 3.15)

idf_component_register(
    SRCS
        "src/Button.cxx"
        "src/RecursiveMutex.cpp"
        "src/Mutex.cpp"
        "src/ScopedLock.cpp"
        "src/Task.cpp"
        "src/BinarySemaphore.cpp"
        "src/StreamBuffer.cpp"
        "src/EventGroup.cpp"

    INCLUDE_DIRS
        "include"

    PRIV_REQUIRES
        esp_driver_gpio
)