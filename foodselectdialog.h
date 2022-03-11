#ifndef FOODSELECTDIALOG_H
#define FOODSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class FoodSelectDialog;
}

class FoodSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FoodSelectDialog(QWidget *parent = nullptr,QStringList list=QStringList(),QStringList listId=QStringList());
    ~FoodSelectDialog();
    QString currentFood;
    QString currentId;

private slots:
    void on_okPB_clicked();

    void on_cancelPB_clicked();

private:
    Ui::FoodSelectDialog *ui;
    void setFoods(QStringList lst);
    QStringList mylistId;

};

#endif // FOODSELECTDIALOG_H
