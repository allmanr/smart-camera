class V4L2Camera {
public:
    V4L2Camera(const std::string& device_path) {
        fd_ = open(device_path.c_str(), O_RDWR);
        if(fd_ < 0) {
            throw std::runtime_error("Failed to open device");
        }
        initializeDevice();
    }
    
    ~V4L2Camera() {
        stopStreaming();
        close(fd_);
    }

    void startStreaming() {
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(ioctl(fd_, VIDIOC_STREAMON, &type) < 0) {
            throw std::runtime_error("Failed to start streaming");
        }
    }

    std::vector<uint8_t> captureFrame() {
        struct v4l2_buffer buf = {0};
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        
        // Dequeue buffer
        if(ioctl(fd_, VIDIOC_DQBUF, &buf) < 0) {
            throw std::runtime_error("Failed to dequeue buffer");
        }

        // Copy frame data
        std::vector<uint8_t> frame_data(
            static_cast<uint8_t*>(buffers_[buf.index].start),
            static_cast<uint8_t*>(buffers_[buf.index].start) + buf.bytesused
        );

        // Requeue the buffer
        if(ioctl(fd_, VIDIOC_QBUF, &buf) < 0) {
            throw std::runtime_error("Failed to requeue buffer");
        }

        return frame_data;
    }

private:
    struct Buffer {
        void* start;
        size_t length;
    };

    int fd_;
    std::vector<Buffer> buffers_;

    void initializeDevice() {
        // Query capabilities
        struct v4l2_capability cap;
        if(ioctl(fd_, VIDIOC_QUERYCAP, &cap) < 0) {
            throw std::runtime_error("Failed to query capabilities");
        }

        // Set format
        struct v4l2_format fmt = {0};
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = 1280;
        fmt.fmt.pix.height = 720;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        
        if(ioctl(fd_, VIDIOC_S_FMT, &fmt) < 0) {
            throw std::runtime_error("Failed to set format");
        }

        initializeBuffers();
    }

    void initializeBuffers() {
        // Request buffers
        struct v4l2_requestbuffers req = {0};
        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if(ioctl(fd_, VIDIOC_REQBUFS, &req) < 0) {
            throw std::runtime_error("Failed to request buffers");
        }

        // Map buffers
        buffers_.resize(req.count);
        for(unsigned int i = 0; i < req.count; ++i) {
            struct v4l2_buffer buf = {0};
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            if(ioctl(fd_, VIDIOC_QUERYBUF, &buf) < 0) {
                throw std::runtime_error("Failed to query buffer");
            }

            buffers_[i].length = buf.length;
            buffers_[i].start = mmap(
                NULL,
                buf.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd_,
                buf.m.offset
            );

            if(buffers_[i].start == MAP_FAILED) {
                throw std::runtime_error("Failed to map buffer");
            }
        }
    }
};