#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>

using namespace cv;

class PictureTools
{
public:
	Mat crop(Mat image, std::pair <uint8_t, uint8_t> topL, std::pair <uint8_t, uint8_t> botR);
	static Mat resize(const Mat& image, int width = 0, int height = 0);
	static Scalar averageColor(const Mat& image);
};

