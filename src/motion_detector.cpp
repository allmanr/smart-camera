#include "motion_detector.hpp"
#include <iostream>

MotionDetector::MotionDetector(float threshold)
    : threshold_(threshold) {}

bool MotionDetector::detectEvent(const Frame& frame) {
    cv::Mat current_frame, current_gray, prev_gray, diff;
    
    cv::cvtColor(frame.data, current_gray, cv::COLOR_BGR2GRAY);
    
    if (previous_frame_.empty()) {
        previous_frame_ = current_gray.clone();
        return false;
    }

    cv::absdiff(current_gray, previous_frame_, diff);
    cv::threshold(diff, diff, threshold_, 255, cv::THRESH_BINARY);
    
    double pixel_diff = (cv::countNonZero(diff) * 100.0) / 
                       (diff.rows * diff.cols);
    
    previous_frame_ = current_gray.clone();
    
    return pixel_diff > threshold_;
}

void MotionDetector::notify(const EventContext& ctx) {
    std::cout << "Motion detected at " 
              << std::chrono::system_clock::to_time_t(ctx.timestamp)
              << " with confidence " << ctx.confidence << std::endl;
    
    std::string filename = "motion_" + 
                          std::to_string(std::chrono::system_clock::to_time_t(ctx.timestamp)) + 
                          ".jpg";
    cv::imwrite(filename, ctx.frame.data);
}