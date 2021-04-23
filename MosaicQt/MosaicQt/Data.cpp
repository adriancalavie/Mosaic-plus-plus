#include "Data.h"

//INFOS:

const std::string Data::Info::VERSION = "0.0.2a";

const std::string Data::Info::GENERAL_HELP =

"Mosaic++"
" [Version " + Data::Info::VERSION + "]\n\n"

"	[..       [..                                                           \n"
"	[. [..   [...                            [.              [..        [..   \n"
"	[.. [.. [ [..   [..     [....    [..          [...      [..        [..   \n"
"	[..  [..  [.. [..  [.. [..     [..  [.. [.. [..   [... [.....[... [.....\n"
"	[..   [.  [..[..    [..  [... [..   [.. [..[..         [..        [..   \n"
"	[..       [.. [..  [..     [..[..   [.. [.. [..        [..        [..   \n"
"	[..       [..   [..    [.. [..  [.. [...[..   [...                      \n\n"

"For more information on a specific element, put your mouse over it.\n"
"SELECT PICTURE 				Select a picture to be Mosaiced.\n"
"RESULT FOLDER				Select the folder in which the Mosaiced picture will be saved.\n"
"BASE PICTURES FOLDER			Select the folder to your pictures' database. The pictures are added in our database.\n"
"USE ONLY THIS PHOTO			Keep this off to append the photos from the above folder to the database.\n"
"START					Start the Mosaic++ algorithm for the selected image.\n"
"CELL SIZE				This is the size of the partition used to split the original image.\n"
"RESOLUTION				Resolution for the resulting Mosaic++.\n"
"ORIGINAL SIZE				Keep this on to retain the original width and height of the image(after Crop or Resize).\n"
"EXTENSION				Extension for the resulting Mosaic++.\n"
"METHOD					Method used for resolving uncommon resolutions for the original picture.\n"
"ALGORITHM				Algorithm used to calculate the distance between mapped colors. \n"
"CELL SHAPE				The shape of the cells used in the Mosaic++.\n"
"NAME					Provides a name for the resulting Mosaic++.\n\n"
"You can contact us here:\n"
"Calavie Adrian		-->		adrian.calavie06@gmail.com\n"
"Arhip Florin		-->		florin9925@gmail.com\n"
"Dinu Alin			-->		dinui.alin@gmail.com\n";





//INFOS END


// ERRORS:
const std::string Data::Errors::BASE_PICTURES_EMPTY = "No base pictures for mosaic!";
const std::string Data::Errors::PICTURE_FOR_MOSAIC_EMPTY = "Incorect path for picture!";
const std::string Data::Errors::UNSUPPORTED_PICTURE ="Something is wrong with your photo! \n Please check your photo." ;
const std::string Data::Errors::ANOTHER_ERROR = "Unknown error! Please contact the administrator.";
//ERRORS END


//DEFAULTS
#ifdef _DEBUG
//const std::string Data::Defaults::PARENT_PATH = "..//..//Mosaic++\\";
//const std::string Data::Defaults::PATH_BASE_PICTURES = "D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\MosaicQt\\MosaicQt\\Resources\\Base pictures\\";
//const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\MosaicQt\\MosaicQt\\Resources\\Pictures for mosaics\\";
//const std::string Data::Defaults::PATH_DATA_BASE_FILE = "D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\MosaicQt\\MosaicQt\\Resources\\Database\\data_base.txt";
//const std::string Data::Defaults::PATH_TEST_IMAGE = "D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\MosaicQt\\MosaicTests\\test.jpg";
//const std::string Data::Defaults::PATH_RESULT_IMAGE = "D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\MosaicQt\\MosaicQt\\Resources\\Resulting pictures\\";


const std::string Data::Defaults::PATH_BASE_PICTURES = "Resources\\Base pictures\\";
const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "Resources\\Pictures for mosaics\\";
const std::string Data::Defaults::PATH_DATA_BASE_FILE = "Resources\\Database\\data_base.txt";
const std::string Data::Defaults::PATH_TEST_IMAGE = "test.jpg";
const std::string Data::Defaults::PATH_RESULT_IMAGE = "Resources\\Resulting pictures\\";

#else
//const std::string Data::Defaults::PARENT_PATH = "..//..//Mosaic++\\";
const std::string Data::Defaults::PATH_BASE_PICTURES = "Resources\\Base pictures\\";
const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "Resources\\Pictures for mosaics\\";
const std::string Data::Defaults::PATH_DATA_BASE_FILE = "Resources\\Database\\data_base.txt";
const std::string Data::Defaults::PATH_TEST_IMAGE = "test.jpg";
const std::string Data::Defaults::PATH_RESULT_IMAGE = "Resources\\Resulting pictures\\";
#endif
//DEFAULTS END