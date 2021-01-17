#include "Data.h"

//INFOS:

const std::string Data::Info::VERSION = "0.0.1a";

const std::string Data::Info::GENERAL_HELP =

"Mosaic++"
" [Version " + Data::Info::VERSION + "]\n\n"

"	[..       [..                                                           \n"
"	[. [..   [...                           [.             [..        [..   \n"
"	[.. [.. [ [..   [..     [....    [..          [...     [..        [..   \n"
"	[..  [..  [.. [..  [.. [..     [..  [.. [.. [..   [... [.....[... [.....\n"
"	[..   [.  [..[..    [..  [... [..   [.. [..[..         [..        [..   \n"
"	[..       [.. [..  [..     [..[..   [.. [.. [..        [..        [..   \n"
"	[..       [..   [..    [.. [..  [.. [...[..   [...                      \n"

"\nFor more information on a specific command, type HELP command-name \n"
"HELP			Provides Help information for Mosaic++ commands.\n"
"MAKE			Generates mosaic photo out of a given shape from an input image using a set of images.\n"
"SET_IMG_POOL	Sets important absolute paths for MAKE command.\n\n\n";

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
const std::string Data::Defaults::PARENT_PATH = "..//";
const std::string Data::Defaults::PATH_BASE_PICTURES = "..//Base pictures\\";
const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "..//Pictures for mosaics\\";
const std::string Data::Defaults::PATH_DATA_BASE_FILE = "data_base.txt";
const std::string Data::Defaults::PATH_TEST_IMAGE = "..//test.jpg";
const std::string Data::Defaults::PATH_RESULT_IMAGE = "..//Resulting pictures\\";
#else
const std::string Data::Defaults::PARENT_PATH = "..//..//";
const std::string Data::Defaults::PATH_BASE_PICTURES = "..//..//Base pictures\\";
const std::string Data::Defaults::PATH_PICTURES_FOR_MOSAIC = "..//..//Pictures for mosaics\\";
const std::string Data::Defaults::PATH_DATA_BASE_FILE = "..//Mosaic++\\data_base.txt";
const std::string Data::Defaults::PATH_TEST_IMAGE = "..//..//test.jpg";
const std::string Data::Defaults::PATH_RESULT_IMAGE = "..//..//Resulting pictures\\";
#endif
//DEFAULTS END