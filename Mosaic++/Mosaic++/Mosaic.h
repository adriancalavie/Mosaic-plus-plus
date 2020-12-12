#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include "BasePictures.h"

class Mosaic
{
private:
	using Point = std::pair <uint16_t, uint16_t>;
private:
	static const uint8_t defaultSize = 10;
	static const uint8_t defaultShape = 1;
	static void alphaBlending(const std::unordered_map < cv::Scalar, std::string>& dataPictures);

public:
	friend int32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize = defaultSize, const uint8_t& shape = defaultShape);
	static cv::Mat makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize = defaultSize);
	static void replaceCell(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL);
};

