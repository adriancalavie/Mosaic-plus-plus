#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>


class BasePictures
{
private:
	std::vector<cv::Scalar> m_mediumColor;
	std::uint16_t m_numberPictures;
	std::string m_pictureExtension;


public:
	BasePictures(const uint16_t&, const std::string&);

	const void CreatingPicturesForMosaics(const std::string& ="..//Base pictures\\", const std::string& = "..//Pictures for mosaics\\" );
};

