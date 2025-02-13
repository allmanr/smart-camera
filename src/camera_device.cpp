#include "camera_device.hpp"
#include <stdexcept>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

V4L2Device::V4L2Device(const std::string& device)
    : device_path_(device), fd_(-1), is_streaming_(false) {}

V4L2Device::~V4L2Device() {
    if (fd_ >= 0) {
        if (is_streaming_) {
            stopStreaming();
        }
        close(fd_);
    }
}

bool V4L2Device::open() {
    fd_ = ::open(device_path_.c_str(), O_RDWR);
    return fd_ >= 0;
}

bool V4L2Device::startStreaming() {
    if (fd_ < 0) return false;
    
    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd_, VIDIOC_STREAMON, &type) < 0) {
        return false;
    }
    
    is_streaming_ = true;
    return true;
}

void V4L2Device::stopStreaming() {
    if (fd_ >= 0 && is_streaming_) {
        v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        ioctl(fd_, VIDIOC_STREAMOFF, &type);
        is_streaming_ = false;
    }
}

std::vector<uint8_t> V4L2Device::captureFrame() {
    // Basic frame capture implementation
    // You'll need to implement proper V4L2 buffer handling here
    std::vector<uint8_t> buffer(640 * 480 * 3); // Example size
    // ... implement actual V4L2 frame capture
    return buffer;
}

CameraDevice::CameraDevice(const std::string& device) {
    device_ = std::make_unique<V4L2Device>(device);
    if (!device_->open()) {
        throw std::runtime_error("Failed to open camera device: " + device);
    }
}

CameraDevice::~CameraDevice() {
    if (device_) {
        stopStream();
    }
}

Frame CameraDevice::captureFrame() {
    Frame frame;
    frame.timestamp = std::chrono::system_clock::now();
    
    auto raw_frame = device_->captureFrame();
    frame.data = cv::imdecode(raw_frame, cv::IMREAD_COLOR);
    
    return frame;
}

void CameraDevice::startStream() {
    if (!device_->startStreaming()) {
        throw std::runtime_error("Failed to start camera stream");
    }
}

void CameraDevice::stopStream() {
    device_->stopStreaming();
}