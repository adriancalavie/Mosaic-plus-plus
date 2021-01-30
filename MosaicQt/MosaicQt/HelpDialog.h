//#include <QFileDialog>
//#include <fstream>
//#include <QMessageBox>
//#include <QPicture>
//#include <QAction>
//#include <windows.h>
//#include <QStringListModel>
//
//
//class HelpDialog : public QDialog
//{
//    Q_OBJECT
//
//public:
//    MyDHelpDialogialog(QWidget* parent = 0);
//
//    QStringList getSettings() { return settings; }
//public slots:
//    int exec();
//    void done(int val);
//    void accept();
//    void reject();
//private:
//    QStringList settings;
//    QPushButton* exportButton;
//    QPushButton* cancelButton;
//};
//
//HelpDialog::MyDialog(QWidget* parent) : QDialog(parent)
//{
//
//
//    setModal(true);
//    setFocusPolicy(Qt::StrongFocus);
//    setFocus();
//    ......
//        exportButton = new QPushButton(tr("Export"));
//    connect(exportButton, SIGNAL(clicked()), this, SLOT(accept()));
//    cancelButton = new QPushButton(tr("Cancel"));
//    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
//    .....
//}
//int MyDialog::exec()
//{
//    activateWindow();
//    this->show();
//    return 1;
//}
//void MyDialog::done(int val)
//{
//    printf("done()\n");
//}
//
//void MyDialog::accept()
//{
//    settings << "setting1" << "setting2" << "ect";
//    printf("Accept.\n");
//    this->hide();
//}
//void ExportSequenceWidget::reject()
//{
//    settings.clear();
//    printf("Reject.\n");
//    this->hide();
//}
