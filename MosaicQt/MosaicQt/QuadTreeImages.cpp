#include "QuadTreeImages.h"
#include <iostream>

QuadTreeImages::QNode::QNode(int x0, int y0, int width, int heigth)
{
	this->x0 = x0;
	this->y0 = y0;
	this->width = width;
	this->height = heigth;
	this->children = std::vector<QNode*>();
}

int QuadTreeImages::QNode::getWidth()
{
	return this->width;
}

int QuadTreeImages::QNode::getHeigth()
{
	return this->height;
}

cv::Mat QuadTreeImages::QNode::getPoints()
{
	return this->points;
}

cv::Mat QuadTreeImages::QNode::getPoints(const cv::Mat& image)
{
	//std::cout << x0 << " " << y0 << " " << width << " " << height << std::endl;
	cv::Rect cropped(y0, x0, width, height);

	return image(cropped);
}

double QuadTreeImages::QNode::getError(const cv::Mat& image)
{
	auto pixels = getPoints(image);

	cv::Mat3i temp = pixels;

	cv::Scalar avg = cv::mean(temp);
	mean = avg;
	temp = temp - avg;

	cv::pow(temp, 2, temp);
	cv::Scalar mse = cv::mean(temp);

	double e = mse[0] * 0.2989 + mse[1] * 0.5870 + mse[2] * 0.1140;
	return (e * image.cols * image.rows) / 90000000;
}

void QuadTreeImages::recursiveSubDivide(QNode* node, double k, int minPixelSize, const cv::Mat& img)
{
	double size = node->getError(img);

	//std::cout << "size:" << size << std::endl;
	if (size <= k)
	{
		return;
	}
	int w_1 = (int)(std::floor(node->width / 2));
	int w_2 = (int)(std::ceil(node->width / 2));
	int h_1 = (int)(std::floor(node->height / 2));
	int h_2 = (int)(std::ceil(node->height / 2));
	//std::cout << w_1 << " " << w_2 << " " << h_1 << " " << h_2;

	if (w_1 <= minPixelSize || h_1 <= minPixelSize)
	{
		return;
	}

	QNode* x1 = new QNode(node->x0, node->y0, w_1, h_1); //top left
	recursiveSubDivide(x1, k, minPixelSize, img);

	QNode* x2 = new QNode(node->x0, node->y0 + w_1, w_1, h_2); //btm left
	recursiveSubDivide(x2, k, minPixelSize, img);

	QNode* x3 = new QNode(node->x0 + h_1, node->y0, w_2, h_1); //top right
	recursiveSubDivide(x3, k, minPixelSize, img);

	QNode* x4 = new QNode(node->x0 + h_1, node->y0 + w_1, w_2, h_2); //btm right
	recursiveSubDivide(x4, k, minPixelSize, img);

	node->children = std::vector<QNode*>();

	node->children.push_back(x1);
	node->children.push_back(x2);
	node->children.push_back(x3);
	node->children.push_back(x4);

}

std::vector<QuadTreeImages::QNode*> QuadTreeImages::findChildren(QNode* node)
{
	if (!node->children.size())
	{
		std::vector<QNode*> result;
		result.push_back(node);
		return result;
	}
	else
	{
		auto children = std::vector<QNode*>();
		for (auto& child : node->children)
		{
			auto foundChildren = findChildren(child);

			children.reserve(children.size() + foundChildren.size());
			children.insert(children.end(), foundChildren.begin(), foundChildren.end());
		}
		return children;
	}
}

QuadTreeImages::QuadTreeImages(double stdThreshold, int minPixelSize, const cv::Mat& image)
{
	this->threshold = stdThreshold;
	this->minSize = minPixelSize;
	this->minPixelSize = minPixelSize;
	this->image = image;
	this->root = new QNode(0, 0, image.cols, image.rows);
}

void QuadTreeImages::subdivide()
{
	recursiveSubDivide(root, threshold, minPixelSize, image);
}

cv::Mat QuadTreeImages::concat_images(const cv::Mat& img1, const cv::Mat& img2, int boarder, const cv::Scalar& color)
{
	cv::Mat borderImg1 = img1;
	cv::Mat borderImg2 = img2;

	cv::copyMakeBorder(img1, borderImg1, boarder, boarder, boarder, boarder, cv::BORDER_CONSTANT, color);
	cv::copyMakeBorder(img2, borderImg2, boarder, boarder, 0, boarder, cv::BORDER_CONSTANT, color);


	// Get dimension of final image
	int rows = cv::max(img1.rows, img2.rows);
	int cols = img1.cols + img2.cols;

	// Create a black image
	cv::Mat3b res(rows, cols, cv::Vec3b(0, 0, 0));

	// Copy images in correct position
	img1.copyTo(res(cv::Rect(0, 0, img1.cols, img1.rows)));
	img2.copyTo(res(cv::Rect(img1.cols, 0, img2.cols, img2.rows)));

	return res;
}



void QuadTreeImages::graphTree()
{
	cv::Mat quadImage = this->image;

	auto c = findChildren(this->root);

	std::cout << c.size();
	for (auto& n : c)
	{
		//cv::Point pt1(this->root->x0, this->root->y0);

		//cv::Point pt2(this->root->x0 + this->root->width, this->root->y0 + this->root->height);

		cv::Rect part(n->y0, n->x0, n->width + n->y0, n->height + n->x0);

		cv::rectangle(quadImage, part, n->mean, -1);

	}

	cv::imshow("QuadImage", quadImage);
	cv::waitKey(0);
}