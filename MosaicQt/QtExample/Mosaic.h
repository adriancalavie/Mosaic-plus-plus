#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <time.h> 
#include <atomic>
#include <qprogressbar.h>
#include "BasePictures.h"
#include "Data.h"
#include "../../Mosaic++/Picture Tools/PictureTools.h"

enum class Method {
	CROPPING,
	RESIZING
};

enum class Type {
	SQUARE,
	TRIANGLE,
	DIAMOND
};

enum class Algorithm {
	EUCLIDEAN,
	RIEMERSMA
};

static int count = 0;
//std::atomic_uint8_t progress = 0;

class Mosaic
{
public:

private:
	using Point = std::pair <uint16_t, uint16_t>;
	using pt = PictureTools;

public:
	friend uint32_t EuclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	friend uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat MakeMosaic(const cv::Mat& image,
		const BasePictures& basePictures,
		const Method& method,
		const Type& type,
		const uint8_t& partitionSize,
		const Algorithm& algorithm ,
		const bool& blending);

private:
	static void ReplaceCellRectangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL); // for square and rectangle

	static void ReplaceCellTriangle(cv::Mat& originalPicture,
		cv::Mat&& mosaicPhoto,
		const Point& topL,
		const uint8_t& type,
		const Point & = { (uint16_t)0, (uint16_t)0 },
		const Point & = { (uint16_t)0, (uint16_t)0 });

	static void ReplaceCellDiamond(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& top);

private:
	static cv::Mat MakeRectangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static cv::Mat MakeTriangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static cv::Mat MakeDiamond(const BasePictures::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static void MakeMargins(cv::Mat& result, const BasePictures::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);
private:
	static cv::Mat FindPictureWithColorMed(const BasePictures::map& dataPictures,
		const cv::Scalar& mediumColor,
		std::string& pictureDifferent,
		const Algorithm& algorithm);
};



