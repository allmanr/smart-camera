# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(security_camera)

# Set OpenCV_DIR explicitly
set(OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source files
set(SOURCES
    src/camera_device.cpp
    src/motion_detector.cpp
    src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
    v4l2
)