#ifndef MATERIALSDIALOG_H
#define MATERIALSDIALOG_H

#include <QDialog>
#include<QStandardItemModel>
#include<QVBoxLayout>
#include <QCheckBox>


namespace Ui {
class MaterialsDialog;
}

class MaterialsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialsDialog(QWidget *parent = nullptr,QStandardItemModel* matmodel=0,QStandardItemModel* model=0);
    ~MaterialsDialog();
    QStringList list;
    QStringList IDlist;
    QList<bool> states;


private slots:
    void on_okPB_clicked();

    void on_cancelPB_clicked();

    void on_selectallPB_clicked();

    void on_deselectallPB_clicked();

private:
    Ui::MaterialsDialog *ui;
    QVBoxLayout *layout;
    QList<QCheckBox*> checkList;
};

#endif // MATERIALSDIALOG_H
