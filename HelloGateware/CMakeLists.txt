cmake_minimum_required(VERSION 3.10)

project(HelloGateware)

# currently using unicode in some libraries on win32 but will change soon
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)


if(APPLE)
    add_executable (HelloGateware main.mm)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fmodules -fcxx-modules")
    #set(Architecture ${CMAKE_OSX_ARCHITECTURES})
else()

    add_executable (HelloGateware main.cpp)

    if (UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()

endif()        
