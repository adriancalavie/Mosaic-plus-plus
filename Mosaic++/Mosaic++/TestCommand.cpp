//////ATTENTION: HARDCODED FOR "make" COMMAND ONLY!!!
//
//
//
//#include <iostream>
//#include "BasePictures.h"
//#include "PictureTools.h"
//#include "StopWatch.h"
//#include "Mosaic.h"
//#include <unordered_set>
//#include <io.h>   // For access().
//#include <sys/types.h>  // For stat().
//#include <sys/stat.h>   // For stat().
//#include <regex>
//
////errors
//std::string paramCountErr = "Wrong number of parameters";
//std::string unknownCommandErr = "Unknown command used";
//std::string wrongArgOrder = "Wrong agrument order";
//std::string wrongInputErr = "The argument may be an unsupported file extension or an invalid path(check for file existence!)";
//
//
////consts
//const enum class ParameterType { PATH, EXTENSION, TYPE, NONE };
//
//const std::unordered_set<std::string> knownExtensions = { "jpg", "png" };
//const std::unordered_set<std::string> commands = { "make", "set_img_pool_dir" };
//const std::unordered_map<std::string, ParameterType> flags = {
//	{"--shape",		ParameterType::TYPE},
//	{"-s",			ParameterType::TYPE},
//
//	{"--extension",	ParameterType::EXTENSION},
//	{"-e",			ParameterType::EXTENSION},
//
//	{"--directory",	ParameterType::PATH},
//	{"-d",			ParameterType::PATH},
//
//	{"--help",		ParameterType::NONE},
//	{"-h",			ParameterType::NONE},
//
//	{"--version",	ParameterType::NONE},
//	{"-v",			ParameterType::NONE}
//};
//
//
//
//
//inline bool isExtension(const std::string& parameter)
//{
//	return knownExtensions.find(parameter) != knownExtensions.end();
//}
//
//inline bool isDir(const std::string& parameter)
//{
//	if (_access(parameter.c_str(), 0) == 0)
//	{
//		struct stat status;
//
//		stat(parameter.c_str(), &status);
//
//		if (status.st_mode & S_IFDIR)
//		{
//			return true;
//		}
//
//		else
//		{
//			//this is a file path, not a directory path
//
//			return false;
//		}
//	}
//	else
//	{
//		//The path doesn't exist
//
//		return false;
//	}
//
//}
//
//inline bool isType(const std::string& parameter)
//{
//	return (parameter == "diamond" || parameter == "rectangle" || parameter == "square" || parameter == "triangle");
//}
//
//inline bool inCheck(const std::string& flag, const std::string& parameter)
//{
//	switch (flags.at(flag))
//	{
//	case ParameterType::EXTENSION:
//		return isExtension(parameter);
//		break;
//
//	case ParameterType::PATH:
//		return isDir(parameter);
//		break;
//
//	case ParameterType::TYPE:
//		return isType(parameter);
//		break;
//
//	case ParameterType::NONE:
//	default:
//		std::cerr << "No idea how you got here";
//		break;
//	}
//
//	return false;
//}
//
//inline bool isFile(const std::string& parameter)
//{
//
//	std::string matcher = ".*.(";
//
//	for (auto extension : knownExtensions)
//	{
//		matcher += extension + '|';
//	}
//
//	matcher[matcher.size() - 1] = ')';
//	return std::regex_match(parameter, std::regex(matcher));
//}
//
//inline bool isPath(const std::string& parameter)
//{
//	struct stat buffer;
//	return (stat(parameter.c_str(), &buffer) == 0);
//}
//
//
//
//void make(const std::vector<std::string>& params)
//{
//	std::cout << "entered make function\n";
//
//	BasePictures test(5900);
//
//	std::cout << "constructed base pictures\n";
//
//	test.addPicturesMosaic(false);
//
//	std::cout << "added images\n";
//
//	for (auto image : params)
//	{
//		std::cout << "selecting image\n";
//
//		cv::Mat input = cv::imread(image, cv::IMREAD_COLOR);
//
//		std::cout << "read image\n";
//
//		input = PictureTools::resize(input, 1200, 700);
//
//		std::cout << "resized image\n";
//
//		cv::Mat input3 = Mosaic::makeMosaic(input, test, Type::square, 10);
//
//		std::cout << "made mosaic\n";
//		cv::imwrite("D:\\Mosaic++\\Mosaic++\\Resulting pictures\\" + std::to_string(test.getNumberPictures()) + test.getExtension(), input3);
//
//		std::cout << "wrote to disk\n";
//
//		cv::imshow("CommandTest_1.2_", input3);
//
//		std::cout << "show image\n";
//		cv::waitKey(0);
//	}
//}
//
//void make(const std::string& flag)
//{
//	if (flag == "-v" || flag == "--version")
//	{
//		std::cout << "0.0.1";
//		return;
//	}
//
//
//	if (flag == "-h" || flag == "--help")
//	{
//		std::cout << "*help*";
//	}
//}
//void make(const std::vector<const std::string&>& params, const std::unordered_map<std::string, std::string>& flags)
//{
//
//}
//
//int main(int argc, char* args[])
//{
//	if (argc < 2)
//	{
//		std::cerr << paramCountErr;
//	}
//	else
//		//if we have enough arguments
//	{
//		if (commands.find(std::string(args[1])) != commands.end())
//			//if we have a valid command as a first argument
//		{
//			bool hasParams = false;
//			std::unordered_map<std::string, std::string> flagsParams;
//			std::vector<std::string> commandParams;
//
//			for (int i = 2; i < argc; ++i)
//				//we search for either flags or parameters
//			{
//				std::string argument(args[i]);
//
//				if (flags.find(argument) != flags.end())
//					//if we found a flag
//				{
//
//					if (flags.at(argument) == ParameterType::NONE)
//					{
//						make(argument);
//						return 0;
//					}
//					else
//					{
//						if (hasParams)
//							//if the flag requires parameters
//						{
//							if (i < argc)
//								//if we have enough arguments
//								if (inCheck(argument, args[++i]))
//									//if the flag parameter is valid
//									flagsParams.insert({ argument, args[i] });
//						}
//						else
//						{
//							std::cerr << wrongArgOrder;
//						}
//					}
//
//				}
//				else
//					//else, if we find a command parameter
//				{
//					if (isPath(argument) && isFile(argument))//TODO: optimise for set_img_pool_dir
//						//if we have an image as an input
//					{
//						commandParams.push_back(argument);
//						hasParams = true;
//					}
//					else
//					{
//						std::cerr << wrongInputErr;
//					}
//				}
//			}
//
//			
//			make(commandParams);
//		}
//		else
//		{
//			if (flags.find(std::string(args[1])) != flags.end() && flags.at(std::string(args[1])) == ParameterType::NONE)
//				//if instead of a command, we find a help or version request
//			{
//				//TODO: show all documentation on all the commands
//			}
//			else
//			{
//				std::cerr << unknownCommandErr;
//			}
//		}
//	}
//
//	return 0;
//}