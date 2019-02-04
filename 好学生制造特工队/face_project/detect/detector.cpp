#include "detector.h"
#include "vector"
#include "string"

detector::detector(const std::string model_path) : mtcnn(model_path)
{
}

std::vector<int> detector::detectAFrame(cv::Mat &frame)
{
    std::vector<int> locationInfo;
    clock_t start_time = clock();
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR, frame.cols, frame.rows);

    std::vector<Bbox> finalBbox;
    mtcnn.NcnnDetect(ncnn_img, finalBbox);
    const int num_box = finalBbox.size();
    std::vector<cv::Rect> bbox;
    bbox.resize(num_box);

    //put info into vector
    locationInfo.push_back(num_box);
    for (int i = 0; i < num_box; i++)
    {
        locationInfo.push_back(finalBbox[i].x1);
        locationInfo.push_back(finalBbox[i].y1);
        locationInfo.push_back(finalBbox[i].x2 - finalBbox[i].x1 + 1);
        locationInfo.push_back(finalBbox[i].y2 - finalBbox[i].y1 + 1);
    }
    clock_t finish_time = clock();
    double total_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "detect a frame time" << total_time * 1000 << "ms" << std::endl;
    return locationInfo;
}