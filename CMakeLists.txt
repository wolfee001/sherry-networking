cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(sherry-networking)

# download conan.cmake to have an easy use of conan in Visual Studio
# see: https://docs.conan.io/en/latest/howtos/vs2017_cmake.html
if(NOT EXISTS "${CMAKE_HOME_DIRECTORY}/conan.cmake")
message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake"
		"${CMAKE_HOME_DIRECTORY}/conan.cmake")
endif()
include(${CMAKE_HOME_DIRECTORY}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt BUILD missing BASIC_SETUP)

add_library(networking STATIC
	implementation/server/concrete_multiconnection_server.cpp
	implementation/server/multiconnection_server.cpp
	implementation/client/concrete_client.cpp
	implementation/client/client.cpp
)
conan_target_link_libraries(networking PUBLIC ${CONAN_LIBS})

add_executable(network_test test_package/networking_test.cpp)
target_link_libraries(network_test networking)
