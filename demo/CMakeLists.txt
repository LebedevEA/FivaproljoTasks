cmake_minimum_required(VERSION 3.15)

project(demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

file(GLOB sources *.h src/*.cpp)
# SET(CMAKE_CXX_FLAGS  "-Wall -Wextra -Werror")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# attempt to make qt work anywhere

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(WINDOWS TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

if(LINUX)
   set(CMAKE_PREFIX_PATH "/home/daniel/Qt5.14.1/5.14.1/gcc_64") # for Daniel only
elseif(MACOSX)
   set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.14.1") # Any macOS
endif()

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
add_executable(demo ${sources} include/Model.h include/Block.h include/Menu.h include/MenuButton.h)
target_link_libraries(demo Qt5::Widgets Qt5::Multimedia)
# qt block end

