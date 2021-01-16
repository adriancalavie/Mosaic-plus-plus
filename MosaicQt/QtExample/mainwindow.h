#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow();
private:
    std::unique_ptr<Ui::MainWindow> ui = std::make_unique<Ui::MainWindow>();
    std::unique_ptr<QDialog> help = std::make_unique<QDialog>();
    std::unique_ptr<QMessageBox> error = std::make_unique<QMessageBox>();
private slots:
    std::string selectBasePicturesFolder();
    std::string selectPictureForMosaic();
    std::string selectFolderForResult();
    bool startMosaic();
    void actionExit();
    void actionHelp();
};
