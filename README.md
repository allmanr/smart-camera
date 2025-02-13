# Security Camera System

A C++ security camera system using OpenCV and V4L2 for motion detection.

## Dependencies
- OpenCV (>= 4.0)
- V4L2
- CMake (>= 3.10)
- C++17 compiler

## Building
```bash
mkdir build
cd build
cmake ..
make
```

## Running
```bash
./security_camera
```

Note: Make sure you have proper permissions to access the camera device (/dev/video0).