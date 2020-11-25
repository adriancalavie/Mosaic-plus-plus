#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <unordered_map>



class BasePictures
{
private:
	std::vector<cv::Scalar> m_mediumColor;
	std::uint16_t m_numberPictures;
	std::string m_pictureExtension;
	//std::unordered_map<cv::Scalar, std::string> m_mediumColorv2; 
	// need a hash function for unorder_map

public:
	BasePictures(const uint16_t&, const std::string&);
	const std::vector<cv::Scalar>& GetMediumColor() const;
	const void CreatingPicturesForMosaics(const std::string& ="..//Base pictures\\", const std::string& = "..//Pictures for mosaics\\" );
};

