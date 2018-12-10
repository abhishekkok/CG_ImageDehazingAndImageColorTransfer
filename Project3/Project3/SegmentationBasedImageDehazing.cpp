#include "SegmentationBasedImageDehazing.h"
#include "ImageDehazing.h"

void SegmentationBasedImageDehazing::performSegmentationBasedDehazing(string fileName, int minimiumEdge)
{	
	Mat img = imread(fileName);
	Mat darkChannelImage = imread(fileName); // read orignal and  convert it to darkchannel
	ImageGraphSegmentation egbs = ImageGraphSegmentation();
	egbs.applySegmentation(img, 5, minimiumEdge, darkChannelImage);//10,20

	string segmentedImage = "segmentedImage.jpg";
	string segmentedDarkChannel = "segmentedDarkChannel.jpg";
	string finalImage = "segmentationDehazerFileResult.jpg";

	imwrite(segmentedImage, img);
	imwrite(segmentedDarkChannel, darkChannelImage);
	//write segmented images to file 

	ImageDehazing dehazer ; //use standard dehazer code on the segmented Image
	dehazer.loadImage(segmentedImage);
	dehazer.dehazeImageWithSegmentation(darkChannelImage,fileName);
	dehazer.writeImage(finalImage);
	
	display(fileName, segmentedDarkChannel , segmentedImage ,finalImage);
}

void SegmentationBasedImageDehazing::display(string fileName , string segmentationDarkChannel,string segmentedImage, string finalImage) {

	namedWindow("orignalImage", WINDOW_KEEPRATIO);
	imshow("orignalImage", imread(fileName));

	namedWindow("segmentedDarkChannelImage", WINDOW_KEEPRATIO);
	imshow("segmentedDarkChannelImage", imread(segmentationDarkChannel));

	namedWindow("segmentedImage", WINDOW_KEEPRATIO);
	imshow("segmentedImage", imread(segmentedImage));

	namedWindow("finalImage", WINDOW_KEEPRATIO);
	imshow("finalImage", imread(finalImage));

	waitKey(0);
	destroyAllWindows();

}
