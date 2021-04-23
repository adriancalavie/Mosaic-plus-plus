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

		int GetWidth();
		int GetHeigth();

		cv::Mat GetPoints();
		cv::Mat GetPoints(const cv::Mat&);
	public:
		double GetError(const cv::Mat&);
	};

private:
	double m_threshold;
	int m_minSize;
	int m_minPixelSize;
	cv::Mat m_image;
	QNode* m_root;

	void RecursiveSubDivide(QNode* node, double k, int m_minPixelSize, const cv::Mat& img);

public:
	QuadTreeImages(double stdm_threshold, int m_minPixelSize, const cv::Mat& image);
	void Subdivide();

public:
	double GetThreshold() const;
	void SetThreshold(const double& treshold);


	int	GetMinSize() const;
	void SetMinSize(const int& minSize);

	int	GetMinPixelSize()const;
	void SetMinPixelSize(const int& minPixelSize);


	cv::Mat	GetImage() const;
	void SetImage(const cv::Mat& image);

	QNode* GetRoot() const;

};

