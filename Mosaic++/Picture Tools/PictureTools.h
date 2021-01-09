

#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <cmath>
#include <tuple>


#ifdef PICTURETOOLS_EXPORTS
#define PICTURE_TOOLS_API __declspec(dllexport)
#else
#define PICTURE_TOOLS_API __declspec(dllimport)
#endif

class PICTURE_TOOLS_API PictureTools
{
public:
	enum class Algorithm : uint8_t
	{
		NEAREST_NEIGHBOUR,
		BILINEAR_INTERPOLATION
	};

private:
	using Point = std::pair <uint16_t, uint16_t>;

private:
	static const uint8_t defaultSize = 10;

private:
	static uint8_t valueCheck(int number);

private:
	static cv::Mat nearestNeighbour(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0);
	static double interpolation(double firstNeighbour, double secondNeighbour, double proportion);
	static cv::Vec3b interpolation(const cv::Vec3b& firstNeighbour, const cv::Vec3b& secondNeighbour, double proportion);
	static cv::Mat bilinearInterpolation(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0);


public:
	static std::tuple <uint8_t, uint8_t, uint8_t> hueShiftPixel(const uint8_t& B, const uint8_t& G, const uint8_t& R, const uint16_t& angle);
	static cv::Mat cropSquare(const cv::Mat& image, const Point& topL, const Point& botR);

	static cv::Mat resize(const cv::Mat& image, const uint16_t& width = 0, const uint16_t& height = 0, const Algorithm& type = Algorithm::NEAREST_NEIGHBOUR);

	static cv::Scalar averageColorRectangle(const cv::Mat& image, const Point& startLocation = { 0, 0 }, const Point& size = { 0, 0 });
	static cv::Scalar averageColorTriangle(const cv::Mat& image, const Point& startLocation, const Point& size, const uint8_t& type);

	static cv::Mat hueShiftImage(const cv::Mat& image, const uint16_t& angle);
};

