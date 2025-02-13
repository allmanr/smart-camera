#pragma once
#include "event_detector.hpp"
#include <opencv2/opencv.hpp>

class MotionDetector : public IEventDetector {
public:
    explicit MotionDetector(float threshold = 30.0);
    bool detectEvent(const Frame& frame) override;
    void notify(const EventContext& ctx) override;

private:
    cv::Mat previous_frame_;
    float threshold_;
};