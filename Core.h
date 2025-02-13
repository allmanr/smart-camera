// Base camera interface using RAII principles
class CameraDevice {
    public:
        CameraDevice(const std::string& device);
        ~CameraDevice();
        
        Frame captureFrame();
        void startStream();
        void stopStream();
    
    private:
        std::unique_ptr<V4L2Device> device_;
        // ... other members
};

// Event detection interface
class IEventDetector {
    public:
        virtual bool detectEvent(const Frame& frame) = 0;
        virtual void notify(const EventContext& ctx) = 0;
};

// Concrete motion detector
class MotionDetector : public IEventDetector {
    public:
        bool detectEvent(const Frame& frame) override;
        void notify(const EventContext& ctx) override;
    
    private:
        cv::Mat previous_frame_;
        float threshold_;
};