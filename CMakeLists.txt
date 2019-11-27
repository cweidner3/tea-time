cmake_minimum_required(VERSION 3.13)

set(PROJNAME tea-time)
project(${PROJNAME} C CXX)

#-------------------------------------------------------------------------------
# Qt Libraries

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)

set(QTLIBS
    Core
    Gui
    Widgets
    Sql
)

foreach(lib ${QTLIBS})
    find_package(Qt5${lib})
endforeach()

#-------------------------------------------------------------------------------
# Other Libraries

find_package(fmt REQUIRED)

#-------------------------------------------------------------------------------

add_definitions(-std=gnu++17 -Wall -Wextra -pthread)
add_definitions(-Og -g)

#-------------------------------------------------------------------------------

# All files here will be packaged into a convenience library which can also
# be used for the unit tests.
file(GLOB_RECURSE SRCFILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)
# Remove the main file from the list
list(REMOVE_ITEM SRCFILES "${CMAKE_SOURCE_DIR}/src/main.cpp")

#-------------------------------------------------------------------------------

# Convienience library
add_library(mainlib ${SRCFILES})
# Main executable that defines the project
add_executable(${PROJNAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
# Include directories for the project
target_include_directories(${PROJNAME}
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------

# Linking the libraries to the targets
target_link_libraries(${PROJNAME} mainlib)
target_link_libraries(mainlib fmt::fmt-header-only)
# Link each of the QT libraries
foreach(lib ${QTLIBS})
    target_link_libraries(${PROJNAME} Qt5::${lib})
    target_link_libraries(mainlib Qt5::${lib})
endforeach()

#-------------------------------------------------------------------------------

# Requried for unit testing
enable_testing()

# Add boost to the setup
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)
# Find all tests
file(GLOB TEST_SRCS "${CMAKE_SOURCE_DIR}/test/*.cpp")

foreach(test_src ${TEST_SRCS})
    # Extract the filename without an extension
    get_filename_component(test_name ${test_src} NAME_WE)
    # Each test will have a new executable
    add_executable(${test_name} ${test_src})
    target_include_directories(${test_name}
        PRIVATE "${CMAKE_SOURCE_DIR}/src"
        SYSTEM "${Boost_INCLUDE_DIRS}"
    )
    target_link_libraries(${test_name}
        mainlib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    # Set destination location for binaries.
    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/testBin
    )
    # Add tests to the test stack
    add_test(NAME ${test_name}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/testBin
        COMMAND ${CMAKE_BINARY_DIR}/testBin/${test_name}
    )
endforeach(test_src)

#-------------------------------------------------------------------------------

include(GNUInstallDirs)

set(ICON_DIRS_TBD
    1024x1024
    512x512
    384x384
    32x32
    22x22
    20x20
    )
set(ICON_DIRS
    256x256
    192x192
    128x128
    96x96
    72x72
    64x64
    48x48
    40x40
    36x36
    24x24
    16x16
    )

string(REGEX REPLACE "([^;]+)" "icon-dev/\\1" ICON_PATHS "${ICON_DIRS}")

install(TARGETS ${PROJNAME} RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES save/tea-time.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(DIRECTORY ${ICON_PATHS}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor)

