#include "PictureTools.h"
#include <iostream>
#include <unordered_map>

uint8_t PictureTools::ValueCheck(int number)
{
	if (number < 0)
		return 0;
	if (number > 255)
		return 255;
	return number;
}

cv::Mat PictureTools::CropSquare(const cv::Mat& image, const Point& topL, const Point& botR)
{
	const uint16_t height = botR.first - topL.first;
	const uint16_t width = botR.second - topL.second;
	cv::Mat result(height, width, CV_8UC3);
	for (int index_rows = 0; index_rows < height; index_rows++)
		for (int index_cols = 0; index_cols < width; index_cols++)
		{
			result.at<cv::Vec3b>(index_rows, index_cols)[0] =
				image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[0];

			result.at<cv::Vec3b>(index_rows, index_cols)[1] =
				image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[1];

			result.at<cv::Vec3b>(index_rows, index_cols)[2] =
				image.at<cv::Vec3b>(topL.first + index_rows, topL.second + index_cols)[2];
		}
	return result;
}

cv::Mat PictureTools::Resize(const cv::Mat& image, const uint16_t& width, const uint16_t& height, const PictureTools::Algorithm& type)
{
	switch (type)
	{
	case PictureTools::Algorithm::NEAREST_NEIGHBOUR:
		return PictureTools::NearestNeighbour(image, width, height);

	case PictureTools::Algorithm::BILINEAR_INTERPOLATION:
		return PictureTools::BilinearInterpolation(image, width, height);

	default:
		return cv::Mat();
	}
}



cv::Mat PictureTools::NearestNeighbour(const cv::Mat& image, const uint16_t& width, const uint16_t& height)
{
	assert(!image.empty());
	cv::Mat newimage(height, width, CV_8UC3);

	const int x_ratio = (image.cols << 16) / width + 1;
	const int y_ratio = (image.rows << 16) / height + 1;

	for (int rows = 0; rows < height; ++rows) {
		for (int cols = 0; cols < width; ++cols) {
			const int nearX = ((cols * x_ratio) >> 16);
			const int nearY = ((rows * y_ratio) >> 16);

			newimage.at<cv::Vec3b>(rows, cols) = cv::Vec3b(image.at<cv::Vec3b>(nearY, nearX));
		}
	}

	return newimage;
}

double PictureTools::Interpolation(double firstNeighbour, double secondNeighbour, double proportion)
{
	return (secondNeighbour - firstNeighbour) * proportion + firstNeighbour;
}

cv::Vec3b PictureTools::Interpolation(const cv::Vec3b& firstNeighbour, const cv::Vec3b& secondNeighbour, double proportion)
{
	return cv::Vec3b(
		static_cast<uchar>(Interpolation(firstNeighbour[0], secondNeighbour[0], proportion)),
		static_cast<uchar>(Interpolation(firstNeighbour[1], secondNeighbour[1], proportion)),
		static_cast<uchar>(Interpolation(firstNeighbour[2], secondNeighbour[2], proportion))
	);
}

cv::Mat PictureTools::BilinearInterpolation(const cv::Mat& image, const uint16_t& width, const uint16_t& height)
{

	cv::Mat newimage(height, width, CV_8UC3);

	const double heightRatio = height / static_cast<double>(image.rows);
	const double widthRatio = width / static_cast<double>(image.cols);

	for (int hIndex = 0; hIndex < height; ++hIndex)
	{
		for (int wIndex = 0; wIndex < width; ++wIndex)
		{
			std::array <cv::Point, 2> neighbours;

			const double h_t = hIndex / heightRatio;
			const double w_t = wIndex / widthRatio;

			neighbours[0].x = static_cast<int>(h_t);
			neighbours[0].y = static_cast<int>(w_t);
			neighbours[1].x = neighbours[0].x + 1;
			neighbours[1].y = neighbours[0].y + 1;

			if (neighbours[1].x < image.rows && neighbours[1].y < image.cols)
			{
				cv::Vec3b valueW2, valueW1;
				valueW1 = Interpolation(
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y),
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[1].y),
					h_t - neighbours[0].x
				);

				valueW2 = Interpolation(
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y),
					image.at<cv::Vec3b>(neighbours[0].x, neighbours[1].y),
					h_t - neighbours[0].x
				);

				const cv::Vec3b valueH = Interpolation(
					valueW1,
					valueW2,
					w_t - neighbours[0].y
				);

				newimage.at<cv::Vec3b>(hIndex, wIndex) = valueH;
			}
			else
			{
				newimage.at<cv::Vec3b>(hIndex, wIndex) = image.at<cv::Vec3b>(neighbours[0].x, neighbours[0].y);
			}
		}
	}

	return newimage;
}


