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
#include <qprogressbar.h>
#include <qtimer.h>
#include <qthread.h>
#include <qwidget.h>
#include <qlineedit.h>
#include "ui_mainwindow.h"

#include <iomanip>
#include <fstream>
#include <iostream>
#include <qlabel.h>
#include <fstream>
#include <string>

#include "Mosaic.h"
#include "StopWatch.h"
#include "Settings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<QWidget> parent = Q_NULLPTR);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> ui = std::make_unique<Ui::MainWindow>();
    std::unique_ptr<QWidget> help = std::make_unique<QWidget>();
    std::unique_ptr<QMessageBox> error = std::make_unique<QMessageBox>();
    std::unique_ptr<Settings> st = std::make_unique<Settings>();

private:
    BasePictures basePictures;
    
private slots:
    std::string SelectBasePicturesFolder();
    std::string SelectPictureForMosaic();
    std::string SelectFolderForResult();
    bool StartMosaic();
    void ActionSettings();
    void ActionExit();
    void ActionHelp();
    void SelectQuadTree();

    void MakeMosaic();
    void MakeQuadMosaic();
};
