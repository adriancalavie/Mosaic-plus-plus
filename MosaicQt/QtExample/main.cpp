#include "mainwindow.h"
#include <QtWidgets/QApplication>

#include <qdebug.h>

#include "..\Mosaic\Mosaic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

	/*BasePictures test(5997);

	test.AddPicturesMosaic(false);

	cv::Mat input = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

	std::cout << input.rows << " " << input.cols << std::endl;
	cv::Mat input3 = Mosaic::MakeMosaic(input, test, Method::RESIZING, Type::SQUARE, 30, false);
	std::string str = Data::Defaults::PATH_RESULT_IMAGE + "Diamond.jpg";
	cv::imwrite(str, input3);
	cv::imshow("Processed Image", input3);*/
	cv::Mat input = cv::imread("D:\\Mosaic++\\Mosaic++\\test.jpg", cv::IMREAD_COLOR);
	input = PictureTools::resize(input, 300, 300, PictureTools::Algorithm::BILINEAR_INTERPOLATION);
	cv::imshow("Photo", input);
	cv::waitKey(0);

    return a.exec();
}