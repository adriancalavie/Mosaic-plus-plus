#include "Data.h"

//INFOS:

const std::string Data::Info::VERSION = "0.0.1a";

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

"For more information on a specific element, put your mouse over him.\n"
"SELECT PICTURE 			Select a picture to be Mosaiced.\n"
"RESULT FOLDER				Select the folder in which the Mosaiced picture will be saved.\n"
"BASE PICTURES FOLDER			Select the folder to your pictures' database. The pictures are added in our database.\n"
"USE ONLY THIS PHOTO			Keep this off to append the photos from the above folder to the database.\n"
"START					Start the Mosaic++ algorithm for the selected image.\n"
"CELL SIZE				This is the size of the partition used to split the original image.\n"
"RESOLUTION				Resolution for the resulting Mosaic++.\n"
"ORIGINAL SIZE				Keep this on to retain the original width and height of the image(after Crop or Resize).\n"
"EXTENSION				Extension for the resulting Mosaic++.\n"
"METHOD				Method used for resolving uncommon resolutions for the original picture.\n"
"ALGORITHM				Algorithm used to calculate the distance between mapped colors. \n"
"CELL SHAPE				The shape of the cells used in the Mosaic++.\n"
"NAME					Provides a name for the resulting Mosaic++.\n\n"
"You can contact us here:\n"
"Calavie Adrian		-->		adrian.calavie06@gmail.com\n"
"Arhip Florin		-->		florin9925@gmail.com\n"
"Dinu Alin		-->		dinui.alin@gmail.com\n";



const std::unordered_map< Data::HelpTypes, std::string> Data::Info::HELP_LEVEL = {
	{HelpTypes::MAKE_HELP, "make <image1, ...> [flags] [flag_parameters]\n"
	 "\nflags list\n"
	 "-p"},

	{HelpTypes::GENERAL_HELP, GENERAL_HELP},

	{HelpTypes::SET_POOL_HELP, "set_img_pool <sourcePath> <numberPhotosDatabase> <destinationPath> [processedImagePath], [databaseFile]> [flags] [flag_parameters]\n"}
};






const std::string Data::Info::MAKE_HELP = "make <image1, ...> [flags] [flag_parameters]\n";

const std::string Data::Info::SET_POOL_HELP = "WIP";

//static const std::unordered_set<std::string> KNOWN_EXTENSIONS = { "jpg", "png" };
//INFOS END


// ERRORS:
const std::string Data::Errors::PARAMETER_COUNT = "Wrong number of parameters";
const std::string Data::Errors::WRONG_PARAMETER = "Wrong type of parameter";
const std::string Data::Errors::UNKNOWN_COMMAND = "Unknown command used";
const std::string Data::Errors::WRONG_ARGUMENT_ORDER = "Wrong agrument order";
const std::string Data::Errors::WRONG_INPUT = "The argument may be an unsupported file extension or an invalid path(check for file existence!)";
const std::string Data::Errors::WRONG_ARGUMENT = "One or more arguments are invalid";

const std::string Data::Errors::OUT_OF_BOUNDS = "Partitioning size is too large\Expected bounds ~~~ [0-512) ~~~";
//ERRORS END


//DEFAULTS
#ifdef _DEBUG
//const std::string Data::Defaults::PARENT_PATH = "..//..//Mosaic++\\";
const std::string Data::Defaults::PATH_BASE_PICTURES = "Resources\\Base pictures\\";
const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "Resources\\Pictures for mosaics\\";
const std::string Data::Defaults::PATH_DATA_BASE_FILE = "Resources\\Database\\data_base.txt";
const std::string Data::Defaults::PATH_TEST_IMAGE = "C:\\Users\\adita\Desktop\\jupiter.jpg";
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