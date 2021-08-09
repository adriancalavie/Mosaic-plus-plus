#include "mainwindow.h"

void MainWindow::SelectBasePicturesFolder()
{
	QFileDialog dialog(this);
	const QString aux = QFileDialog::getExistingDirectory(this,
		tr("Open Directory"),
		"D:/",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	ui->textEditBasePictureFolder->setText(aux);
}

void MainWindow::SelectFolderForResult()
{
	QFileDialog dialog(this);
	const QString aux = QFileDialog::getExistingDirectory(this,
		tr("Open Directory"),
		"D:",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	ui->textEditFolderResultForPicture->setText(aux);
}

void MainWindow::SelectPictureForMosaic()
{
	const QString aux = QFileDialog::getOpenFileName(this,
		tr("Open Directory"),
		R"(D:\)",
		"*.jpg; *.png");

	ui->textEditPictureForMosaic->setText(aux);
	const QPixmap pixmap(aux);
	ui->labelOriginalPicture->setPixmap(pixmap.scaled(ui->labelOriginalPicture->width(),
		ui->labelOriginalPicture->height(),
		Qt::KeepAspectRatio));

	qw->GetUI()->labelOriginalPicture->setPixmap(pixmap.scaled(
		qw->GetUI()->labelOriginalPicture->width(),
		qw->GetUI()->labelOriginalPicture->height(),
		Qt::KeepAspectRatio));
}

bool MainWindow::StartMosaic()
{
	stopwatch timeMosaic;
	timeMosaic.tick();

	if (ui->textEditPictureForMosaic->toPlainText().isEmpty())
	{
		Errors(Data::Errors::PICTURE_FOR_MOSAIC_EMPTY);
		return false;
	}

	if (!ui->textEditBasePictureFolder->toPlainText().isEmpty())
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

	ui->waitLabel->setText(QString::fromStdString("Time elapsed: " + convertor + " seconds"));

	return true;
}



void MainWindow::ActionHelp() const
{

	QLabel* label = new QLabel(tr("Name:"));

	QHBoxLayout* layout = new QHBoxLayout();

	const QString textLayout(QString::fromStdString(Data::Info::GENERAL_HELP));
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

void MainWindow::CheckQuadMosaic() const
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

void MainWindow::ShowDetailsQuad() const
{
	qw->show();
	ui->buttonShowDetailsQuad->setVisible(false);
}


void MainWindow::MakeMosaic()
{
	const std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	const cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

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

	auto method = [s = st->GetUI()]{
		if (s.get()->radioButtonMethodCropping->isChecked())
			return Method::CROPPING;
		return Method::RESIZING;
	};
	auto algorithm = [s = st->GetUI()]{
		if (s->radioButtonEuclidianAlgorithm->isChecked())
			return Algorithm::EUCLIDEAN;
		return Algorithm::RIEMERSMA;
	};

	cv::Mat output = Mosaic::MakeMosaic(input,
		basePictures,
		method(),
		typeCell(),
		ui->spinBoxCellSize->value(),
		algorithm(),
		st->GetUI()->checkBoxBlendingPicture->isChecked());

	if (output.empty())
	{
		Errors(Data::Errors::ANOTHER_ERROR);
		return;

	}

	std::string folderForResultPathString = ui->textEditFolderResultForPicture->toPlainText().toStdString();

	auto extension = [s = st->GetUI()]{
		if (s->extensionJPG->isChecked())
			return ".jpg";
		return ".png";
	};

	auto outputPath = [s = st->GetUI(), &extension, u = *ui]{
		if (u.textEditFolderResultForPicture->toPlainText().toStdString().empty())
		{
			return Data::Defaults::PATH_RESULT_IMAGE +
				s->textEditNameResultPicture->toPlainText().toStdString() +
				extension();
		}

		return u.textEditFolderResultForPicture->toPlainText().toStdString() +
			"/" +
			s->textEditNameResultPicture->toPlainText().toStdString() +
			extension();
	};

	if (!st->GetUI()->checkBoxOriginalSize->isChecked())
	{
		output = Mosaic::pt::Resize(output, st->GetUI()->spinBoxWidthResultPicture->value(),
			st->GetUI()->spinBoxHeightResultPicture->value());
	}

	cv::imwrite(outputPath(), output);

	const QPixmap mosaic(QString::fromStdString(outputPath()));
	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::KeepAspectRatio));
}

void MainWindow::MakeQuadMosaic()
{
	const std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	const cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

	Mosaic::imgPair res = Mosaic::MakeQuadTree(basePictures,
		input,
		st->GetUI()->checkBoxBlendingPicture->isChecked(),
		st->GetUI()->sliderThreshold->value() / 500.0,
		st->GetUI()->sliderMinimumSize->value(),
		st->GetUI()->checkBoxDetailsQuad->isChecked());

	auto extension = [s = st->GetUI()]{
		if (s->extensionJPG->isChecked())
			return ".jpg";
		return ".png";
	};

	auto outputPath = [s = st->GetUI(), &extension, u = *ui]{
		if (u.textEditFolderResultForPicture->toPlainText().toStdString().size() == 0)
		{
			return Data::Defaults::PATH_RESULT_IMAGE +
				s->textEditNameResultPicture->toPlainText().toStdString() +
				extension();
		}
		return u.textEditFolderResultForPicture->toPlainText().toStdString() +
			"/" +
			s->textEditNameResultPicture->toPlainText().toStdString() +
			extension();
	};

	if (res.second.value().empty())
	{
		Errors(Data::Errors::ANOTHER_ERROR);
		return;
	}

	imwrite(outputPath(), res.first.value());
	if (st->GetUI()->checkBoxDetailsQuad->isChecked())
		imwrite(Data::Defaults::PATH_RESULT_IMAGE + "quad" + extension(), res.second.value());

	const QPixmap mosaic(QString::fromStdString(outputPath()));

	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::KeepAspectRatio));

	if (st->GetUI()->checkBoxDetailsQuad->isChecked())
	{
		qw->GetUI()->labelMosaicPicture->setPixmap(mosaic.scaled(qw->GetUI()->labelMosaicPicture->width(),
			qw->GetUI()->labelMosaicPicture->height(), Qt::KeepAspectRatio));

		const QPixmap quadImage(QString::fromStdString(Data::Defaults::PATH_RESULT_IMAGE + "quad" + extension()));

		qw->GetUI()->labelQuadImage->setPixmap(quadImage.scaled(qw->GetUI()->labelQuadImage->width(),
			qw->GetUI()->labelQuadImage->height(), Qt::KeepAspectRatio));
		ui->buttonShowDetailsQuad->setVisible(true);
	}

}

