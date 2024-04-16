# Define source files
set(SRCS
    src/temperatureservice_main.cpp
    src/TemperatureService.cpp)

# Add target for the application
add_executable(temperature_app)

# Assign sources to the target
target_sources(temperature_app PRIVATE ${SRCS})

# Assign include to the target
target_include_directories(temperature_app
    PUBLIC include
    PRIVATE src)

# Link with required libraries
target_link_libraries(temperature_app ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
