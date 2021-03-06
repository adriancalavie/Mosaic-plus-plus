#pragma once

#include <random>
#include <optional>
#include "BasePictures.h"
#include "../Picture Tools/PictureTools.h"
#include "QuadTreeImages.h"

enum class Method {
	CROPPING,
	RESIZING
};

enum class Type {
	SQUARE,
	TRIANGLE,
	DIAMOND,
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

public:
	using Point = std::pair <uint16_t, uint16_t>;
	using pt = PictureTools;
	using bp = BasePictures;
	using imgPair = std::pair<std::optional<cv::Mat>, std::optional<cv::Mat>>;
public:
	friend uint32_t EuclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	friend uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor);
	static cv::Mat MakeMosaic(const cv::Mat& image,
		const bp& basePictures,
		const Method& method,
		const Type& type,
		const uint8_t& partitionSize,
		const Algorithm& algorithm,
		const bool& blending);
	static imgPair MakeQuadTree(const bp& dataPictures,
		const cv::Mat& image,
		const bool& blending,
		const double& treshold,
		const int& minSize,
		const bool& hasDetails);

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
	static cv::Mat MakeRectangle(const bp::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static cv::Mat MakeTriangle(const bp::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static cv::Mat MakeDiamond(const bp::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static void MakeMargins(cv::Mat& result, const bp::map& dataPictures,
		const cv::Mat& image,
		const Algorithm& algorithm,
		const bool& blending,
		const uint8_t& partitionSize);

	static std::vector<QuadTreeImages::QNode*> FindChildren(QuadTreeImages::QNode*);

	static imgPair graphTree(const bp& database, const QuadTreeImages* qt, const int& thickness = -1, const bool& hasDetails = false, const bool& blending = false);

public:
	static cv::Mat FindPictureWithColorMed(const bp::map& dataPictures,
		const cv::Scalar& mediumColor,
		std::string& pictureDifferent,
		const Algorithm& algorithm);
	static cv::Mat Concat4(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& img3, const cv::Mat& img4);
};



