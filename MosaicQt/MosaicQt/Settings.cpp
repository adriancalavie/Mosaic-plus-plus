#include "Settings.h"

std::shared_ptr<Ui::Settings> Settings::GetUI()
{
	return this->ui;
}

Settings::Settings(std::shared_ptr<QWidget> parent)
	: QMainWindow(parent.get()),
	ui(new Ui::Settings)
{
	ui->setupUi(this);
}

Settings::~Settings()
{
	
}
