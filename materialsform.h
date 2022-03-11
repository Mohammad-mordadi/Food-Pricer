#ifndef MATERIALSFORM_H
#define MATERIALSFORM_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlError>
#include<QStandardItemModel>
#include<QPushButton>
#include<QPen>
#include<QBrush>
#include<QMouseEvent>
#include<QPainter>


namespace Ui {
class materialsForm;
}

class materialsForm : public QWidget
{
    Q_OBJECT

public:
    explicit materialsForm(QWidget *parent = nullptr);
    void fetchData();
    ~materialsForm();

private:
    Ui::materialsForm *ui;
    bool addData(QString name,QString unit,qlonglong price);
    bool removeData(QString name);
    void showEvent(QShowEvent *event);

private slots:
    void showData(QModelIndex index);
    void on_addPB_clicked();
    void on_deletPB_clicked();
};
//----------------------------------------------------------------
class Btn : public QPushButton
{

    Q_OBJECT
public:
    explicit Btn(QWidget* parent=0);
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


#endif // MATERIALSFORM_H
