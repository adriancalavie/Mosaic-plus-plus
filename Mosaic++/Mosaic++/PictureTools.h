#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>

using namespace cv;




class PictureTools
{
private:
	using Point = std::pair <uint16_t, uint16_t>;

private:
	static const uint8_t defaultSize = 10;

public:
	static Mat crop(const Mat& image, Point topL, Point botR);
	static Mat resize(const Mat& image, const uint16_t& width = 0, const uint16_t& height = 0);
	static Scalar averageColor(const Mat& image);
	static Scalar averageColor(const Mat& image,std::pair<int,int>startLocation, std::pair<int, int> size);
};

