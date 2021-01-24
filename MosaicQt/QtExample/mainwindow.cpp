#include "mainwindow.h"

std::string MainWindow::selectBasePicturesFolder()
{
	QFileDialog dialog(this);
	QString aux = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/",
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
	ui->progressBarMosaic->setValue(0);
	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	if (pictureForMosaicPathString.size() == 0)
	{
		error->setWindowTitle("Error");
		error->setWindowIcon(QIcon("Pictures\\error.png"));
		error->setText("Incorect path for picture!");
		error->show();
		return false;
	}

	std::string basePicturePathString = ui->textEditBasePictureFolder->toPlainText().toStdString();
	BasePictures basePictures;
	if (!ui->checkBoxBasePictures->isChecked())
	{
		basePictures.AddBasePicturesMosaic();
	}
	if (basePicturePathString.size() > 0)
	{
		basePictures.CreatePictures(basePicturePathString);
	}
	if (basePictures.GetNumberPictures() < 10)
	{
		error->setWindowTitle("Error");
		error->setWindowIcon(QIcon("Pictures\\error.png"));
		error->setText("No base pictures for mosaic!");
		error->show();
		return false;
	}

	cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

	auto typeCell = [u = *ui]{
		if (u.diamondRadioButton->isChecked())
			return Type::DIAMOND;
		if (u.squareRadioButton->isChecked())
			return Type::SQUARE;
		return Type::TRIANGLE;
	};
	auto method = [u = *ui]{
		if (u.radioButtonMethodCropping->isChecked())
			return Method::CROPPING;
		return Method::RESIZING;
	};
	auto algorithm = [u = *ui]{
		if (u.radioButtonEuclidianAlgorithm->isChecked())
			return Algorithm::EUCLIDEAN;
		return Algorithm::RIEMERSMA;
	};

	cv::Mat output;

	auto test = [u = *ui, &output, &input, &basePictures, &method, &typeCell, &algorithm]{
		output = Mosaic::MakeMosaic(input, basePictures, method(), typeCell(), u.spinBoxCellSize->value(), algorithm(), u.checkBoxBlendingPicture->isChecked());
	};
	std::thread t1{ test };

	t1.join();

	std::string folderForResultPathString = ui->textEditFolderResultForPicture->toPlainText().toStdString();

	auto extension = [u = *ui]{
		if (u.extensionJPG->isChecked())
			return ".jpg";
		return ".png";
	};

	auto outputPath = [u = *ui, &extension]{
		if (u.textEditFolderResultForPicture->toPlainText().toStdString().size() == 0)
		{
			return Data::Defaults::PATH_RESULT_IMAGE + u.textEditNameResultPicture->toPlainText().toStdString() + extension();
		}
		return u.textEditFolderResultForPicture->toPlainText().toStdString() + "/" + u.textEditNameResultPicture->toPlainText().toStdString() + extension();
	};

	if (!ui->checkBoxOriginalSize->isChecked())
	{
		output = PictureTools::resize(output, ui->spinBoxWidthResultPicture->value(), ui->spinBoxHeightResultPicture->value());
	}

	cv::imwrite(outputPath(), output);
	QPixmap mosaic(std::move(QString::fromStdString(outputPath())));
	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::IgnoreAspectRatio));

	//Mosaic::progress = 10;
	ui->progressBarMosaic->setValue(100);
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

	QString textLayout(QString::fromStdString(Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP)));
	labelHelp->setText(textLayout);

	layoutLabelHelp->addWidget(labelHelp.get());

	Qt::Alignment alignment;
	alignment.setFlag(Qt::AlignmentFlag::AlignCenter);
	layoutLabelHelp->setAlignment(alignment);

	help->setMinimumWidth(300);
	help->setMinimumHeight(300);

	help->setLayout(layoutLabelHelp.get());
	help->setWindowTitle("ReadMe");
	help->exec();
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

