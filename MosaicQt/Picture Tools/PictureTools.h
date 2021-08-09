#pragma once
#include <opencv2/imgproc.hpp>


#ifdef PICTURETOOLS_EXPORTS
#define PICTURE_TOOLS_API __declspec(dllexport)
#else
#define PICTURE_TOOLS_API __declspec(dllimport)
#endif

class PICTURE_TOOLS_API PictureTools
{
public:
	enum class Algorithm : int_fast8_t
	{
		NEAREST_NEIGHBOUR,
		BILINEAR_INTERPOLATION
	};

public:
	using Point = std::pair <uint16_t, uint16_t>;

private:
	static uint8_t ValueCheck(int number);

private:
	static cv::Mat NearestNeighbour(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0);
	static double Interpolation(double firstNeighbour, double secondNeighbour, double proportion);
	static cv::Vec3b Interpolation(const cv::Vec3b& firstNeighbour, const cv::Vec3b& secondNeighbour, double proportion);
	static cv::Mat BilinearInterpolation(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0);


public:
	
	static cv::Mat CropSquare(const cv::Mat& image, const Point& topL, const Point& botR);

	static cv::Mat Resize(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0, const Algorithm& type = Algorithm::NEAREST_NEIGHBOUR);

	static cv::Scalar AverageColorRectangle(const cv::Mat& image, const Point& startLocation = { 0, 0 }, const Point& size = { 0, 0 });
	static cv::Scalar AverageColorTriangle(const cv::Mat& image, const Point& startLocation, const Point& size, const uint8_t& type);

	static void AlphaBlending(cv::Mat&, const cv::Scalar&);
};

