#ifndef MENUFORM_H
#define MENUFORM_H

#include <QWidget>
#include<QStandardItemModel>
#include"foodform.h"
#include<QPushButton>
#include<QPen>
#include<QBrush>
#include<QSqlQuery>

namespace Ui {
class MenuForm;
}

class MenuForm : public QWidget
{
    Q_OBJECT

public:
    explicit MenuForm(QWidget *parent = nullptr);
    ~MenuForm();
    void fetchData();

private slots:
    void on_makeFood_clicked();
    void showFood(QModelIndex index);

    void on_deleteFood_clicked();

    void on_changeName_clicked();

    void on_changeMat_clicked();

    void on_duplicatePB_clicked();

private:
    Ui::MenuForm *ui;
    void showEvent(QShowEvent *event);


signals:
};

//----------------------------------------------------------------
class BtnFood : public QPushButton
{

    Q_OBJECT
public:
    explicit BtnFood(QWidget* parent=0);
private:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    QBrush brush,brush2;
    QPen pen;
    QPen pen2;
    QPen txtpen;
    QFont fnt;
    int w;


};

#endif // MENUFORM_H
