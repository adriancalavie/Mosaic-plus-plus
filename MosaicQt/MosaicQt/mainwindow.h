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
#include "StopWatch.hpp"
#include "Settings.h"
#include "QuadWindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<QWidget> parent = Q_NULLPTR);
    ~MainWindow() override = default;

private:
    std::unique_ptr<Ui::MainWindow> ui = std::make_unique<Ui::MainWindow>();
    std::unique_ptr<QWidget> help = std::make_unique<QWidget>();
    std::unique_ptr<QMessageBox> error = std::make_unique<QMessageBox>();
    std::unique_ptr<Settings> st = std::make_unique<Settings>();
    std::unique_ptr<QuadWindow> qw = std::make_unique<QuadWindow>();

private:
    BasePictures basePictures;
    
private slots:
    void SelectBasePicturesFolder();
    void SelectPictureForMosaic();
    void SelectFolderForResult();
    bool StartMosaic();
    void ActionHelp() const;
    void CheckQuadMosaic() const;
    void ShowDetailsQuad() const;
private:
    void MakeMosaic();
    void MakeQuadMosaic();
    void Errors(const std::string& message) const;


};
