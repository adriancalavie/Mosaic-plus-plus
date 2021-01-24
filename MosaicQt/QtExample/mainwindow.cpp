#include "mainwindow.h"

//void MainWindow::setupConnections()
//{
//	
//
//	//// Slots and signals
//	//// Connect buttons to slots
//	///*connect(ui->buttonToggleWorker, SIGNAL(clicked()), this, SLOT(setThreadState()));*/
//
//	///*connect(ui->spinBoxWorkAmount, SIGNAL(valueChanged(int)), this, SLOT(setWorkAmount(int)));
//	//connect(ui->sliderWorkSpeed, SIGNAL(valueChanged(int)), this, SLOT(setWorkSpeed(int)));*/
//
//	//// Create thread, worker and timer
//	//thread = std::make_unique<QThread>();
//	//// Important that both the worker and timer are NOT members of this widget class otherwise thread affinity will not change at all!
//	//QMosaicThread* worker = new QMosaicThread();
//	//QTimer* timer = new QTimer();
//	//timer->setInterval(0);  // Timer's inteveral set to 0 means that timer will trigger an event as soon as there are no other events to be processed
//
//	//// Connect worker to widget and vice verser (buttons, progressBarWork)
//	//// Connect timer to worker
//	//connect(worker, SIGNAL(sendThreadStatus(QString)), this, SLOT(receiveThreadStatus(QString)));
//	//connect(worker, SIGNAL(sendFinished()), this, SLOT(receiveFinished()));
//	//connect(worker, SIGNAL(sendProgress(int, int)), this, SLOT(receiveProgress(int, int)));
//
//	//connect(this, SIGNAL(sendWorkAmount(int)), worker, SLOT(receiveWorkAmount(int)));
//	//connect(this, SIGNAL(sendWorkSpeed(int)), worker, SLOT(receiveWorkSpeed(int)));
//	//connect(this, SIGNAL(toggleThread()), worker, SLOT(toggle()));
//
//	//connect(timer, SIGNAL(timeout()), worker, SLOT(doWork()));
//	//connect(thread.get(), SIGNAL(started()), timer, SLOT(start()));
//
//	//// Mark timer and worker for deletion ones the thread is stopped
//	//connect(thread.get(), SIGNAL(finished()), worker, SLOT(deleteLater()));
//	//connect(thread.get(), SIGNAL(finished()), timer, SLOT(deleteLater()));
//	//connect(thread.get(), SIGNAL(finished()), thread.get(), SLOT(deleteLater()));
//
//
//	//// Start timer and move to thread
//	//timer->moveToThread(thread.get());
//
//	//// Move worker to thread
//	//worker->moveToThread(thread.get());
//
//	//// Send initial work amount and speed to worker
//	///*emit sendWorkAmount(ui->spinBoxWorkAmount->value());
//	//emit sendWorkSpeed(ui->sliderWorkSpeed->value() * 10);*/
//
//	//// Start main event loop of thread
//	//thread->start();
//
//}

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
	static int value = 0;
	auto test1 = [u = *ui, &value = value]{
		u.progressBarMosaic->setValue(value);
		value++;
	};
	std::thread t1{ test };
	std::thread t2{ test1 };
	t1.join();
	t2.join();

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
	/*ui->progressBarMosaic->setValue(100);*/
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

void MainWindow::incrementProgressBar()
{
	//static int value = 0;
	///*ui->progressBarMosaic->setValue(value);
	//value++;*/

	//auto increment = [this] {
	//	while (ui->progressBarMosaic->value() <=100)
	//	{
	//		ui->progressBarMosaic->setValue(value++);
	//		std::cout << "Increased progress bar\n";
	//	}
	//	return;
	//};

	///*auto increaseValue = [] {
	//	value++;
	//	std::cout << "Increased value\n";
	//	if (value > 100)
	//		return;
	//};*/

	///*std::thread t1{ increment };*/
	//QThread* thread = new QThread();

	//thread->create(increment);
	//thread->start();

	//thread->quit();
	///*t1.join();*/
	

	while (count <= 100)
	{
		ui->progressBarMosaic->setValue(count++);
		std::cout << "added\n";
	}
	count = 0;
}

MainWindow::MainWindow(std::unique_ptr<QWidget> parent) :
	QMainWindow(parent.get()),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->buttonBasePicturesFolder, &QPushButton::released, this, &MainWindow::selectBasePicturesFolder);
	connect(ui->buttonPictureForMosaic, &QPushButton::released, this, &MainWindow::selectPictureForMosaic);
	connect(ui->buttonFolderForTheResult, &QPushButton::released, this, &MainWindow::selectFolderForResult);
	/*connect(ui->buttonMakeMosaic, &QPushButton::released, this, &MainWindow::startMosaic);*/
	connect(ui->buttonMakeMosaic, &QPushButton::released, this, &MainWindow::incrementProgressBar);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::actionExit);
	connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::actionHelp);
	//connect(/*SENDER*/, /*SIGNAL*/, this, /*INCREMENT PROGRESSBAR*/);

	/*setupConnections();*/
}

MainWindow::~MainWindow()
{
	//EMPTY
}