void MainWindow::Errors(const std::string& message) const
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
	connect(ui->buttonBasePicturesFolder,
		&QPushButton::released,
		this,
		&MainWindow::SelectBasePicturesFolder);

	connect(ui->buttonPictureForMosaic,
		&QPushButton::released,
		this,
		&MainWindow::SelectPictureForMosaic);

	connect(ui->buttonFolderForTheResult,
		&QPushButton::released,
		this,
		&MainWindow::SelectFolderForResult);

	connect(ui->buttonMakeMosaic,
		&QPushButton::released,
		this,
		&MainWindow::StartMosaic);

	connect(ui->actionExit,
		&QAction::triggered,
		this,
		[] {exit(-1); });

	connect(ui->actionHelp,
		&QAction::triggered,
		this,
		&MainWindow::ActionHelp);

	connect(ui->actionSettings,
		&QAction::triggered,
		this,
		[this] {st->show(); });

	connect(ui->checkBoxQuadTree,
		&QPushButton::released,
		this,
		&MainWindow::CheckQuadMosaic);

	connect(ui->buttonShowDetailsQuad,
		&QPushButton::released,
		this,
		&MainWindow::ShowDetailsQuad);

	std::thread([&]() {basePictures.AddBasePicturesMosaic(); }).detach();

	//this->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");
	help->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");
	ui->waitLabel->setWordWrap(true);
	ui->buttonShowDetailsQuad->setVisible(false);


}


