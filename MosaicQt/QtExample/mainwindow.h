#pragma once

#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow();
private:
    std::unique_ptr<Ui::MainWindow> ui = std::make_unique<Ui::MainWindow>();
};
