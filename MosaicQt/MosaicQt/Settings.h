#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include "ui_settings.h"

class Settings : public QMainWindow
{
	Q_OBJECT

public:
	std::unique_ptr<Ui::Settings> ui = std::make_unique<Ui::Settings>();


public:
	Settings(std::unique_ptr<QWidget> parent = Q_NULLPTR);
	~Settings();
};
