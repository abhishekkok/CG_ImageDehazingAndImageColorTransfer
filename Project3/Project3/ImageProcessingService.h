#pragma once
#include <string>
#include "ImageConvolution.h"
#include "ImageDehazing.h"
#include "ImageGraphSegmentation.h"
#include "GamutImageColorConvolution.h"
using namespace cv;
using namespace std;
class ImageProcessingService
{
public:
	
	void ImageDehazingTechnique(int index, string fileName);

	void ImageColorTransferTechnique(int index, string sourceFile, string targetFile);

};

