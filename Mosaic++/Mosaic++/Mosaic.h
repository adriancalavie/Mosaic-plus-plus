#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
using namespace cv;



class Mosaic
{
private:
	static const uint8_t defaultSize = 10;

public:
	friend int euclideanDistance(const Scalar& firstColor, const Scalar& secondColor);
	static Mat makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, Mat& image, const uint8_t& partitionSize = defaultSize);
	static void replaceCell(Mat& originalPicture, const Mat& mosaicPhoto, const std::pair<int, int>& topL);
};

