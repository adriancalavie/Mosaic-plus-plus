#include "BasePictures.h"
#include "PictureTools.h"

BasePictures::BasePictures(const uint16_t& numberPictures)
{
	this->m_numberPictures = numberPictures;
	this->m_pictureExtension = ".jpg";

	fileSource = "..//Base Pictures\\";
	fileDestination = "..//Pictures for mosaics\\";
}

const std::unordered_map<cv::Scalar, std::string>& BasePictures::GetMediumColor() const
{
	return m_mediumColor;
}

const void BasePictures::CreatingPicturesForMosaics() 
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

const uint16_t BasePictures::getNumberPicture() const
{
	return this->m_numberPictures;
}

const std::string BasePictures::getPictureExtension() const
{
	return this->m_pictureExtension;
}

cv::Mat BasePictures::readPhoto(const std::string& pictureName, const std::string& fileName)
{
	cv::Mat img = cv::imread(fileName+ pictureName, cv::IMREAD_COLOR);
	assert(!img.empty());
	return img;
}

const std::string& BasePictures::getFileSource() const
{
	return fileSource;
}

void BasePictures::setFileSource(const std::string& source)
{
	fileSource = source;
}

const std::string& BasePictures::getFileDestination() const
{
	return fileDestination;
}

void BasePictures::setFileDestination(const std::string& source)
{
	fileDestination = source;
}

size_t BasePictures::HashKey::operator()(const cv::Scalar& toHash) const
{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(toHash.val[0]);
		res = res * 31 + std::hash<double>()(toHash.val[1]);
		res = res * 31 + std::hash<double>()(toHash.val[2]);
		return res;
}