cv::Scalar PictureTools::AverageColorRectangle(const cv::Mat& image, const Point& startLocation, const Point& size)
{
	assert(!image.empty());
	double green, red;
	double blue = green = red = 0;
	const int sizeOfPartition = (size.first) * (size.second);
	const int rowSize = size.first + startLocation.first;
	const int colSize = size.second + startLocation.second;
	for (int rows = startLocation.first; rows < rowSize; ++rows)
	{
		for (int cols = startLocation.second; cols < colSize; ++cols)
		{
			blue += static_cast<int>(image.at<cv::Vec3b>(rows, cols)[0]) / static_cast<double>(100);
			green += static_cast<int>(image.at<cv::Vec3b>(rows, cols)[1]) / static_cast<double>(100);
			red += static_cast<int>(image.at<cv::Vec3b>(rows, cols)[2]) / static_cast<double>(100);

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

cv::Scalar PictureTools::AverageColorTriangle(const cv::Mat& image, const Point& startLocation, const Point& size, const uint8_t& type)
{
	assert(!image.empty());
	double green, red;
	double blue = green = red = 0;
	const int sizeOfPartition = (size.first) * (size.second) / 2;

	switch (type)
	{
	case 1:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = 0; cols < size.second - rows; ++cols)
			{
				blue += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0]) / static_cast<double>(100);
				green += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1]) / static_cast<double>(100);
				red += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2]) / static_cast<double>(100);
			}
		}

		break;
	case 2:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = size.second - rows; cols < size.second; ++cols)
			{
				blue += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0]) / static_cast<double>(100);
				green += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1]) / static_cast<double>(100);
				red += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2]) / static_cast<double>(100);
			}
		}

		break;
	case 3:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = rows; cols < size.second; ++cols)
			{
				blue += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0]) / static_cast<double>(100);
				green += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1]) / static_cast<double>(100);
				red += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2]) / static_cast<double>(100);
			}
		}

		break;
	case 4:
		for (int rows = 0; rows < size.first; ++rows)
		{
			for (int cols = 0; cols < rows; ++cols)
			{
				blue += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[0]) / static_cast<double>(100);
				green += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[1]) / static_cast<double>(100);
				red += static_cast<int>(image.at<cv::Vec3b>(startLocation.first + rows, startLocation.second + cols)[2]) / static_cast<double>(100);
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

void PictureTools::AlphaBlending(cv::Mat& image, const cv::Scalar& color)
{
	for (auto x = 0; x < image.rows; ++x)
	{
		for (auto y = 0; y < image.cols; ++y)
		{
			image.at<cv::Vec3b>(x, y)[0] = (image.at<cv::Vec3b>(x, y)[0] + color[0] / 1.25) / 2;
			image.at<cv::Vec3b>(x, y)[1] = (image.at<cv::Vec3b>(x, y)[1] + color[1] / 1.25) / 2;
			image.at<cv::Vec3b>(x, y)[2] = (image.at<cv::Vec3b>(x, y)[2] + color[2] / 1.25) / 2;
		}
	}
}
