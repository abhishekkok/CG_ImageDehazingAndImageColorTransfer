#include "SegmentationBasedImageDehazing.h"
#include "ImageDehazing.h"

void SegmentationBasedImageDehazing::performImageSegmentationBasedDehazing(string fileName)
{
	#include "ImageDehazing.h"
	Mat img = imread(fileName);
	Mat darkChannelImage = imread(fileName);
	ImageGraphSegmentation egbs = ImageGraphSegmentation();
	egbs.applySegmentation(darkChannelImage, 5, 30);//10,20
	
	namedWindow("orignalImage", WINDOW_KEEPRATIO);
	imshow("orignalImage", img);

	namedWindow("darkChannelImage", WINDOW_KEEPRATIO);
	imshow("darkChannelImage", darkChannelImage);

	ImageDehazing dehazer ;
	dehazer.dehazeImageWithSegmentation(darkChannelImage,img);
	dehazer.writeImage("segmentationDehazerFileResult.jpg");
	Mat result = imread("segmentationDehazerFileResult.jpg");
	namedWindow("finalImage", WINDOW_KEEPRATIO);
	imshow("finalImage", result);
	
	waitKey(0);
	destroyWindow("orignalImage");
	destroyWindow("darkChannelImage");
	destroyWindow("finalImage");
}
