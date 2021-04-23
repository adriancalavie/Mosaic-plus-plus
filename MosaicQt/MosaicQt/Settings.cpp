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
	connect(ui->sliderThreshold, &QSlider::valueChanged, this, &Settings::ChangeValueTrehhold);
	connect(ui->sliderMinimumSize, &QSlider::valueChanged, this, &Settings::ChangeValueMinimumSize);
}

Settings::~Settings()
{
	
}

void Settings::ChangeValueTrehhold()
{
	ui->labelValueThreshold->setText(QString::fromStdString(std::to_string(ui->sliderThreshold->value())));
}

void Settings::ChangeValueMinimumSize()
{
	ui->labelValueMinimumSize->setText(QString::fromStdString(std::to_string(ui->sliderMinimumSize->value())));

}
