#include "BasePictures.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <conio.h>


BasePictures::BasePictures(const uint16_t& number_pictures, const std::string& picture_extension)
{
	this->m_number_pictures = number_pictures;
	this->m_mediun_color.resize(number_pictures);
	this->m_picture_extension = picture_extension;
}

const void BasePictures::CreatingPicturesForMosaics(const std::string& file_source, const std::string& file_destination) const
{
	uint16_t count = 0;
	std::string image_path = file_source;
	image_path += std::to_string(count) + m_picture_extension;

	while (count < this->m_number_pictures)
	{
		cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
		cv::imshow(std::to_string(count), img);
		if (img.empty())
		{
			throw "The picture does not exist";
		}

		



		++count;
		image_path = file_source + std::to_string(count) + m_picture_extension;

		std::cout << img.rows << " " << img.cols << std::endl;
	}
}
