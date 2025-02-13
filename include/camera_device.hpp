#pragma once
#include <memory>
#include <string>
#include <opencv2/opencv.hpp>

struct Frame {
    cv::Mat data;
    std::chrono::system_clock::time_point timestamp;
};

class V4L2Device {
public:
    V4L2Device(const std::string& device);
    ~V4L2Device();
    
    bool open();
    bool startStreaming();
    void stopStreaming();
    std::vector<uint8_t> captureFrame();

private:
    int fd_;
    std::string device_path_;
    bool is_streaming_;
};

class CameraDevice {
public:
    CameraDevice(const std::string& device);
    ~CameraDevice();
    
    Frame captureFrame();
    void startStream();
    void stopStream();

private:
    std::unique_ptr<V4L2Device> device_;
};