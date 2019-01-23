//
//  Created by ShaoQiJun on 2018/12/17.
//
#pragma once

#ifndef __MTCNN_H__
#define __MTCNN_H__
#include "../common/include/net.h"
#include <opencv2/opencv.hpp>
#include<string>
#include<vector>
#include <time.h>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;

#define OPENCV 0
#define MORE 1

struct Bbox
{
	float score;
	int x1;
	int y1;
	int x2;
	int y2;
	float area;
	float ppoint[10];
	float regreCoord[4];
};

class MTCNN
{
public:
	MTCNN(const string &model_path);
	~MTCNN();

	void SetMinFace(int minSize);
	void NcnnDetect(ncnn::Mat &img_, std::vector<Bbox> &finalBbox_);
	void OpencvDetec(cv::Mat &img, std::vector<cv::Rect> &finalBbox);
private:
	void generateBbox(ncnn::Mat score, ncnn::Mat location, vector<Bbox> &boundingBox_, float scale);
	void nms(vector<Bbox> &boundingBox_, const float overlap_threshold, string modelname = "Union");
	void refine(vector<Bbox> &vecBox, const int &height, const int &width, bool square);
	void PNet();
	void RNet();
	void ONet();

	ncnn::Net Pnet, Rnet, Onet;
	ncnn::Mat img;

	const float nms_threshold[3] = { 0.6f,0.4f,0.4f };
	const float mean_vals[3] = { 127.5,127.5,127.5 };
	const float norm_vals[3] = { 0.0078125,0.0078125,0.0078125 };
	const int MIN_DET_SIZE = 12;
	std::vector<Bbox> firstBbox_, secondBbox_, thirdBbox_;
	int img_w, img_h;

private:
	const float threshold[3] = { 0.6f,0.6f,0.7f };
	int minsize = 36;
	const float pre_facetor = 0.709f;

};

#endif