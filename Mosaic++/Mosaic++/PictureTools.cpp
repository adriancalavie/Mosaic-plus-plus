#include "PictureTools.h"
#include <iostream>
#include <unordered_map>



Mat PictureTools::crop(const Mat& image, Point topL, Point botR)
{
	uint16_t height = botR.first - topL.first;
	uint16_t width = botR.second - topL.second;
	Mat result(height, width, CV_8UC3);
	for (int index_rows = 0; index_rows < height; index_rows++)
		for (int index_cols = 0; index_cols < width; index_cols++)
		{
			result.at<Vec3b>(index_rows, index_cols)[0] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[0];
			result.at<Vec3b>(index_rows, index_cols)[1] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[1];
			result.at<Vec3b>(index_rows, index_cols)[2] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[2];
		}
	return result;
}

Mat PictureTools::resize(const Mat& image, const uint16_t& width, const uint16_t& height)
{
	assert(!image.empty());
	Mat newimage(width, height, CV_8UC3);

	int x_ratio = (int)((image.cols << 16) / width) + 1;
	int y_ratio = (int)((image.rows << 16) / height) + 1;

	int nearX, nearY;
	for (int rows = 0; rows < height; ++rows) {
		for (int cols = 0; cols < width; ++cols) {
			nearX = ((cols * x_ratio) >> 16);
			nearY = ((rows * y_ratio) >> 16);

			newimage.at<cv::Vec3b>(rows, cols)[0] = image.at<cv::Vec3b>(nearY, nearX)[0];
			newimage.at<cv::Vec3b>(rows, cols)[1] = image.at<cv::Vec3b>(nearY, nearX)[1];
			newimage.at<cv::Vec3b>(rows, cols)[2] = image.at<cv::Vec3b>(nearY, nearX)[2];
		}
	}
	return newimage;
}

Scalar PictureTools::averageColor(const Mat& image)
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

	return Scalar(blue, green, red);
}

Scalar PictureTools::averageColor(const Mat& image, std::pair<int, int>startLocation, std::pair<int, int> size)
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

	return Scalar(blue, green, red);
}



