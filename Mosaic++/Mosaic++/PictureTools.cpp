#include "PictureTools.h"
#include <iostream>
#include <unordered_map>


uint8_t PictureTools::valueCheck(int number)
{
	if (number < 0)
		return 0;
	if (number > 255)
		return 255;
	return number;
}

std::tuple<uint8_t, uint8_t, uint8_t> PictureTools::hueShiftPixel(const uint8_t& R, const uint8_t& G, const uint8_t& B, const uint16_t& angle)
{
	uint8_t resR, resG, resB;
	const double pi = atan(1) * 4;
	double x = angle * pi / 180;
	double xCos = cos(x);
	double xSin = sin(x);
	double calculationMatrix[3][3] = { 1,0,0,0,1,0,0,0,1 };
	calculationMatrix[0][0] = xCos + (1.0 - xCos) / 3.0;
	calculationMatrix[0][1] = 1. / 3. * (1.0 - xCos) - sqrt(1. / 3.) * xSin;
	calculationMatrix[0][2] = 1. / 3. * (1.0 - xCos) + sqrt(1. / 3.) * xSin;
	calculationMatrix[1][0] = 1. / 3. * (1.0 - xCos) + sqrt(1. / 3.) * xSin;
	calculationMatrix[1][1] = xCos + 1. / 3. * (1.0 - xCos);
	calculationMatrix[1][2] = 1. / 3. * (1.0 - xCos) - sqrt(1. / 3.) * xSin;
	calculationMatrix[2][0] = 1. / 3. * (1.0 - xCos) - sqrt(1. / 3.) * xSin;
	calculationMatrix[2][1] = 1. / 3. * (1.0 - xCos) + sqrt(1. / 3.) * xSin;
	calculationMatrix[2][2] = xCos + 1. / 3. * (1.0 - xCos);
	resR = R * calculationMatrix[0][0] + G * calculationMatrix[0][1] + B * calculationMatrix[0][2];
	resG = R * calculationMatrix[1][0] + G * calculationMatrix[1][1] + B * calculationMatrix[1][2];
	resB = R * calculationMatrix[2][0] + G * calculationMatrix[2][1] + B * calculationMatrix[2][2];
	return std::make_tuple(valueCheck(resR), valueCheck(resG), valueCheck(resB));
}

cv::Mat PictureTools::crop(const cv::Mat& image, const Point& topL, const Point& botR)
{
	uint16_t height = botR.first - topL.first;
	uint16_t width = botR.second - topL.second;
	cv::Mat result(height, width, CV_8UC3);
	for (int index_rows = 0; index_rows < height; index_rows++)
		for (int index_cols = 0; index_cols < width; index_cols++)
		{
			result.at<cv::Vec3b>(index_rows, index_cols)[0] = image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[0];
			result.at<cv::Vec3b>(index_rows, index_cols)[1] = image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[1];
			result.at<cv::Vec3b>(index_rows, index_cols)[2] = image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[2];
		}
	return result;
}

cv::Mat PictureTools::resize(const cv::Mat& image, const uint16_t& width, const uint16_t& height)
{
	assert(!image.empty());
	cv::Mat newimage(height, width, CV_8UC3);

	int x_ratio = (int)((image.cols << 16) / width) + 1;
	int y_ratio = (int)((image.rows << 16) / height) + 1;

	int nearX, nearY;
	for (int rows = 0; rows < height; ++rows) {
		for (int cols = 0; cols < width; ++cols) {
			nearX = ((cols * x_ratio) >> 16);
			nearY = ((rows * y_ratio) >> 16);

			newimage.at<cv::Vec3b>(rows, cols) = cv::Vec3b(image.at<cv::Vec3b>(nearY, nearX));

			//std::cout << (int)newimage.at<cv::Vec3b>(rows,cols)[0] <<" "
				//<<(int) newimage.at<cv::Vec3b>(rows,cols)[1] <<" "<<
			//(int) newimage.at<cv::Vec3b>(rows,cols)[2] << std::endl;
		}
	}

	return newimage;
}

cv::Scalar PictureTools::averageColor(const cv::Mat& image)
{
	assert(!image.empty());
	double blue, green, red;
	blue = green = red = 0;
	int size = (image.rows) * (image.cols);
	for (int rows = 0; rows < image.rows; ++rows)
	{
		for (int cols = 0; cols < image.cols; ++cols)
		{
			blue += (int)image.at<cv::Vec3b>(rows, cols)[0] / (double)100;
			green += (int)image.at<cv::Vec3b>(rows, cols)[1] / (double)100;
			red += (int)image.at<cv::Vec3b>(rows, cols)[2] / (double)100;

		}
	}
	red = red / size;
	green = green / size;
	blue = blue / size;
	red = red * 100;
	green = green * 100;
	blue = blue * 100;

	return cv::Scalar(blue, green, red);
}

cv::Scalar PictureTools::averageColor(const cv::Mat& image, const Point& startLocation, const Point& size)
{
	assert(!image.empty());
	double blue, green, red;
	blue = green = red = 0;
	int sizeOfPartition = (size.first) * (size.second);
	int rowSize = size.first + startLocation.first;
	int colSize = size.second + startLocation.second;
	for (int rows = startLocation.first; rows < rowSize; ++rows)
	{
		for (int cols = startLocation.second; cols < colSize; ++cols)
		{
			blue += (int)image.at<cv::Vec3b>(rows, cols)[0] / (double)100;
			green += (int)image.at<cv::Vec3b>(rows, cols)[1] / (double)100;
			red += (int)image.at<cv::Vec3b>(rows, cols)[2] / (double)100;

		}
	}
	red = red / sizeOfPartition;
	green = green / sizeOfPartition;
	blue = blue / sizeOfPartition;
	red = red * 100;
	green = green * 100;
	blue = blue * 100;

	return cv::Scalar(blue, green, red);
}

cv::Mat PictureTools::hueShiftImage(const cv::Mat& image, const uint16_t& angle)
{
	std::tuple <uint8_t, uint8_t, uint8_t> newColor;
	cv::Mat result(image.rows, image.cols, CV_8UC3);
	for (int rows = 0; rows < image.rows; ++rows)
		for (int cols = 0; cols < image.cols; ++cols)
		{
			newColor = hueShiftPixel(image.at<cv::Vec3b>(rows, cols)[2], image.at<cv::Vec3b>(rows, cols)[1], image.at<cv::Vec3b>(rows, cols)[0], angle);
			result.at<cv::Vec3b>(rows, cols)[0] = std::get<2>(newColor);
			result.at<cv::Vec3b>(rows, cols)[1] = std::get<1>(newColor);
			result.at<cv::Vec3b>(rows, cols)[2] = std::get<0>(newColor);
		}
	return result;
}



