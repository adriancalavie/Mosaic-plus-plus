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
	TRIANGLE,
	DIAMOND
};


#ifdef MOSAIC_EXPORTS
#define MOSAIC_API __declspec(dllexport)
#else
#define MOSAIC_API __declspec(dllimport)
#endif

class MOSAIC_API Mosaic
{
private:
	using Point = std::pair <uint16_t, uint16_t>;
	using pt = PictureTools;

	static const uint8_t defaultSize = 10;

public:
	static void AlphaBlending(cv::Mat&, const cv::Scalar&);
	friend uint32_t EuclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	friend uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat MakeMosaic(const cv::Mat& image,
		const BasePictures& basePictures,
		const Method& method,
		const Type& type = Type::SQUARE,
		const uint8_t& partitionSize = defaultSize,
		bool blending = false);

public:
	static void ReplaceCellRectangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL); // for square and rectangle

	static void ReplaceCellTriangle(cv::Mat& originalPicture,
		cv::Mat&& mosaicPhoto,
		const Point& topL,
		const uint8_t& type,
		const Point & = { (uint16_t)0, (uint16_t)0 },
		const Point & = { defaultSize, defaultSize });

	static void ReplaceCellDiamond(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& top);

public:
	static uint8_t GetDefaultSize();

private:
	static cv::Mat MakeRectangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static cv::Mat MakeTriangle(const std::unordered_map<cv::Scalar,
		std::string>& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static cv::Mat MakeDiamond(const BasePictures::map& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);

	static void MakeMargins(cv::Mat& result, const BasePictures::map& dataPictures,
		const cv::Mat& image, bool blending,
		const uint8_t& partitionSize = defaultSize);
private:
	static cv::Mat FindPictureWithColorMed(const BasePictures::map& dataPictures,
		const cv::Scalar& mediumColor,
		std::string& pictureDifferent);
};

