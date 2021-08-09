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

int QuadTreeImages::QNode::GetWidth() const
{
	return this->width;
}

int QuadTreeImages::QNode::GetHeigth() const
{
	return this->height;
}

cv::Mat QuadTreeImages::QNode::GetPoints() const
{
	return this->points;
}

cv::Mat QuadTreeImages::QNode::GetPoints(const cv::Mat& image) const
{
	const cv::Rect cropped(y0, x0, width, height);

	return image(cropped);
}

double QuadTreeImages::QNode::GetError(const cv::Mat& image)
{
	const auto pixels = GetPoints(image);

	cv::Mat3i temp = pixels;

	const cv::Scalar avg = cv::mean(temp);
	mean = avg;
	temp = temp - avg;

	cv::pow(temp, 2, temp);
	cv::Scalar mse = cv::mean(temp);

	const double e = mse[0] * 0.2989 + mse[1] * 0.5870 + mse[2] * 0.1140;
	return (e * image.cols * image.rows) / 90000000;
}

void QuadTreeImages::RecursiveSubDivide(QNode* node, double k, int m_minPixelSize, const cv::Mat& img) const
{
	if (node->GetError(img) <= k)
	{
		return;
	}
	const int w_1 = static_cast<int>(std::floor(node->width / 2.0));
	const int w_2 = static_cast<int>(std::ceil(node->width / 2.0));
	const int h_1 = static_cast<int>(std::floor(node->height / 2.0));
	const int h_2 = static_cast<int>(std::ceil(node->height / 2.0));

	if (w_1 <= m_minPixelSize || h_1 <= m_minPixelSize)
	{
		return;
	}

	QNode* x1 = new QNode(node->x0, node->y0, w_1, h_1); //top left
	RecursiveSubDivide(x1, k, m_minPixelSize, img);

	QNode* x2 = new QNode(node->x0 + h_1, node->y0, w_1, h_2); //btm left
	RecursiveSubDivide(x2, k, m_minPixelSize, img);

	QNode* x3 = new QNode(node->x0, node->y0 + w_1, w_2, h_1); //top right
	RecursiveSubDivide(x3, k, m_minPixelSize, img);

	QNode* x4 = new QNode(node->x0 + h_1, node->y0 + w_1, w_2, h_2); //btm right
	RecursiveSubDivide(x4, k, m_minPixelSize, img);

	node->children = std::vector<QNode*>();

	node->children.push_back(x1);
	node->children.push_back(x2);
	node->children.push_back(x3);
	node->children.push_back(x4);

}

QuadTreeImages::QuadTreeImages(double m_threshold, int m_minPixelSize, const cv::Mat& image)
{
	this->m_threshold = m_threshold;
	this->m_minSize = m_minPixelSize;
	this->m_minPixelSize = m_minPixelSize;
	this->m_image = image;
	this->m_root = new QNode(0, 0, image.cols, image.rows);
}

void QuadTreeImages::recursiveDeletition(QNode* root)
{
	if (root->children.empty())
	{
		delete root;
		return;
	}

	for (auto child : root->children)
	{
		recursiveDeletition(child);
	}
}

QuadTreeImages::~QuadTreeImages()
{
	recursiveDeletition(m_root);
}

void QuadTreeImages::Subdivide() const
{
	RecursiveSubDivide(m_root, m_threshold, m_minPixelSize, m_image);
}

double QuadTreeImages::GetThreshold() const
{
	return this->m_threshold;
}

void QuadTreeImages::SetThreshold(const double threshold)
{
	this->m_threshold = threshold;
}

int QuadTreeImages::GetMinSize() const
{
	return this->m_minSize;
}

void QuadTreeImages::SetMinSize(const int minSize)
{
	this->m_minSize = minSize;
}

int QuadTreeImages::GetMinPixelSize() const
{
	return this->m_minPixelSize;
}

void QuadTreeImages::SetMinPixelSize(const int minPixelSize)
{
	this->m_minPixelSize = minPixelSize;
}

cv::Mat QuadTreeImages::GetImage() const
{
	return this->m_image;
}

void QuadTreeImages::SetImage(const cv::Mat& image)
{
	this->m_image = image;
}

QuadTreeImages::QNode* QuadTreeImages::GetRoot() const
{
	return this->m_root;
}
