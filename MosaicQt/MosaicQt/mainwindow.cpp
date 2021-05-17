#include "mainwindow.h"

std::string MainWindow::SelectBasePicturesFolder()
{
	QFileDialog dialog(this);
	QString aux = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->textEditBasePictureFolder->setText(aux);

	return aux.toStdString();
}

std::string MainWindow::SelectFolderForResult()
{
	QFileDialog dialog(this);
	QString aux = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->textEditFolderResultForPicture->setText(aux);

	return aux.toStdString();
}

std::string MainWindow::SelectPictureForMosaic()
{
	QString aux = QFileDialog::getOpenFileName(
		this, tr("Open Directory"), "D:", "*.jpg; *.png");

	ui->textEditPictureForMosaic->setText(aux);
	QPixmap pixmap(aux);
	ui->labelOriginalPicture->setPixmap(pixmap.scaled(ui->labelOriginalPicture->width(),
		ui->labelOriginalPicture->height(), Qt::KeepAspectRatio));

	qw.get()->GetUI().get()->labelOriginalPicture->setPixmap(pixmap.scaled(qw.get()->GetUI().get()->labelOriginalPicture->width(),
		qw.get()->GetUI().get()->labelOriginalPicture->height(), Qt::KeepAspectRatio));

	return aux.toStdString();
}

bool MainWindow::StartMosaic()
{
	stopwatch timeMosaic;
	timeMosaic.tick();

	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	if (pictureForMosaicPathString.size() == 0)
	{
		Errors(Data::Errors::PICTURE_FOR_MOSAIC_EMPTY);
		return false;
	}

	if (ui->textEditBasePictureFolder->toPlainText().toStdString().size() > 0)
		basePictures.CreatePictures(ui->textEditBasePictureFolder->toPlainText().toStdString());

	if (basePictures.GetNumberPictures() < 10)
	{
		Errors(Data::Errors::BASE_PICTURES_EMPTY);
		return false;
	}

	if (ui->checkBoxQuadTree->isChecked())
	{
		MakeQuadMosaic();
	}
	else
	{
		MakeMosaic();
	}


	timeMosaic.tock();

	std::string convertor = std::to_string(timeMosaic.report_ms() / 1000.0);
	convertor = std::move(convertor.substr(0, convertor.size() - 3));

	ui->waitLabel->setText(std::move(QString::fromStdString("Time elapsed: " + convertor + " seconds")));

	return true;
}

void MainWindow::ActionSettings()
{
	st->show();
}


void MainWindow::ActionExit()
{
	exit(1);
}

void MainWindow::ActionHelp()
{

	QLabel* label = new QLabel(tr("Name:"));

	QHBoxLayout* layout = new QHBoxLayout();

	QString textLayout(QString::fromStdString(Data::Info::GENERAL_HELP));
	label->setText(textLayout);

	Qt::Alignment alignment;
	alignment.setFlag(Qt::AlignmentFlag::AlignCenter);
	layout->setAlignment(alignment);

	help->setMinimumWidth(800);
	help->setMinimumHeight(600);

	layout->addWidget(label);
	help->setLayout(layout);

	help->show();
}

void MainWindow::CheckQuadMosaic()
{
	if (ui->checkBoxQuadTree->isChecked())
	{
		ui->labelCellSize->setVisible(false);
		ui->spinBoxCellSize->setVisible(false);
		ui->diamondRadioButton->setVisible(false);
		ui->triangleRadioButton->setVisible(false);
		ui->squareRadioButton->setChecked(true);
	}
	else
	{
		ui->labelCellSize->setVisible(true);
		ui->spinBoxCellSize->setVisible(true);
		ui->diamondRadioButton->setVisible(true);
		ui->triangleRadioButton->setVisible(true);
		ui->squareRadioButton->setChecked(true);
	}

}

void MainWindow::ShowDetailsQuad()
{
	qw->show();
	ui->buttonShowDetailsQuad->setVisible(false);
}


