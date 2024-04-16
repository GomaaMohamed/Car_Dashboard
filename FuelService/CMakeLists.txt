# Define source files
set(SRCS
    src/fuelservice_main.cpp
    src/FuelService.cpp)

# Add target for the application
add_executable(fuel_app)

# Assign sources to the target
target_sources(fuel_app PRIVATE ${SRCS})

# Assign include to the target
target_include_directories(fuel_app
    PUBLIC include
    PRIVATE src)

# Link with required libraries
target_link_libraries(fuel_app ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
