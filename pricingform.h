#ifndef PRICINGFORM_H
#define PRICINGFORM_H

#include <QWidget>
#include<QStandardItemModel>
#include"foodform.h"

namespace Ui {
class pricingForm;
}

class pricingForm : public QWidget
{
    Q_OBJECT

public:
    explicit pricingForm(QWidget *parent = nullptr);
    ~pricingForm();

private slots:
    void on_FoodselectPB_clicked();


    void CheckStateChanged(QModelIndex ind);

    void on_retrieveDataPB_clicked();
    void valuechanged(QString str);
    void pricechanged(QString str);
    void setnewValue(int row, double price);

//    void on_profitPercent_editingFinished();

    void on_sellPrice_editingFinished();

//    void on_Profit_editingFinished();

    void on_calculatePB_clicked();

    void on_saveReplacePB_clicked();

    void on_saveNewNamePB_clicked();

private:
    Ui::pricingForm *ui;
    void fetchData(QString foodId);
    QStandardItemModel* model;
    QStringList unit2gram(QString unitprice,QString unit);
    QString currentId;
    void updateValues_Prices();
    void showEvent(QShowEvent *event);

};

#endif // PRICINGFORM_H
