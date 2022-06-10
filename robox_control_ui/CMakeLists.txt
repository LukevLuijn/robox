cmake_minimum_required(VERSION 3.16)
project(robox_control_ui)
set(EXE robox)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wconversion -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Threads REQUIRED)
find_package(wxWidgets REQUIRED)

include(${wxWidgets_USE_FILE})

include_directories(lib/serial)

set(LIB_SERIAL
        lib/serial/AsyncSerial.h
        lib/serial/AsyncSerial.cpp)

include_directories(include/frame)
include_directories(include/logger)
include_directories(include/base)
include_directories(include/config)
include_directories(include/driver)
include_directories(include/utils)
include_directories(include/widgets)
include_directories(include/event)
include_directories(src/)

set(HEADERS_BASE
        include/base/Controller.h
        include/base/MainApplication.h)
set(HEADERS_CONFIG
        include/config/global_config.h
        include/config/msg_protocol.h)
set(HEADERS_DRIVER
        include/driver/DataManager.h
        include/driver/RobotDriver.h
        include/driver/SerialDriver.h
        include/driver/RobotController.h)
set(HEADERS_FRAME
        include/frame/MainFrame.h
        include/frame/FrameInterface.h
        include/base/FrameBuilder.h
        include/frame/ManualCtrl.h
        include/widgets/DrawPanel.h
        include/frame/RobotDrawPanel.h
        include/frame/BKECtrl.h)
set(HEADERS_LOGGER
        include/logger/Logger.h
        include/logger/LoggerInterface.h
        include/logger/LogMessage.h
        include/logger/LogToConsole.h
        include/logger/LogToWidget.h)
set(HEADERS_UTILS
        include/utils/Bitmap.h
        include/utils/String.h
        include/utils/Point.h
        include/utils/Kinematics.h)
set(HEADERS_WIDGETS
        include/widgets/ConcreteMessagePanels.h
        include/widgets/LogMessagePanel.h
        include/widgets/LogPanel.h
        include/widgets/ScrolledLogWindow.h)
set(SOURCES
        src/ConcreteMessagePanels.cpp
        src/Controller.cpp
        src/FrameBuilder.cpp
        src/Logger.cpp
        src/LogMessagePanel.cpp
        src/LogPanel.cpp
        src/LogToConsole.cpp
        src/LogToWidget.cpp
        src/MainFrame.cpp
        src/ScrolledLogWindow.cpp
        src/RobotDriver.cpp
        src/RobotController.cpp
        src/SerialDriver.cpp
        src/DataManager.cpp
        src/Kinematics.cpp
        src/ManualCtrl.cpp
        src/DrawPanel.cpp
        src/RobotDrawPanel.cpp
        src/MainApplication.cpp
        src/BKECtrl.cpp
        src/main.cpp)

set(HEADERS
        ${HEADERS_BASE}
        ${HEADERS_CONFIG}
        ${HEADERS_DRIVER}
        ${HEADERS_EVENT}
        ${HEADERS_FRAME}
        ${HEADERS_LOGGER}
        ${HEADERS_UTILS}
        ${HEADERS_WIDGETS})

add_executable(${EXE} ${SOURCES} ${HEADERS} ${LIB_SERIAL})

target_link_libraries(${EXE} ${wxWidgets_LIBRARIES})
target_link_libraries(${EXE} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS ${EXE} RUNTIME DESTINATION ${PROJECT_SOURCE_DIR})
