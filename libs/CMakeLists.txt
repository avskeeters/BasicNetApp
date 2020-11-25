## CMAkeLists.txt for libs ##

cmake_minimum_required(VERSION 3.5)
project(MyLibraries VERSION 0.0 LANGUAGES CXX)

set(NetworkingLibPublicHeaders 
    include/NetworkingLib/Shared.h
    include/NetworkingLib/SocketAddress.h
    include/NetworkingLib/UDPSocket.h
    include/NetworkingLib/SocketUtilities.h
)

set(NetworkingLibSources
    src/SocketAddress.cpp
    src/UDPSocket.cpp
    src/SocketUtilities.cpp
)

add_library(NetworkingLib ${NetworkingLibSources} ${NetworkingLibPublicHeaders})

set_target_properties(NetworkingLib
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES)

target_include_directories(NetworkingLib
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS NetworkingLib
EXPORT NetworkingLib-export
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)

install(EXPORT NetworkingLib-export
FILE
NetworkingLibTargets.cmake
NAMESPACE
MyLibraries::
DESTINATION
lib/cmake/NetworkingLib
)

install(FILES
${NetworkingLibPublicHeaders}
DESTINATION "include/NetworkingLib"
)

install(FILES
${CMAKE_CURRENT_SOURCE_DIR}/cmake/NetworkingLibConfig.cmake
DESTINATION "lib/cmake/NetworkingLib")