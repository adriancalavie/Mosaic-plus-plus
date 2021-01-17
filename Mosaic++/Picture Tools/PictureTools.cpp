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

cv::Mat PictureTools::cropSquare(const cv::Mat& image, const Point& topL, const Point& botR)
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

cv::Mat PictureTools::resize(const cv::Mat& image, const uint16_t& width, const uint16_t& height, const PictureTools::Algorithm& type)
{
	switch (type)
	{
	case PictureTools::Algorithm::NEAREST_NEIGHBOUR:
		return PictureTools::nearestNeighbour(image, width, height);

	case PictureTools::Algorithm::BILINEAR_INTERPOLATION:
		return PictureTools::bilinearInterpolation(image, width, height);

	default:
		return cv::Mat();
	}
}



cv::Mat PictureTools::nearestNeighbour(const cv::Mat& image, const uint16_t& width, const uint16_t& height)
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
		}
	}

	return newimage;
}

double PictureTools::interpolation(double firstNeighbour, double secondNeighbour, double proportion)
{
	return (secondNeighbour - firstNeighbour) * proportion + firstNeighbour;
}

cv::Vec3b PictureTools::interpolation(const cv::Vec3b& firstNeighbour, const cv::Vec3b& secondNeighbour, double proportion)
{
	return cv::Vec3b(
		static_cast<uchar>(interpolation(firstNeighbour[0], secondNeighbour[0], proportion)),
		static_cast<uchar>(interpolation(firstNeighbour[1], secondNeighbour[1], proportion)),
		static_cast<uchar>(interpolation(firstNeighbour[2], secondNeighbour[2], proportion))
	);
}

cv::Mat PictureTools::bilinearInterpolation(const cv::Mat& image, const uint16_t& width, const uint16_t& height)
{

	cv::Mat newImage(height, width, CV_8UC3);

	double heightRatio = height / static_cast<double>(image.rows);
	double widthRatio = width / static_cast<double>(image.cols);

	for (int hIndex = 0; hIndex < height; ++hIndex)
	{
		for (int wIndex = 0; wIndex < width; ++wIndex)
		{
			std::array <cv::Point, 2> neighbours;
			double h_t, w_t;
			cv::Vec3b valueW1, valueW2, valueH;

			h_t = hIndex / heightRatio;
			w_t = wIndex / widthRatio;

			neighbours[0].x = static_cast<int>(h_t);
			neighbours[0].y = static_cast<int>(w_t);
			neighbours[1].x = neighbours[0].x + 1;
			neighbours[1].y = neighbours[0].y + 1;

			if (neighbours[1].x < image.rows && neighbours[1].y < image.cols)
			{
				valueW1 = interpolation(
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y),
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[1].y),
					h_t - neighbours[0].x
				);

				valueW2 = interpolation(
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y),
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[1].y),
					h_t - neighbours[0].x
				);

				valueH = interpolation(
					valueW1,
					valueW2,
					w_t - neighbours[0].y
				);

				newImage.at<cv::Vec3b>(hIndex, wIndex) = std::move(valueH);
			}
			else
			{
				newImage.at<cv::Vec3b>(hIndex, wIndex) = image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y);
			}

		}
	}

	return newImage;
}


cv::Scalar PictureTools::averageColorRectangle(const cv::Mat& image, const Point& startLocation, const Point& size)
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

cv::Scalar PictureTools::averageColorTriangle(const cv::Mat& image, const Point& startLocation, const Point& size, const uint8_t& type)
{
	assert(!image.empty());
	double blue, green, red;
	blue = green = red = 0;
	int sizeOfPartition = (size.first) * (size.second) / 2;
	int rowSize = size.first + startLocation.first;
	int colSize = size.second + startLocation.second;

	switch (type)
	{
	case 1:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = 0; cols < size.second - rows; ++cols)
			{
				blue += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0] / (double)100;
				green += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1] / (double)100;
				red += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2] / (double)100;
			}
		}

		break;
	case 2:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = size.second - rows; cols < size.second; ++cols)
			{
				blue += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0] / (double)100;
				green += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1] / (double)100;
				red += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2] / (double)100;
			}
		}

		break;
	case 3:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = rows; cols < size.second; ++cols)
			{
				blue += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0] / (double)100;
				green += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1] / (double)100;
				red += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2] / (double)100;
			}
		}

		break;
	case 4:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = 0; cols < rows; ++cols)
			{
				blue += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0] / (double)100;
				green += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1] / (double)100;
				red += (int)image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2] / (double)100;
			}
		}
		break;
	default:
		break;
	}

	red = red / sizeOfPartition;
	green = green / sizeOfPartition;
	blue = blue / sizeOfPartition;
	red = red * 100;
	green = green * 100;
	blue = blue * 100;

	return cv::Scalar(blue, green, red);
}



