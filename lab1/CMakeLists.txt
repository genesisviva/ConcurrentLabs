cmake_minimum_required(VERSION 3.0.0)
project(lab1 VERSION 0.1.0)

include_directories(/home/genesisviva/libs/boost_1_71_0)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )


add_executable(lab1 main.cpp)
add_executable(child child.cpp)

TARGET_LINK_LIBRARIES(lab1 ${Boost_LIBRARIES})






