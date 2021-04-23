#include "QuadWindow.h"

QuadWindow::QuadWindow(std::shared_ptr<QWidget> parent)
	: QMainWindow(parent.get()), ui(new Ui::QuadWindow)
{
	ui->setupUi(this);
}

QuadWindow::~QuadWindow()
{
}


std::shared_ptr<Ui::QuadWindow> QuadWindow::GetUI()
{
	return this->ui;
}