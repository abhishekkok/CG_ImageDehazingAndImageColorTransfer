#include <opencv2/opencv.hpp>
#include "graph.h"

float diff(const cv::Mat &image, int x1, int y1, int x2, int y2) {
	cv::Vec3f pix1 = image.at<cv::Vec3f>(y1, x1);
	cv::Vec3f pix2 = image.at<cv::Vec3f>(y2, x2);

	return sqrt((pix1 - pix2).dot((pix1 - pix2)));
}

std::vector <Edge> constructGraph(const cv::Mat &image) {
	int height = image.rows;
	int width = image.cols;

	int size = 4 * height * width;
	std::vector <Edge> edges(static_cast<unsigned long>(size));

	int num = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int current = width * y + x;
			if (x > 0) {
				edges[num].a = current;
				edges[num].b = width * y + (x - 1);
				edges[num].weight = diff(image, x, y, x - 1, y);
				num++;
			}

			if (y > 0) {
				edges[num].a = current;
				edges[num].b = width * (y - 1) + x;
				edges[num].weight = diff(image, x, y, x, y - 1);
				num++;
			}

			if (x > 0 && y > 0) {
				edges[num].a = current;
				edges[num].b = width * (y - 1) + (x - 1);
				edges[num].weight = diff(image, x, y, x - 1, y - 1);
				num++;
			}

			if (x > 0 && y < height - 1) {
				edges[num].a = current;
				edges[num].b = width * (y + 1) + (x - 1);
				edges[num].weight = diff(image, x, y, x - 1, y + 1);
				num++;
			}
		}
	}

	edges.resize(static_cast<unsigned long>(num));

	return edges;
}

DisjointSet segmentIntoSubGraph(int num_vertices, std::vector <Edge> &edges, float c) {
	/* sort(edges.begin(), edges.end(), [](Edge &a, Edge &b) {
		return a.weight < b.weight;
	}); */

	sort(edges.begin(), edges.end());

	DisjointSet graphForest = DisjointSet(edges.size());
	std::vector<float> thresholds(static_cast<unsigned long>(num_vertices), c);

	for (Edge &edge : edges) {
		int a = graphForest.find(edge.a);
		int b = graphForest.find(edge.b);

		if (a != b && edge.weight <= thresholds[a] && edge.weight <= thresholds[b]) {
			graphForest.merge(a, b);
			a = graphForest.find(a);
			thresholds[a] = edge.weight + c / graphForest.size(a);
		}
	}

	return graphForest;
}