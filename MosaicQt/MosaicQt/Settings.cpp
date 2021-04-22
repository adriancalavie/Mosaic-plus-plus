#include "Settings.h"

Settings::Settings(std::unique_ptr<QWidget> parent)
	: QMainWindow(parent.get()),
	ui(new Ui::Settings)
{
	ui->setupUi(this);
}

Settings::~Settings()
{
	
}
