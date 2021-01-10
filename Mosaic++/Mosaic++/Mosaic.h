#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include "BasePictures.h"
#include "Data.h"

enum class Method {
	CROPPING,
	RESIZING
};

enum class Type {
	SQUARE,
	RECTANGLE,
	TRIANGLE,
	DIAMOND
};

class Mosaic
{
private:
	using Point = std::pair <uint16_t, uint16_t>;
	static const uint8_t defaultSize = 10;

public:
	static void alphaBlending(cv::Mat&, const cv::Scalar&);
	friend uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	friend uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat makeMosaic(const cv::Mat& image,
		const BasePictures& basePictures,
		const Method& method,
		const Type& type = Type::SQUARE,
		const uint8_t& partitionSize = defaultSize,
		bool blending = false);

public:
	static void replaceCellRectangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL); // for square and rectangle

	static void replaceCellTriangle(cv::Mat& originalPicture,
		cv::Mat&& mosaicPhoto,
		const Point& topL,
		const uint8_t& type,
		const Point & = std::make_pair((uint16_t)0, (uint16_t)0),
		const Point & = std::make_pair(defaultSize, defaultSize));

	static void replaceCellDiamond(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& top);

public:
	static uint8_t getDefaultSize();

private:
	static cv::Mat makeRectangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static cv::Mat makeTriangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static cv::Mat makeDiamond(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static void MakeMargins(cv::Mat& result, std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);
private:
	static cv::Mat findPictureWithColorMed(const std::unordered_map<cv::Scalar,std::string>& dataPictures,
		const cv::Scalar& mediumColor,
		 std::string& pictureDifferent);
};

