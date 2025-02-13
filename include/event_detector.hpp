#pragma once
#include <opencv2/opencv.hpp>
#include "camera_device.hpp"

struct EventContext {
    Frame frame;
    std::string event_type;
    double confidence;
    std::chrono::system_clock::time_point timestamp;
};

class IEventDetector {
public:
    virtual ~IEventDetector() = default;
    virtual bool detectEvent(const Frame& frame) = 0;
    virtual void notify(const EventContext& ctx) = 0;
};