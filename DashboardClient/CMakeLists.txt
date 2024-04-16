# Tell CMake to run the Qt tools moc, rcc, and uic automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configure Qt Paths
# If Qt6 is not found, you can manually specify the Qt6_DIR
set(Qt6_DIR "/usr/local/qt/lib/cmake/Qt6")

# If necessary, you can set CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "/usr/local/qt")

# Find Qt6 components
find_package(Qt6 COMPONENTS Core Quick Gui Qml REQUIRED)

# Define an executable target
add_executable(dashboard_app)

# Extend the executable target
add_custom_command(TARGET dashboard_app POST_BUILD
    COMMAND echo "------- Built successfully ---------")

# Add the include directries for a target
target_include_directories(
    dashboard_app
    PRIVATE Backend/src
    PUBLIC Backend/include)

# Add the sources of the target
set(SRCS
    Backend/src/dashboardclient_main.cpp
    Backend/src/Updater.cpp
    Backend/src/DashboardClient.cpp
    qml.qrc)

# force cmake to generate a new header file contains the meta object
qt_wrap_cpp(SPEEDOMETER_MOC_SRCS Backend/include/Updater.hpp)

target_sources(
    dashboard_app
    PRIVATE ${SRCS}
    ${SPEEDOMETER_MOC_SRCS})

# Define the language standard
target_compile_features(
    dashboard_app
    PUBLIC cxx_std_17)
# tell cmake to link the executable to the Qt 6 core and test modules
target_link_libraries(dashboard_app
    PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    ${VSOMEIP_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}
)