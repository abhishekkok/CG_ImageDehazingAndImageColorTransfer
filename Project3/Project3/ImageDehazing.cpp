#include "ImageDehazing.h"
#include <queue>
#include <functional>
using namespace std;
using namespace cv;

void ImageDehazing::loadImage(const std::string& _filename) {

	matImage = imread(_filename, IMREAD_COLOR); // read image in BGR way 
}


bool ImageDehazing::writeImage(string filename) {
	if (matRecoveredImage.empty())
		return false;
	return imwrite(filename, matRecoveredImage);
}


void ImageDehazing::generateDarkChannelImage(const int& _patchsize)
{
	darkChannelMatImage.create(matImage.rows, matImage.cols, CV_8UC1); //creating the empty image to fill out
	for (int i = 0; i < matImage.rows; ++i) {
		for (int j = 0; j < matImage.cols; ++j) {
			unsigned char DarkVal = 255;
			DarkVal = getMinimumOfRGBforPatch(_patchsize, matImage.rows, matImage.cols, i, j);
			darkChannelMatImage.at<uchar>(i, j) = DarkVal;
		}
	}
	imwrite("patchBasedDarkChannel.jpg", darkChannelMatImage);
}

unsigned char ImageDehazing::getMinimumOfRGBforPatch(const int& _patchsize, int matRows, int matCols, int row, int col)
{
	unsigned char darkVal = 255;
	for (int m = row - _patchsize / 2; m <= row + _patchsize / 2; ++m) {
		for (int n = col - _patchsize / 2; n <= col + _patchsize / 2; ++n) {
			if (m < 0 || n < 0 || m >= matImage.rows || n >= matImage.cols)
				continue;
			unsigned char color1 = matImage.at<Vec3b>(m, n)[0];
			unsigned char color2 = matImage.at<Vec3b>(m, n)[1];
			unsigned char color3 = matImage.at<Vec3b>(m, n)[2];
			darkVal = std::min(darkVal,std::min(std::min(color1, color2), color3));
		}
	}

	return darkVal;
}

double ImageDehazing::atmosphericLightEstimater()
{
	//find out the 0.1% highest pixels in the dark channel
	// 
	std::priority_queue<uchar, vector<uchar>, std::greater<uchar>> maxDarkChannelValues;

	int priorityQMaxSize = darkChannelMatImage.rows * darkChannelMatImage.cols;
	priorityQMaxSize = priorityQMaxSize / 1000;
	double sumTotal = 0;

	for (int i = 0; i < darkChannelMatImage.rows; i++)
	{
		for (int j = 0;j < darkChannelMatImage.cols; j++)
		{
			uchar pixelIntensity = darkChannelMatImage.at<uchar>(i, j);
			//keep on adding intensity of pixel if stack is empty
			if (maxDarkChannelValues.size() < priorityQMaxSize) {
				maxDarkChannelValues.push(pixelIntensity);
				sumTotal += pixelIntensity;
			}
			else {
				//remove top most value if stack is full and intensity of new pixel is more 
				if (maxDarkChannelValues.top() >= pixelIntensity)
					continue;
				sumTotal -= maxDarkChannelValues.top();
				maxDarkChannelValues.pop();
				maxDarkChannelValues.push(pixelIntensity);
				sumTotal += pixelIntensity;
			}
		}
	}
	sumTotal /= priorityQMaxSize;

	return sumTotal;
}

void ImageDehazing::addTransmissionMedium(const int& _patchsize, const double& _t, const double& _w)
{
	matRecoveredImage.create(matImage.rows, matImage.cols, CV_8UC3); //create Empty image
	//cout << " t Outside " << _t << " \n";
	for (int i = 0; i < matImage.rows;i++)
	{
		for (int j = 0; j < matImage.cols; j++)
		{ 
			//if (darkChannelMatImage.at<uchar>(i, j) == '0')
				//cout << "zero inside " << "\n";
			double t = std::max(1 - (_w*darkChannelMatImage.at<uchar>(i, j) / atmosphericLight), _t); // if the value is tooo small take _t as the default...
			//cout << t << " ";
			matRecoveredImage.at<Vec3b>(i, j)[0] = getFinalValforIndividualChannel(t, 0, i, j);
			matRecoveredImage.at<Vec3b>(i, j)[1] = getFinalValforIndividualChannel(t, 1, i, j);
			matRecoveredImage.at<Vec3b>(i, j)[2] = getFinalValforIndividualChannel(t, 2, i, j);
		}
		//cout <<"\n";
	}


	return;
}

uchar ImageDehazing::getFinalValforIndividualChannel(double t, int channelIndex, int row, int col)
{
	double val1 = (matImage.at<Vec3b>(row, col)[channelIndex] - atmosphericLight) / t + atmosphericLight;
	return static_cast<uchar>(std::min(val1, 255.0));
}

bool ImageDehazing::dehazeImage(const int& _patchSize, const double& _t, const double& _w) {
	if (matImage.empty())
		return false;
	generateDarkChannelImage(_patchSize);
	atmosphericLight = atmosphericLightEstimater();
	cout << "atmospheric Light " << atmosphericLight << " \n";
	addTransmissionMedium(_patchSize, _t, _w);
	return true;
}

cv::Mat ImageDehazing::dehazeImageWithSegmentation(Mat darkChannelviaSegmentation, const std::string& orignalfileName) {

	generateDarkChannelImage(3); //chosing default minimum patch size for segmented Image	
	atmosphericLight = atmosphericLightEstimater();
	loadImage(orignalfileName); //applying dark channel based estimation on orignal File
	addTransmissionMedium(3,0.01, 0.95); // fixing weights and t 	
	cout << "atmospheric Light " << atmosphericLight << " \n";
	return matRecoveredImage;
}

void ImageDehazing::display()
{
	namedWindow("OrignalImage", WINDOW_KEEPRATIO);
	imshow("OrignalImage", matImage);

	namedWindow("DarkChannelImage", WINDOW_KEEPRATIO);
	imshow("DarkChannelImage", darkChannelMatImage);

	namedWindow("FinalOut", WINDOW_KEEPRATIO);
	imshow("FinalOut", matRecoveredImage);

	waitKey(0);
	destroyAllWindows();
	cout << "Destoryed all temp Windows ";
}

