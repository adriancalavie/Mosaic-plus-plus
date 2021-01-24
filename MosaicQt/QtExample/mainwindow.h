#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QMessageBox>
#include <QPicture>
#include <QAction>
#include <qboxlayout.h>
#include <QStringListModel>
#include <QProcess>
#include <QFileDialog>
#include <qtimer.h>
#include <qthread.h>

#include "ui_mainwindow.h"

#include <fstream>
#include <iostream>
#include <qlabel.h>
#include <windows.h>
#include <fstream>
#include <thread>
#include <atomic>

//#include "..\Mosaic\Data.h"
//#include "..\Mosaic\Data.cpp"
//#include "..\Mosaic\BasePictures.h"
//#include "..\Mosaic\BasePictures.cpp"
//#include "..\Mosaic\Mosaic.h"
//#include "..\Mosaic\Mosaic.cpp"

#include "Mosaic.h"
#include "StopWatch.h"
#include "QMosaicThread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<QWidget> parent = Q_NULLPTR);
    ~MainWindow();
private:
    
    std::unique_ptr<Ui::MainWindow> ui = std::make_unique<Ui::MainWindow>();
    std::unique_ptr<QDialog> help = std::make_unique<QDialog>();
    std::unique_ptr<QMessageBox> error = std::make_unique<QMessageBox>();
   /* std::unique_ptr<QThread> thread;*/

    /*void setupConnections();*/
private slots:
    std::string selectBasePicturesFolder();
    std::string selectPictureForMosaic();
    std::string selectFolderForResult();
    bool startMosaic();
    void actionExit();
    void actionHelp();
    void incrementProgressBar();
};
