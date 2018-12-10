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

class ImageDehazing
{
public:
	void loadImage(const std::string& _filename);
	uchar getFinalValforIndividualChannel(double t, int channelIndex, int row, int col);
	bool dehazeImage(const int& _patchsize, const double& _t, const double& _w);
	cv::Mat dehazeImageWithSegmentation(cv::Mat darkChannelviaSegmentation , cv::Mat orignalImage);
	bool writeImage(const std::string filename);
	void generateDarkChannelImage(const int& _patchsize);
	unsigned char getMinimumOfRGBforPatch(const int& _patchsize, int matRows, int matCols, int row, int col);
	double atmosphericLightEstimater();
	// 
	void addTransmissionMedium(const int& _patchsize, const double& _t, const double& _w);
	void display();
	//void displayAll(String sourceFile, String targetFileName, String outputFileName);
	//void ShowImg(Mat image);
private:
	cv::Mat matImage;
	cv::Mat matDoubleImage;
	cv::Mat darkChannelMatImage;
	cv::Mat matRecoveredImage;
	double atmosphericLight;
	
};

