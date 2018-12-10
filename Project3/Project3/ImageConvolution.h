#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;
class ImageConvolution
{
public:
	Mat LoadImages(string imageName);
	Mat convertRGBToCIE(Mat rgbImage);
	vector<Mat> getModifiedTargetChannels(vector<Mat> sourceChannels, Mat targetImageCIE);
	Mat doImageColorTransfer(Mat sourceImageCIE, Mat targetImageCIE);
	Mat getConvolutedImage(String source_img, String  target_img);
	void display(Mat image);
	bool writeImage(string filename, Mat image);
	void displayAll(String sourceImage, String targetImage, String outputImage);
};

