#include "PictureTools.h"
#include <iostream>

Mat PictureTools::crop(Mat image, std::pair <uint8_t, uint8_t> topL, std::pair <uint8_t, uint8_t> botR)
{
	uint8_t height = botR.first - topL.first;
	uint8_t width = botR.second - topL.second;
	Mat result(height,width, CV_8UC3);
	std::cout << result.rows << " " << result.cols;
	for (int index_rows = 0; index_rows < height ; index_rows++)
		for (int index_cols = 0; index_cols < width; index_cols++)
		{
			result.at<Vec3b>(index_rows, index_cols)[0] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[0];
			result.at<Vec3b>(index_rows, index_cols)[1] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[1];
			result.at<Vec3b>(index_rows, index_cols)[2] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[2];
		}
	return result;
}
