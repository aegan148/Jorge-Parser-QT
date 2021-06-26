#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "read_XML.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern TinyClassObj* parserOBJ;
extern doc_Razdel* xlsxObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_go_Button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
