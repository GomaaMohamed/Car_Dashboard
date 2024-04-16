# Define source files
set(SRCS
    src/speedservice_main.cpp
    src/SpeedService.cpp)

# Add target for the application
add_executable(speed_app)

# Assign sources to the target
target_sources(speed_app PRIVATE ${SRCS})

# Assign include to the target
target_include_directories(speed_app
    PUBLIC include
    PRIVATE src)

# Link with required libraries
target_link_libraries(speed_app ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
