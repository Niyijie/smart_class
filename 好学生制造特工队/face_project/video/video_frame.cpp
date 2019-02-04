#include "video_frame.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"


std::vector<byte> VideoFrame::getJPEG() const
{
    const std::vector<int> compression_params = {
        cv::IMWRITE_JPEG_QUALITY,
        JPEGQuarity};
    std::vector<byte> databuffer;
    cv::imencode(JPEGExtension,frame_image,databuffer,compression_params);
    return databuffer;
}

cv::Mat VideoFrame::getFrameMat()
{
    return frame_image;
}
