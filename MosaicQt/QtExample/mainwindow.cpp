#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QPicture>
#include <QAction>
#include <qboxlayout.h>
#include <qlabel.h>
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
	std::unique_ptr<QLabel> labelHelp = std::make_unique<QLabel>();
	std::unique_ptr<QHBoxLayout> layoutLabelHelp = std::make_unique<QHBoxLayout>();
	
	labelHelp->setWindowTitle("Help");
	labelHelp->setText("	[..[..                                                           \n"
		"	[. [..   [...                            [.             [..        [..   \n"
		"	[.. [.. [ [..   [..     [....    [..          [...     [..        [..   \n"
		"	[..  [..  [.. [..  [.. [..     [..  [.. [.. [..   [... [.....[... [.....\n"
		"	[..   [.  [..[..    [..  [... [..   [.. [..[..         [..        [..   \n"
		"	[..       [.. [..  [..     [..[..   [.. [.. [..        [..        [..   \n"
		"	[..       [..   [..    [.. [..  [.. [...[..   [...                      \n"

		"\nFor more information on a specific command, type HELP command-name \n"
		"HELP			Provides Help information for Mosaic++ commands.\n"
		"MAKE			Generates mosaic photo out of a given shape from an input image using a set of images.\n"
		"SET_IMG_POOL		Sets important absolute paths for MAKE command.\n\n\n");

	layoutLabelHelp->addWidget(labelHelp.get());

	Qt::Alignment alignment;
	alignment.setFlag(Qt::AlignmentFlag::AlignCenter);
	layoutLabelHelp->setAlignment(alignment);

	help->setMinimumWidth(300);
	help->setMinimumHeight(300);

	help->setLayout(layoutLabelHelp.get());
	help->setWindowTitle("ReadMe");
	help->exec();
	//help->setT
}

MainWindow::MainWindow(std::unique_ptr<QWidget> parent) :
	QMainWindow(parent.get()),
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

