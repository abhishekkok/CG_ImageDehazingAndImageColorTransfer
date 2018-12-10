#include <opencv2/opencv.hpp>
#include "ImageGraphSegmentation.h"

ImageGraphSegmentation::ImageGraphSegmentation() {

}

void ImageGraphSegmentation::applySegmentation(cv::Mat &image, int c, int min_size , cv::Mat &darkChannel) {
	int height = image.rows;
	int width = image.cols;

	cv::Mat smoothed(image);
	smoothed.convertTo(smoothed, CV_32FC3);
	GaussianBlur(smoothed, smoothed, cv::Size(5, 5), 0.5); //Bilateral Filter

	std::vector <Edge> edges = constructGraph(smoothed);
	DisjointSet forest = segmentIntoSubGraph(height * width, edges, c);

	// process small components
	for (Edge &edge : edges) {
		int a = forest.find(edge.a);
		int b = forest.find(edge.b);

		if ((a != b) && ((forest.size(a) < min_size) || (forest.size(b) < min_size))) {
			forest.merge(a, b);
		}
	}
	recolor(image, forest);
	darkChannelUsingDisjointSet(darkChannel, forest);
}

void ImageGraphSegmentation::recolor(cv::Mat &image, DisjointSet &forest) {
	int height = image.rows;
	int width = image.cols;

	// calculate average colors for components
	std::unordered_map<int, Component> colors;
	std::unordered_map<int, cv::Vec3f> averageColor;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = forest.find(y * width + x);
			colors[comp] += Component{ 1, image.at<cv::Vec3b>(y, x) };
		}
	}

	for (auto itr : colors)
		averageColor[itr.first] = itr.second.getAverageColor();

	// apply average colors
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = forest.find(y * width + x);
			image.at<cv::Vec3b>(y, x) = averageColor[comp];
		}
	}
}

void ImageGraphSegmentation::darkChannelUsingDisjointSet(cv::Mat &image, DisjointSet &forest) {
	int height = image.rows;
	int width = image.cols;

	// calculate average colors for components
	std::unordered_map<int, Component> colors;
	unsigned char darkVal = 255;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = forest.find(y * width + x);
			colors[comp] += Component{ 1, image.at<cv::Vec3b>(y, x) };
			unsigned char color1 = image.at<Vec3b>(y, x)[0];
			unsigned char color2 = image.at<Vec3b>(y, x)[1];
			unsigned char color3 = image.at<Vec3b>(y, x)[2];
			darkVal = std::min(std::min(color1, color2), color3);
			colors[comp].setLeastDarkVal(darkVal);
		}
	}

	
	// apply average colors
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = forest.find(y * width + x);
			image.at<Vec3b>(y, x)[0] = colors[comp].getLeastDarkVal();
			image.at<Vec3b>(y, x)[1] = colors[comp].getLeastDarkVal();
			image.at<Vec3b>(y, x)[2] = colors[comp].getLeastDarkVal();
			//image.at<cv::Vec3b>(y, x) = colors[comp].getLeastDarkVal();
		}
	}
}