cmake_minimum_required(VERSION 3.10)
project(KeepAlive C)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


set(RESOURCE_FILE src/app.rc)
file(WRITE ${RESOURCE_FILE} "400 ICON \"${CMAKE_SOURCE_DIR}/assets/app.ico\"\n")
file(APPEND ${RESOURCE_FILE} "1 24 \"${CMAKE_SOURCE_DIR}/src/app.manifest\"")

set(SOURCES
    src/keepalive.c
    src/tray.c
    src/timer.c
    src/power.c
    ${RESOURCE_FILE}
)

add_executable(KeepAlive WIN32 ${SOURCES})

set_target_properties(KeepAlive PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup /MANIFEST:NO"
)

target_link_libraries(KeepAlive user32 comctl32 shell32)


file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}) 