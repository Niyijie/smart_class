#include "../common/sockets.h"
#include "../common/util.h"
#include "../common/frameUnit.h"
#include "../common/message.h"
#include "../common/mtcnn.h"
#include "map"

#define PI_PORT "9190"
using namespace cv;

struct matUnit
{
    matUnit(cv::Mat _frame, int _box_num)
    {
        frame = _frame;
        box_num = _box_num;
    }
    cv::Mat frame;
    int box_num;
};

std::vector<matUnit> test_more_img();
std::vector<int> detectAFrame(cv::Mat &frame, MTCNN &mtcnn);
void test_video();

byte buf[2048];

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //tcp server listen
    int serv_sock = Socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    socklen_t serv_addr_len;
    addr_init(&serv_addr, AF_INET, INADDR_ANY, argv[1]);
    if (connect(serv_sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        cout << "connet error!!" << endl;
    }
    else
    {
        cout << "Connecting......" << endl;
    }
    //send login message
    writeDataIntoBUf(buf, 0, PI_LOGIN_MESSAGE);
    write(serv_sock, buf, 4);
    bzero(buf, sizeof(buf));
    //receive send message
    int dataLen, port;
    dataLen = read(serv_sock, buf, 4);
    int type = bytesto_int4(buf);
    //get serv port
    if (type == SERV_PORT_MESSAGE)
    {
        dataLen = read(serv_sock, buf, 4);
        port = bytesto_int4(buf);
    }
    else
    {
        cout << "message type error!!" << endl;
        return 0;
    }

    sockaddr_in serv_udp_addr;
    size_t str_len;
    socklen_t serv_udp_addr_len;
    //init serv udp addr
    char port_s[255];
    sprintf(port_s, "%d", port);
    int sender_fd = Socket(PF_INET, SOCK_DGRAM, 0);
    addr_init(&serv_udp_addr, AF_INET, INADDR_ANY, port_s);

    /*********************************************************************/
    //char *model_path = "./models/";
    //MTCNN mtcnn(model_path);
    /*********************************************************************/
    int64_t preTime = 0;

    //VideoCapture capture(-1);
    vector<matUnit> content = test_more_img();
    int index = 0;
    Mat frame;
    while (true)
    {
        // if (!capture.read(frame))
        // {
        //     cout << "读取视频失败" << endl;
        //     break;
        // }
        //capture >> frame;
        /********************************************************/

        /********************************************************/

        if (content.size() == index)
        {
            index = 0;
        }

        matUnit strUnit = content[index++];
        frame = strUnit.frame;

        vector<frameUnit> unitSet = getFrameUnitSet(frame);
        //packet timeframe + pic_count + order + data_len + data
        byte packet[8 + 4 + 4 + 4 + MAX_SIZE];
        int32_t total = 0;
        for (frameUnit unit : unitSet)
        {
            //get frame time
            int64_t timeFrame = unit.getFrameTime();
            byte timeBytes[8];
            int8to_bytes(timeFrame, timeBytes);
            //get frame count
            int32_t frameCount = unit.getFrameCount();
            byte countBytes[4];
            int4to_bytes(frameCount, countBytes);
            //get frame order
            int32_t frameOrder = unit.getFrameOrder();
            byte orderBytes[4];
            int4to_bytes(frameOrder, orderBytes);
            //get data len
            int32_t frameDataLen = unit.getFrameDataLen();
            byte dataLenBytes[4];
            int4to_bytes(frameDataLen, dataLenBytes);
            //get frame data
            vector<byte> dataBytes = unit.getFrameData();
            byte *dataPtr = &(*dataBytes.begin());

            //put in packet
            arrayCopy(timeBytes, 0, packet, 0, 8);
            arrayCopy(countBytes, 0, packet, 8, 4);
            arrayCopy(orderBytes, 0, packet, 12, 4);
            arrayCopy(dataLenBytes, 0, packet, 16, 4);
            arrayCopy(dataPtr, 0, packet, 20, dataBytes.size());
            //send to
            total += frameDataLen;
            sendto(sender_fd, packet, sizeof(packet), 0, (sockaddr *)&serv_udp_addr, sizeof(serv_udp_addr));

            // if (timeFrame - preTime >= 1000000) //if bigger than 1s get a frame
            // {
            //     cout << "analysy.........................." << endl;
            //     preTime = timeFrame;
            //     vector<int> BoxLocation = detectAFrame(frame,mtcnn);
            //     writeDataIntoBUf(buf, 0, PI_BOXS_LOCATION_MESSAGE);
            //     writeDataIntoBUf(buf, 4, (int32_t)BoxLocation.size());
            //     /*
            //     *  note!!! if bigger than 2048 will error
            //     */
            //     for (size_t i = 0; i < BoxLocation.size(); i++)
            //     {
            //         writeDataIntoBUf(buf, (i + 2) * 4, BoxLocation[i]);
            //     }
            //     write(serv_sock, buf, 8 + BoxLocation.size() * 4);
            //     bzero(buf, sizeof(buf));
            // }
        }
        //send box num
        writeDataIntoBUf(buf,0,PI_BOXS_MESSAGE);
        writeDataIntoBUf(buf,4,strUnit.box_num);
        write(serv_sock, buf, 8);
        bzero(buf,sizeof(buf));
        cout << "total data: " << total << endl;
        sleep(1);
    }
    close(sender_fd);
    return 0;
}


