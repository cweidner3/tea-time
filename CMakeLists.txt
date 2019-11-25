cmake_minimum_required(VERSION 3.13)

set(PROJNAME tea-time)
project(${PROJNAME} C CXX)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

#-------------------------------------------------------------------------------

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
)

foreach(lib ${QTLIBS})
    find_package(Qt5${lib})
endforeach()

#-------------------------------------------------------------------------------

add_definitions(-std=gnu++17 -Wall -Wextra -pthread)
add_definitions(-Og -g)

#-------------------------------------------------------------------------------

file(GLOB_RECURSE SRCFILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

#-------------------------------------------------------------------------------

# Tell CMake to create the helloworld executable
add_executable(${PROJNAME} ${SRCFILES})

target_include_directories(${PROJNAME}
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)

#-------------------------------------------------------------------------------

# Use the Widgets module from Qt 5
foreach(lib ${QTLIBS})
    target_link_libraries(${PROJNAME} Qt5::${lib})
endforeach()


