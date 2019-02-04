#ifndef VIDEO_FRAME_H
#define VIDEO_FRAME_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "vector"
#include "string"

#define byte unsigned char


class VideoFrame
{
  public:
    explicit VideoFrame(const cv::Mat &image) : 
    frame_image(image),JPEGExtension(".jpg"),JPEGQuarity(25){};

    cv::Mat getFrameMat();
    std::vector<byte> getJPEG() const;

  private:
    std::string JPEGExtension;
    int JPEGQuarity;
    cv::Mat frame_image;
};

#endif