std::vector<matUnit> test_more_img()
{
    char *model_path = "./models/"; //模型路径
    MTCNN mtcnn(model_path);
    std::vector<matUnit> content;
    cv::Mat image;
    std::string name;
    std::vector<std::string> vname;
    ifstream in("test_list.txt"); //测试图片路径，完整路径写进txt.
    while (in >> name)
    {
        vname.push_back(name);
        cout << name << endl;
    }

    int id_n = 0;
    cv::Mat crop;
    for (int i = 0; i < vname.size(); i++)
    {
        clock_t start_time = clock();
        image = cv::imread(vname[i]);
        int img_h = image.rows;
        int img_w = image.cols;
        //ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, image.cols, image.rows);
        ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR, image.cols, image.rows);
        std::vector<Bbox> finalBbox;
#if (MAXFACEOPEN == 1)
        mtcnn.detectMaxFace(ncnn_img, finalBbox);
#else
        mtcnn.NcnnDetect(ncnn_img, finalBbox);
#endif
        const int num_box = finalBbox.size();

        //score
        std::vector<cv::Rect> bbox;
        bbox.resize(num_box);

        cv::Mat new_image = image.clone();

        for (int i = 0; i < num_box; i++)
        {
            bbox[i] = cv::Rect(finalBbox[i].x1, finalBbox[i].y1, finalBbox[i].x2 - finalBbox[i].x1 + 1, finalBbox[i].y2 - finalBbox[i].y1 + 1);
        }

        for (vector<cv::Rect>::iterator it = bbox.begin(); it != bbox.end(); it++)
        {
            rectangle(image, (*it), cv::Scalar(0, 0, 255), 2, 8, 0);
        }
        cout << "num box = "
             << " " << num_box << endl;
        //imshow("face_detection", image);
        matUnit unit(image, num_box);
        content.push_back(unit);

        clock_t finish_time = clock();
        double total_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
        std::cout << "time" << total_time * 1000 << "ms" << std::endl;
    }
    return content;
}

vector<int> detectAFrame(cv::Mat &frame, MTCNN &mtcnn)
{
    std::vector<int> locationInfo;
    clock_t start_time = clock();
    ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR, frame.cols, frame.rows);
   
    std::vector<Bbox> finalBbox;
    mtcnn.NcnnDetect(ncnn_img, finalBbox);
    const int num_box = finalBbox.size();
    cout<<"num of boxs: " << num_box<<endl;
    //score
    std::vector<cv::Rect> bbox;
    bbox.resize(num_box);

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
