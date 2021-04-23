#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include "ui_settings.h"

class Settings : public QMainWindow
{
	Q_OBJECT

private:
	std::shared_ptr<Ui::Settings> ui = std::make_shared<Ui::Settings>();

public:
	std::shared_ptr<Ui::Settings> GetUI();

public:
	Settings(std::shared_ptr<QWidget> parent = Q_NULLPTR);
	~Settings();
};
