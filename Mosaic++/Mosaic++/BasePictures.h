#pragma once

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <unordered_map>


template <>
struct std::hash<cv::Scalar>
{
	size_t operator()(const cv::Scalar& k) const
	{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(k.val[0]);
		res = res * 31 + std::hash<double>()(k.val[1]);
		res = res * 31 + std::hash<double>()(k.val[2]);
		return res;
	}

};


class BasePictures
{

private:
	struct HashKey
	{
		size_t operator()(const cv::Scalar& toHash) const;
	};


private:
	std::uint16_t m_numberPictures;
	std::string m_pictureExtension;
	std::unordered_map<cv::Scalar, std::string> m_mediumColor;

public:
	BasePictures(const uint16_t& = 0, const std::string& = ".jpg");
	const std::unordered_map<cv::Scalar, std::string>& GetMediumColor() const;
	const void CreatingPicturesForMosaics(const std::string& ="..//Base pictures\\", const std::string& = "..//Pictures for mosaics\\" );

	const void setPictureExtension(const std::string&);
	const void setNumberPicture(const std::uint16_t&);

	const uint16_t getNumberPicture() const;
	const std::string getPictureExtension() const;

	static cv::Mat readPhoto(const std::string &  ="\n" , const std::string & = "..//Pictures for mosaics\\");
};

