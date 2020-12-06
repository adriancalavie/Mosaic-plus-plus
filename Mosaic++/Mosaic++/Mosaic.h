#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include "BasePictures.h"
using namespace cv;

class Mosaic
{
private:
	static const uint8_t defaultSize = 10;
	static const uint8_t defaultShape = 1;
	static void alphaBlending(const std::unordered_map < cv::Scalar, std::string>& dataPictures);

public:
	friend int euclideanDistance(const Scalar& firstColor, const Scalar& secondColor);
	static Mat makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, Mat& image, const uint8_t& partitionSize = defaultSize, const uint8_t& shape = defaultShape);
	static Mat makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, Mat& image, const uint8_t& partitionSize = defaultSize);
	static void replaceCell(Mat& originalPicture, const Mat& mosaicPhoto, const std::pair<int, int>& topL);
};

