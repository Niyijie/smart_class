#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include"iostream"
#include "string"
using namespace std;
using namespace cv;

int main()
{
	VideoCapture capture("rtsp://admin:admin123456@192.168.0.64/Streaming/Channels/2");
    
    if (!capture.isOpened())
	{
		printf("can't open camera...\n");
		return -1;
	}
    cout<<"opened"<<endl;
	namedWindow("window",CV_WINDOW_NORMAL);
    resizeWindow("window",500,500);
    int i = 0;
	while(true)
	{	
		Mat frame;
		capture>>frame;

        //std::vector<unsigned char> data_encode;
        //imencode(".jpg", frame, data_encode);
        //std::string str_encode(data_encode.begin(), data_encode.end());
        //unsigned char* s = &(*data_encode.begin());
		imshow("window",frame);
        //imwrite("new.jpg"+to_string(i),frame);
		char c = waitKey(20);                           
        if( c == 27)
            break;
	}

    // Mat frame;
    // frame=imread("1.jpg");   
    // if (!frame.data)
    //     cerr << "Opening capture error!" << endl;
    // detection(frame);
    // waitKey(15);
    // return 0;
}

