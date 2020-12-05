#include "BasePictures.h"
#include "PictureTools.h"

BasePictures::BasePictures(const uint16_t& numberPictures, const std::string& pictureExtension)
{
	this->m_numberPictures = numberPictures;
	this->m_pictureExtension = pictureExtension;
}

const std::unordered_map<cv::Scalar, std::string>& BasePictures::GetMediumColor() const
{
	return m_mediumColor;
}



const void BasePictures::CreatingPicturesForMosaics(const std::string& fileSource, const std::string& fileDestination) 
{
	uint16_t count = 0;
	std::string image_path = fileSource;
	image_path += std::to_string(count) + m_pictureExtension;

	while (count < this->m_numberPictures)
	{
		cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
		img=PictureTools::resize(img, 10, 10);
		this->m_mediumColor.insert(std::make_pair(PictureTools::averageColor(img), std::to_string(count) + this->m_pictureExtension));

		cv::imwrite(fileDestination+std::to_string(count)+this->m_pictureExtension, img);
		assert(!img.empty());
			
		++count;
		image_path = fileSource + std::to_string(count) + m_pictureExtension;

	}
	

}

const void BasePictures::setPictureExtension(const std::string& extension)
{
	this->m_pictureExtension = extension;
}

const void BasePictures::setNumberPicture(const std::uint16_t& number)
{
	this->m_numberPictures = number;
}

cv::Mat BasePictures::readPhoto(const std::string& pictureName, const std::string& fileName)
{
	cv::Mat img = cv::imread(fileName+ pictureName, cv::IMREAD_COLOR);
	assert(!img.empty());
	return img;
}

size_t BasePictures::HashKey::operator()(const cv::Scalar& toHash) const
{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(toHash.val[0]);
		res = res * 31 + std::hash<double>()(toHash.val[1]);
		res = res * 31 + std::hash<double>()(toHash.val[2]);
		return res;
}
