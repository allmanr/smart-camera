#include "camera_device.hpp"
#include "motion_detector.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    try {
        CameraDevice camera("/dev/video0");
        camera.startStream();
        
        MotionDetector detector(30.0);
        
        while (true) {
            auto frame = camera.captureFrame();
            
            if (detector.detectEvent(frame)) {
                EventContext ctx{
                    frame,
                    "motion",
                    1.0,
                    std::chrono::system_clock::now()
                };
                detector.notify(ctx);
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}