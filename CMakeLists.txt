# CMakeList.txt : CMake project for BeagleboneMusicQueue, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project (BeagleboneMusicQueue)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add source to this project's executable.
add_executable (BeagleboneMusicQueue
	src/Main.cpp
	src/Main.h

	src/Logging/Logger.cpp
	src/Logging/Logger.h
	src/Logging/easylogging++.cc
	src/Logging/easylogging++.h
)
