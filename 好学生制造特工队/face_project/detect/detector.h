#ifndef DETECTOR_H
#define DETECTOR_H

#include "mtcnn.h"
#include "string"
#include "iostream"

class detector
{
    public:
        explicit detector(const std::string model_path);
        
        std::vector<int> detectAFrame(cv::Mat &frame);
    private:
        MTCNN mtcnn;
};
#endif