void MainWindow::MakeMosaic()
{
	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

	if (input.empty())
	{
		Errors(Data::Errors::UNSUPPORTED_PICTURE);
		return;
	}

	auto typeCell = [u = *ui]{
		if (u.diamondRadioButton->isChecked())
			return Type::DIAMOND;
		if (u.squareRadioButton->isChecked())
			return Type::SQUARE;
		return Type::TRIANGLE;
	};

	auto method = [s = st.get()->GetUI()]{
		if (s.get()->radioButtonMethodCropping->isChecked())
			return Method::CROPPING;
		return Method::RESIZING;
	};
	auto algorithm = [s = st.get()->GetUI()]{
		if (s.get()->radioButtonEuclidianAlgorithm->isChecked())
			return Algorithm::EUCLIDEAN;
		return Algorithm::RIEMERSMA;
	};

	cv::Mat output = Mosaic::MakeMosaic(input, basePictures, method(), typeCell(), ui->spinBoxCellSize->value(),
		algorithm(), st.get()->GetUI().get()->checkBoxBlendingPicture->isChecked());
	if (output.empty())
	{
		Errors(Data::Errors::ANOTHER_ERROR);
		return;

	}

	std::string folderForResultPathString = ui->textEditFolderResultForPicture->toPlainText().toStdString();

	auto extension = [s = st.get()->GetUI()]{
		if (s.get()->extensionJPG->isChecked())
			return ".jpg";
		return ".png";
	};

	auto outputPath = [s = st.get()->GetUI(), &extension, u = *ui]{
		if (u.textEditFolderResultForPicture->toPlainText().toStdString().size() == 0)
		{
			return Data::Defaults::PATH_RESULT_IMAGE + s.get()->textEditNameResultPicture->toPlainText().toStdString() + extension();
		}
		return u.textEditFolderResultForPicture->toPlainText().toStdString() + "/" + s.get()->textEditNameResultPicture->toPlainText().toStdString() + extension();
	};

	if (!st.get()->GetUI().get()->checkBoxOriginalSize->isChecked())
	{
		output = Mosaic::pt::Resize(output, st.get()->GetUI().get()->spinBoxWidthResultPicture->value(), st.get()->GetUI().get()->spinBoxHeightResultPicture->value());
	}

	cv::imwrite(outputPath(), output);

	QPixmap mosaic(std::move(QString::fromStdString(outputPath())));
	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::KeepAspectRatio));
}

void MainWindow::MakeQuadMosaic()
{

	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

	Mosaic::imgPair res = Mosaic::MakeQuadTree(basePictures, input, st.get()->GetUI().get()->checkBoxBlendingPicture->isChecked(), st.get()->GetUI().get()->sliderThreshold->value() / 500.0,
		st.get()->GetUI().get()->sliderMinimumSize->value(), st.get()->GetUI().get()->checkBoxDetailsQuad->isChecked());

	auto extension = [s = st.get()->GetUI()]{
		if (s.get()->extensionJPG->isChecked())
			return ".jpg";
		return ".png";
	};

	auto outputPath = [s = st.get()->GetUI(), &extension, u = *ui]{
		if (u.textEditFolderResultForPicture->toPlainText().toStdString().size() == 0)
		{
			return Data::Defaults::PATH_RESULT_IMAGE + s.get()->textEditNameResultPicture->toPlainText().toStdString() + extension();
		}
		return u.textEditFolderResultForPicture->toPlainText().toStdString() + "/" + s.get()->textEditNameResultPicture->toPlainText().toStdString() + extension();
	};

	if (res.second.value().empty())
	{
		Errors(Data::Errors::ANOTHER_ERROR);
		return;

	}

	cv::imwrite(outputPath(), res.first.value());
	if (st.get()->GetUI().get()->checkBoxDetailsQuad->isChecked())
		cv::imwrite(Data::Defaults::PATH_RESULT_IMAGE + "quad" + extension(), res.second.value());

	QPixmap mosaic(std::move(QString::fromStdString(outputPath())));

	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::KeepAspectRatio));

	if (st.get()->GetUI().get()->checkBoxDetailsQuad->isChecked())
	{
		qw.get()->GetUI().get()->labelMosaicPicture->setPixmap(mosaic.scaled(qw.get()->GetUI().get()->labelMosaicPicture->width(),
			qw.get()->GetUI().get()->labelMosaicPicture->height(), Qt::KeepAspectRatio));

		QPixmap quadImage(std::move(QString::fromStdString(Data::Defaults::PATH_RESULT_IMAGE + "quad" + extension())));

		qw.get()->GetUI().get()->labelQuadImage->setPixmap(quadImage.scaled(qw.get()->GetUI().get()->labelQuadImage->width(),
			qw.get()->GetUI().get()->labelQuadImage->height(), Qt::KeepAspectRatio));
		ui->buttonShowDetailsQuad->setVisible(true);
	}

}

void MainWindow::Errors(std::string message)
{
	ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:#148dfa;}");

	error->setWindowTitle("Error");
	error->setWindowIcon(QIcon("Pictures\\error.png"));
	error->setText(QString::fromStdString(message));
	error->show();
}

MainWindow::MainWindow(std::unique_ptr<QWidget> parent) :
	QMainWindow(parent.get()),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->buttonBasePicturesFolder, &QPushButton::released, this, &MainWindow::SelectBasePicturesFolder);
	connect(ui->buttonPictureForMosaic, &QPushButton::released, this, &MainWindow::SelectPictureForMosaic);
	connect(ui->buttonFolderForTheResult, &QPushButton::released, this, &MainWindow::SelectFolderForResult);
	connect(ui->buttonMakeMosaic, &QPushButton::released, this, &MainWindow::StartMosaic);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::ActionExit);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::ActionHelp);
	connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::ActionSettings);
	connect(ui->checkBoxQuadTree, &QPushButton::released, this, &MainWindow::CheckQuadMosaic);
	connect(ui->buttonShowDetailsQuad, &QPushButton::released, this, &MainWindow::ShowDetailsQuad);


	//this->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");
	help->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");
	ui->waitLabel->setWordWrap(true);
	ui->buttonShowDetailsQuad->setVisible(false);
	basePictures.AddBasePicturesMosaic();
}

MainWindow::~MainWindow()
{
	//EMPTY
}