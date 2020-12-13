#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include "BasePictures.h"


class Mosaic
{
//private: 
	//enum class Type { square, rectangle, triangle, diamond };
private:
	using Point = std::pair <uint16_t, uint16_t>;
private:
	static const uint8_t defaultSize = 10;
	static const uint8_t defaultShape = 1;

public:
	static void alphaBlending(cv::Mat&, const cv::Scalar&);
	friend uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize = defaultSize, const uint8_t& shape = defaultShape,bool blending = false);

	
public:
	static void replaceCellRectangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL); // for square and rectangle

	static void replaceCellTriangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL, const int& type);
	static void replaceCellDiamond(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& top, const Point& low, const Point& left, const Point& right);

private:
	static cv::Mat makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize = defaultSize);
	static cv::Mat makeTriangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize = defaultSize);

};

