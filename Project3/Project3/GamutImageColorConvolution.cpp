#include "GamutImageColorConvolution.h"
#include "ImageConvolution.h"
using namespace std;
using namespace cv;



void GamutImageColorConvolution::whiteBalancing(cv::Mat mat) {
	double discardRatio = 0.05;
	int imageHistogram[3][256];
	memset(imageHistogram, 0, 3 * 256 * sizeof(int));

	for (int y = 0; y < mat.rows; ++y) {
		uchar* ptr = mat.ptr<uchar>(y);
		for (int x = 0; x < mat.cols; ++x) {
			for (int j = 0; j < 3; ++j) {
				imageHistogram[j][ptr[x * 3 + j]] += 1;
			}
		}
	}

	// cumulative hist
	int total = mat.cols*mat.rows;
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 255; ++j) {
			imageHistogram[i][j + 1] += imageHistogram[i][j];
		}
		vmin[i] = 0;
		vmax[i] = 255;
		while (imageHistogram[i][vmin[i]] < discardRatio * total)
			vmin[i] += 1;
		while (imageHistogram[i][vmax[i]] > (1 - discardRatio) * total)
			vmax[i] -= 1;
		if (vmax[i] < 255 - 1)
			vmax[i] += 1;
	}


	for (int y = 0; y < mat.rows; ++y) {
		uchar* ptr = mat.ptr<uchar>(y);
		for (int x = 0; x < mat.cols; ++x) {
			for (int j = 0; j < 3; ++j) {
				int val = ptr[x * 3 + j];
				if (val < vmin[j])
					val = vmin[j];
				if (val > vmax[j])
					val = vmax[j];
				ptr[x * 3 + j] = static_cast<uchar>((val - vmin[j]) * 255.0 / (vmax[j] - vmin[j]));
			}
		}
	}
}

void GamutImageColorConvolution::undoWhiteBalancing(cv::Mat target) {
	
	for (int y = 0; y < target.rows; ++y) {
		uchar* ptr = target.ptr<uchar>(y);
		for (int x = 0; x < target.cols; ++x) {
			for (int j = 0; j < 3; ++j) {
				int val = ptr[x * 3 + j];
				val = val * (vmax[j] - vmin[j]) / 255.0;
				val = val + vmin[j] ;
				if (val < vmin[j])
					val = vmin[j];
				if (val > vmax[j])
					val = vmax[j];
				ptr[x * 3 + j] = static_cast<uchar>(val); // - vmin[j]) * 255.0 / (vmax[j] - vmin[j]));
			}
		}
	}	
}

Mat GamutImageColorConvolution::gamutImageColorTransfer(String source_img, String  target_img)
{
	ImageConvolution colorTransfer = ImageConvolution();

	Mat source = colorTransfer.LoadImages(source_img);
	Mat target = colorTransfer.LoadImages(target_img);

	whiteBalancing(source);
	whiteBalancing(target);
	imwrite("whiteBalancedTarget.jpg", target);
	//namedWindow("whiteBalancedTarget", WINDOW_KEEPRATIO); //Using openCV window just for Display
	//imshow("whiteBalancedTarget", source);

	Mat sourceCie = colorTransfer.convertRGBToCIE(source);
	Mat targetCie = colorTransfer.convertRGBToCIE(target);
	Mat result = colorTransfer.doImageColorTransfer(sourceCie, targetCie);

	//namedWindow("whiteBalancedAndCTE", WINDOW_KEEPRATIO);  //Using openCV window just for Display
	//imshow("whiteBalancedAndCTE", result);

	undoWhiteBalancing(result);

	//namedWindow("finalOutput", WINDOW_KEEPRATIO);  //Using openCV window just for Display
	//imshow("finalOutput", result);
	 //Using openCV to writeintoFile

	return result;
}


Mat GamutImageColorConvolution::LoadImages(string imageName)
{
	return imread(imageName);
}

void GamutImageColorConvolution::displayAll(string sourceImage, string targetImage, string outputImage, string whiteBalanceImage)
{
	namedWindow("sourceImage", WINDOW_KEEPRATIO);
	imshow("sourceImage", LoadImages(sourceImage));

	namedWindow("targetImage", WINDOW_KEEPRATIO);
	imshow("targetImage", LoadImages(targetImage));

	namedWindow("outputImage", WINDOW_KEEPRATIO);
	imshow("outputImage", LoadImages(outputImage));

	namedWindow("targetWhiteBalancedImage", WINDOW_KEEPRATIO);
	imshow("targetWhiteBalancedImage", LoadImages(whiteBalanceImage));

	waitKey(0);
	destroyWindow("sourceImage");
	destroyWindow("targetImage");
	destroyWindow("outputImage");
	destroyWindow("targetWhiteBalancedImage");
	cout << "Destoryed all temp Windows ";
}