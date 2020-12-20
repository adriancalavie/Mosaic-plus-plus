#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
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

	using map = std::unordered_map<cv::Scalar, std::string>;

private:
	struct HashKey
	{
		size_t operator()(const cv::Scalar& toHash) const;
	};

private:
	std::uint16_t m_numberPictures;
	map m_mediumColor;

	std::string m_source;
	std::string m_processedPictures;
	std::string m_extension;


public:
	BasePictures(const uint16_t & = 0);

	const map& GetMediumColor() const;
	const void CreatePictures();

	void setFolder(const std::string&);
	void setPicturesNumber(const std::uint16_t&);

	const uint16_t& getPictureCount() const;
	static cv::Mat readPhoto(const std::string & = "\n", const std::string & = "C:\\Users\\radub\\Desktop\\proiect-modrern-c\\Mosaic++\\Pictures for mosaics\\");

public:
	const std::string& getFileSource() const;
	void setFileSource(const std::string&);

	const std::string& getFileDestination() const;
	void setFileDestination(const std::string&);

	const std::string& getExtension() const;
	void setExtension(const std::string&);

	const uint16_t& getNumberPictures() const;
	void setNumberPictures(const uint16_t&);


	void addPicturesMosaic(const bool&);

};

