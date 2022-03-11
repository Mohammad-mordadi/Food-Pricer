#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSqlQuery>
#include<QSqlError>
#include<QSqlDatabase>
#include"materialsform.h"
#include"menuform.h"
#include<QProcess>
#include"pricingform.h"
#include"passwordform.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, bool active=0);
    ~MainWindow();

private slots:
    void selectsubWindow();

private:
    Ui::MainWindow *ui;
    QProcess *myProcess;
    QSqlDatabase db;
    materialsForm* matForm;
    MenuForm* menuForm;
    pricingForm* pricingform;
    passwordForm* passform;
};
#endif // MAINWINDOW_H
