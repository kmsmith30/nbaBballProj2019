# NBA BBall Project
# Project made using C++11 in Windows Visual Studio 2017
# Author: Kevin M. Smith <kmsmith3@vt.edu>
# Version: May 30, 2019

cmake_minimum_required(VERSION 3.5)
project(NBA-BBALL CXX)

# BBall files
set(bball_src
  event.hpp event.cpp
  player.hpp player.cpp
  team.hpp team.cpp
  game.hpp game.cpp
  )

add_executable(bball bball.cpp ${bball_src})

set_property(TARGET bball PROPERTY CXX_STANDARD 11)