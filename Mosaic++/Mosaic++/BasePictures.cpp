#include "BasePictures.h"
#include "PictureTools.h"

BasePictures::BasePictures(const uint16_t& numberPictures, const std::string& pictureExtension)
{
	this->m_numberPictures = numberPictures;
	this->m_mediumColor.resize(numberPictures);
	this->m_pictureExtension = pictureExtension;
}

const std::vector<cv::Scalar>& BasePictures::GetMediumColor() const
{
	return this->m_mediumColor;
}

const void BasePictures::CreatingPicturesForMosaics(const std::string& fileSource, const std::string& fileDestination) 
{
	uint16_t count = 0;
	std::string image_path = fileSource;
	image_path += std::to_string(count) + m_pictureExtension;

	while (count < this->m_numberPictures)
	{
		cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
		img=PictureTools::resize(img, 50, 50);

		m_mediumColor[count]=(PictureTools::averageColor(img));
		//this->m_mediumColorv2.insert(std::make_pair(PictureTools::averageColor(img), std::to_string(count) + this->m_pictureExtension));
		// need a hash function for unorder_map


		cv::imwrite(fileDestination+std::to_string(count)+this->m_pictureExtension, img);
		assert(!img.empty());
			
		++count;
		image_path = fileSource + std::to_string(count) + m_pictureExtension;

	}
	

}
