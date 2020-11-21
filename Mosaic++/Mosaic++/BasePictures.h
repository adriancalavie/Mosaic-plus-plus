#pragma once
#include <vector>
#include <string>
class BasePictures
{
private:
	std::vector<uint16_t> m_mediun_color;
	std::uint16_t m_number_pictures;
	std::string m_picture_extension;


public:
	BasePictures(const uint16_t&, const std::string&);

	const void CreatingPicturesForMosaics(const std::string& ="..//Base pictures\\", const std::string& = "..//Pictures for mosaics\\" ) const;
};

