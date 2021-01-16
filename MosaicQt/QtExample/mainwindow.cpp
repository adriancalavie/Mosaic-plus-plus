#include "mainwindow.h"

#include "ui_mainwindow.h"
//#include <iostream>
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QPicture>
#include <QAction>
#include <windows.h>
#include <QStringListModel>
#include <fstream>

std::string MainWindow::selectBasePicturesFolder()
{
	QFileDialog dialog(this);
	QString aux = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->textEditBasePictureFolder->setText(aux);
	return aux.toStdString();
}

std::string MainWindow::selectFolderForResult()
{
	QFileDialog dialog(this);
	QString aux = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->textEditFolderResultForPicture->setText(aux);

	return aux.toStdString();
}

std::string MainWindow::selectPictureForMosaic()
{
	QString aux = QFileDialog::getOpenFileName(
		this, tr("Open Directory"), "D:", "*.jpg; *.png");

	ui->textEditPictureForMosaic->setText(aux);
	QPixmap pixmap(aux);
	ui->labelOriginalPicture->setPixmap(pixmap.scaled(ui->labelOriginalPicture->width(),
		ui->labelOriginalPicture->height(), Qt::IgnoreAspectRatio));
	return aux.toStdString();
}

bool MainWindow::startMosaic()
{
	std::string basePicturePathString = ui->textEditBasePictureFolder->toPlainText().toStdString();

	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	if (pictureForMosaicPathString.size() == 0)
	{
		error->setWindowTitle("Error");
		error->setWindowIcon(QIcon("Pictures\\error.png"));
		error->setText("Incorect path for picture!");
		error->show();
		return false;
	}

	std::string folderForResultPathString = ui->textEditFolderResultForPicture->toPlainText().toStdString();

	static int value = 0;
	ui->progressBarMosaic->setValue(++value);
	return true;
}


void MainWindow::actionExit()
{
	exit(1);
}
void MainWindow::actionHelp()
{
	help->show();
	help->setWindowTitle("Help mosaic++");
}

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->buttonBasePicturesFolder, &QPushButton::released, this, &MainWindow::selectBasePicturesFolder);
	connect(ui->buttonPictureForMosaic, &QPushButton::released, this, &MainWindow::selectPictureForMosaic);
	connect(ui->buttonFolderForTheResult, &QPushButton::released, this, &MainWindow::selectFolderForResult);
	connect(ui->buttonMakeMosaic, &QPushButton::released, this, &MainWindow::startMosaic);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::actionExit);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::actionHelp);
}

MainWindow::~MainWindow()
{
}

