#include "ImageConvolution.h"


Mat ImageConvolution::convertRGBToCIE(Mat rgbImage)
{
	Mat sourceImgCie;
	cvtColor(rgbImage, sourceImgCie, COLOR_BGR2Lab);
	return sourceImgCie;
}


Mat ImageConvolution::LoadImages(string imageName)
{
	return imread(imageName);
}

vector<Mat>  ImageConvolution::getModifiedTargetChannels(vector<Mat>sourceChannels, Mat targetImageCIE)
{
	vector<Mat>	targetChannels;	
	split(targetImageCIE, targetChannels);
	for (int i = 0; i < 3; i++) {
		Mat temporaryMean, temporaryStddev;
		meanStdDev(sourceChannels[i], temporaryMean, temporaryStddev);

		double meanSource = temporaryMean.at<double>(0);
		double stddevSource = temporaryStddev.at<double>(0);

		/* mean and stddev for Target image */
		meanStdDev(targetChannels[i], temporaryMean, temporaryStddev);
		double meanTarget = temporaryMean.at<double>(0);
		double stddevTarget = temporaryStddev.at<double>(0);

		/* Fit the color distribution from target LAB to our source LAB */
		targetChannels[i].convertTo(targetChannels[i], CV_64FC1); // convert the channel value to double 
		targetChannels[i] -= meanTarget; //Subtracing mean from the 
		targetChannels[i] *= (stddevTarget / stddevSource);
		targetChannels[i] += meanSource;
		targetChannels[i].convertTo(targetChannels[i], CV_8UC1);
	}
	return targetChannels;
}
Mat ImageConvolution::doImageColorTransfer(Mat sourceImageCIE, Mat targetImageCIE)
{
	vector<Mat> sourceChannels;
	split(sourceImageCIE, sourceChannels);
	vector<Mat>	targetChannels = getModifiedTargetChannels(sourceChannels, targetImageCIE);
	Mat finalmg;
	merge(targetChannels, finalmg);
	cvtColor(finalmg, finalmg, COLOR_Lab2BGR);
	return finalmg;

}


Mat ImageConvolution::getConvolutedImage(String source_img, String  target_img)
{
	Mat sourceCie = convertRGBToCIE(LoadImages(source_img));
	Mat targetCie = convertRGBToCIE(LoadImages(target_img));
	return doImageColorTransfer(sourceCie, targetCie);
}

void ImageConvolution::display(Mat image)
{
	imshow("Image", image);
}

bool ImageConvolution::writeImage(string filename,Mat image) {
	if (image.empty())
		return false;
	return imwrite(filename, image);
}

void ImageConvolution::displayAll(String sourceImage, String targetImage, String outputImage)
{
	namedWindow("sourceImage", WINDOW_KEEPRATIO);
	imshow("sourceImage", LoadImages(sourceImage));

	namedWindow("targetImage", WINDOW_KEEPRATIO);
	imshow("targetImage", LoadImages(targetImage));

	namedWindow("outputImage", WINDOW_KEEPRATIO);
	imshow("outputImage", LoadImages(outputImage));

	waitKey(0);

	destroyAllWindows();
	cout << "Destoryed all temp Windows ";
}

