#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <unordered_map>

/*
template <>
struct std::hash<cv::Scalar>
{
	//do smt magic
	size_t operator()(const cv::Scalar& k) const
	{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(k.val[0]);
		res = res * 31 + std::hash<double>()(k.val[1]);
		res = res * 31 + std::hash<double>()(k.val[2]);
		return res;
	}

};
*/

class BasePictures
{

private:
	struct HashKey
	{
		size_t operator()(const cv::Scalar& toHash) const;
	};

private:
	std::vector<cv::Scalar> m_mediumColor;
	std::uint16_t m_numberPictures;
	std::string m_pictureExtension;
	std::unordered_map<cv::Scalar, std::string, HashKey> m_mediumColorv2;

public:
	BasePictures(const uint16_t&, const std::string&);
	const std::vector<cv::Scalar>& GetMediumColor() const;
	const void CreatingPicturesForMosaics(const std::string& ="..//Base pictures\\", const std::string& = "..//Pictures for mosaics\\" );
};

