#ifndef FRAME_UNIT_H
#define FRAME_UNIT_H

#include "stdint.h"
#include "util.h"
#include "vector"

using namespace std;

class frameUnit
{
    public:
        void setFrameTime(int64_t _frameTime) {  
            frame_time = _frameTime; 
        }

        void setFrameCount(int32_t _frameCount) {
            frame_count = _frameCount;  
        }

        void setFrameOrder(int32_t _frameOrder) { 
            frame_order = _frameOrder;
        }

        void setFrameData(vector<byte> _frameData) {
            frame_data = _frameData;
        }

        void setFrameDataLen(int32_t _dataLen) {
            frame_dataLen = _dataLen;
        }

        int64_t getFrameTime(){ 
            return frame_time; 
        }

        int32_t getFrameCount() {
            return frame_count;
        }

        int32_t getFrameOrder() {
            return frame_order;
        }

        int32_t getFrameDataLen()
        {
            return frame_dataLen;
        }

        vector<byte> getFrameData(){
            return frame_data;
        }
        
    private:
        int64_t frame_time;
        int32_t frame_count;
        int32_t frame_order;
        int32_t frame_dataLen;
        vector<byte> frame_data;
};
#endif