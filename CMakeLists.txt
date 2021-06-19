project(Galaxy_camera_driver)
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

# set(OpenCV_DIR /home/artinx001/3rdparty/opencv-3.4.12/build)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Install libserial with
# sudo apt install libserial-dev
# find_package(PkgConfig)
# pkg_check_modules(SERIAL libserial)
# find_package(SERIAL REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/inc
)

add_subdirectory(src)
add_subdirectory(sample)

# add_executable(
#     show_capture_image
#         ${PROJECT_SOURCE_DIR}/src/GxCamera.cc
#         ${PROJECT_SOURCE_DIR}/sample/ShowCaptureImage.cc
# )

# target_link_libraries(
#     show_capture_image 
#         ${OpenCV_LIBS}
#         libgxiapi.so          # /usr/lib/libgxiapi.so
# )
