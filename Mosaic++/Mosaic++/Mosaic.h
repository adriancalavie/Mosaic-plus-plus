#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include "BasePictures.h"

enum class Type { square, rectangle, triangle, diamond };

class Mosaic
{
	

private:
	using Point = std::pair <uint16_t, uint16_t>;
	static const uint8_t defaultSize = 10;

public:
	static void alphaBlending(cv::Mat&, const cv::Scalar&);
	friend uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat makeMosaic(const cv::Mat& image,const BasePictures& basePictures, const Type& type= Type::square,const uint8_t& partitionSize=defaultSize, bool blending = false);


public:
	static void replaceCellRectangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL); // for square and rectangle

	static void replaceCellTriangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL, const uint8_t& type, const Point & = std::make_pair((uint16_t)0, (uint16_t)0), const Point & = std::make_pair(defaultSize, defaultSize));

	static void replaceCellDiamond(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& top);

private:
	static cv::Mat makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize = defaultSize);
	static cv::Mat makeTriangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize = defaultSize);
	static cv::Mat makeDiamond(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize = defaultSize);

};

