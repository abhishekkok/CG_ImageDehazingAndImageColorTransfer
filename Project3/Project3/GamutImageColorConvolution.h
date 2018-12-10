#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <memory>
using namespace std;
using namespace cv;
class GamutImageColorConvolution
{
public:

	cv::Mat gamutImageColorTransfer(String sourceImg, String  targetImg);
	Mat LoadImages(string imageName);
	void displayAll(string sourceImage, string targetImage, string outputImage, string whiteBalanceImage);
	void whiteBalancing(cv::Mat mat);
	void undoWhiteBalancing(cv::Mat target);
	int vmin[3], vmax[3];
};

