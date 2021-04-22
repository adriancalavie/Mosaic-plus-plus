#pragma once
#include <vector>
#include "../Picture Tools/PictureTools.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>


class QuadTreeImages
{
	using pt = PictureTools;

public:
	class QNode
	{
	public:

		int x0;
		int y0;
		int width;
		int height;
		pt::Point first;
		pt::Point second;
		cv::Scalar mean;
		std::vector<QNode*> children;
		cv::Mat points;
	public:
		QNode(int x0, int y0, int width, int height);


	public:

		int getWidth();
		int getHeigth();

		cv::Mat getPoints();
		cv::Mat getPoints(const cv::Mat&);
	public:
		double getError(const cv::Mat&);
	};

private:
	double threshold;
	int minSize;
	int minPixelSize;
	cv::Mat image;
	QNode* root;

	void recursiveSubDivide(QNode* node, double k, int minPixelSize, const cv::Mat& img);
	std::vector<QNode*> findChildren(QNode*);
public:
	void graphTree();

public:
	QuadTreeImages(double stdThreshold, int minPixelSize, const cv::Mat& image);
	void subdivide();
	cv::Mat concat_images(const cv::Mat& img1, const cv::Mat& img2, int boarder = 5, const cv::Scalar& color = (255, 255, 255));
};

