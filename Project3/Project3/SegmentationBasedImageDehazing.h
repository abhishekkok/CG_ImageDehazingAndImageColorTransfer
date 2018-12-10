#pragma once
#include <string>
#include "ImageGraphSegmentation.h"
using namespace std;

class SegmentationBasedImageDehazing
{
public:
	
	void performSegmentationBasedDehazing(string fileName,int minimiumEdge);
	void display(string fileName, string segmentationDarkChannel, string segmentedImage, string finalImage);
};

