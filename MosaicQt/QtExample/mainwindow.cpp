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
		ui->labelOriginalPicture->height(), Qt::IgnoreAspectRatio));
	return aux.toStdString();
}

bool MainWindow::StartPressed()
{
	//ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:rgb(255, 80, 83);}");
	bool returned = StartMosaic();
	//ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:#148dfa;}");

	return returned;
}

bool MainWindow::StartMosaic()
{
	//ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:rgb(255, 80, 83);}");
	
	auto errors = [&](QString message) {
		ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:#148dfa;}");

		error->setWindowTitle("Error");
		error->setWindowIcon(QIcon("Pictures\\error.png"));
		error->setText(message);
		error->show();
	};

	std::string pictureForMosaicPathString = ui->textEditPictureForMosaic->toPlainText().toStdString();
	if (pictureForMosaicPathString.size() == 0)
	{
		errors("Incorect path for picture!");
		return false;
	}

	std::string basePicturePathString = ui->textEditBasePictureFolder->toPlainText().toStdString();
	BasePictures basePictures;

	if (!ui->checkBoxBasePictures->isChecked())
		basePictures.AddBasePicturesMosaic();

	if (basePicturePathString.size() > 0)
		basePictures.CreatePictures(basePicturePathString);

	if (basePictures.GetNumberPictures() < 10)
	{
		errors("No base pictures for mosaic!");
		return false;
	}

	cv::Mat input = cv::imread(pictureForMosaicPathString, cv::IMREAD_COLOR);

	if (input.empty())
	{
		errors("Something is wrong with your photo!");
		return false;
	}

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

	cv::Mat output = Mosaic::MakeMosaic(input, basePictures, method(), typeCell(), ui->spinBoxCellSize->value(),
		algorithm(), ui->checkBoxBlendingPicture->isChecked());


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
		output = Mosaic::pt::Resize(output, ui->spinBoxWidthResultPicture->value(), ui->spinBoxHeightResultPicture->value());
	}

	cv::imwrite(outputPath(), output);

	QPixmap mosaic(std::move(QString::fromStdString(outputPath())));
	ui->labelMosaicPicture->setPixmap(mosaic.scaled(ui->labelMosaicPicture->width(),
		ui->labelMosaicPicture->height(), Qt::IgnoreAspectRatio));

	//ui->buttonMakeMosaic->setStyleSheet("QPushButton{border-radius: 10px;font: 20pt \"Century Gothic\";color:#19232D; background:#148dfa;}");

	return true;
}


void MainWindow::ActionExit()
{
	exit(1);
}

void MainWindow::ActionHelp()
{
	//std::unique_ptr<QLabel> labelHelp = std::make_unique<QLabel>();
	//std::unique_ptr<QHBoxLayout> layoutLabelHelp = std::make_unique<QHBoxLayout>();

	//labelHelp->setWindowTitle("Help");

	//QString textLayout(QString::fromStdString(Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP)));
	//labelHelp->setText(textLayout);

	//layoutLabelHelp->addWidget(labelHelp.get());

	//Qt::Alignment alignment;
	//alignment.setFlag(Qt::AlignmentFlag::AlignCenter);
	//layoutLabelHelp->setAlignment(alignment);

	//help->setMinimumWidth(300);
	//help->setMinimumHeight(300);

	//help->setLayout(layoutLabelHelp.get());
	//help->setWindowTitle("ReadMe");
	//help->show();
	//hide();

	/*std::unique_ptr<QLabel> labelHelp = std::make_unique<QLabel>();
	std::unique_ptr<QHBoxLayout> layoutLabelHelp = std::make_unique<QHBoxLayout>();

	setStyleSheet("QWidget{ background-color : rgba( 160, 160, 160, 255); border-radius : 7px;  }");

	QString textLayout(QString::fromStdString(Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP)));
	labelHelp->setText(textLayout);

	layoutLabelHelp->addWidget(labelHelp.get());

	help->setMinimumWidth(300);
	help->setMinimumHeight(300);

	help->setLayout(layoutLabelHelp.get());
	help->setWindowTitle("ReadMe");
	help->show();
	hide();*/

	QLabel* label = new QLabel(tr("Name:"));
	//QLineEdit* lineEdit = new QLineEdit();

	QHBoxLayout* layout = new QHBoxLayout();

	QString textLayout(QString::fromStdString(Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP)));
	label->setText(textLayout);

	Qt::Alignment alignment;
	alignment.setFlag(Qt::AlignmentFlag::AlignCenter);
	layout->setAlignment(alignment);

	help->setMinimumWidth(300);
	help->setMinimumHeight(300);

	layout->addWidget(label);
	help->setLayout(layout);

	help->show();
}

MainWindow::MainWindow(std::unique_ptr<QWidget> parent) :
	QMainWindow(parent.get()),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->buttonBasePicturesFolder, &QPushButton::released, this, &MainWindow::SelectBasePicturesFolder);
	connect(ui->buttonPictureForMosaic, &QPushButton::released, this, &MainWindow::SelectPictureForMosaic);
	connect(ui->buttonFolderForTheResult, &QPushButton::released, this, &MainWindow::SelectFolderForResult);
	connect(ui->buttonMakeMosaic, &QPushButton::released, this, &MainWindow::StartPressed);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::ActionExit);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::ActionHelp);

	//this->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");
	help->setStyleSheet("QWidget{ background-color: #19232D;border: 0px solid #32414B;padding: 0px;color: #F0F0F0;selection - background - color: #1464A0;selection - color: #F0F0F0;}");

}

MainWindow::~MainWindow()
{
	//EMPTY
}

