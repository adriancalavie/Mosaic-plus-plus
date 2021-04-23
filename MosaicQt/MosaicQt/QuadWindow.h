#pragma once

#include <QMainWindow>
#include <ui_quadwindow.h>

class QuadWindow : public QMainWindow
{
	Q_OBJECT
private:
	std::shared_ptr<Ui::QuadWindow> ui = std::make_shared<Ui::QuadWindow>();

public:
	QuadWindow(std::shared_ptr<QWidget> parent = Q_NULLPTR);
	~QuadWindow();

	std::shared_ptr<Ui::QuadWindow> GetUI();
};
