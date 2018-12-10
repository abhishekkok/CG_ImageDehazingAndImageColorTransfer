#pragma once
//ImageGraphSegmentation
#ifndef __ImageGraphSegmentation__H__
#define __ImageGraphSegmentation__H__

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include "graph.h"
#include "DisjointSet.h"

struct Component {
	int count;
	cv::Vec3f colorSum;
	unsigned char leastDarkVal=255;

	struct Component &operator+=(const Component &rhs) {
		count += rhs.count;
		colorSum += rhs.colorSum;
		
		return *this;
	}

	cv::Vec3f getAverageColor() {
		return colorSum / count;
	}

	unsigned char getLeastDarkVal() {
		return leastDarkVal;
	}

	void setLeastDarkVal(unsigned char darkVal) {
		if (darkVal < leastDarkVal)
			leastDarkVal = darkVal;
	}
};

class ImageGraphSegmentation {
public:
	ImageGraphSegmentation();

	void applySegmentation(cv::Mat &image, int c, int min_size);

private:
	void recolor(cv::Mat &image, DisjointSet &forest);
	void darkChannelUsingDisjointSet(cv::Mat & image, DisjointSet & forest);
};

#endif // ImageGraphSegmentation