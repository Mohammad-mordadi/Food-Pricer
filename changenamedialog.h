#ifndef CHANGENAMEDIALOG_H
#define CHANGENAMEDIALOG_H

#include <QDialog>
#include<QStandardItemModel>

namespace Ui {
class ChangeNameDialog;
}

class ChangeNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeNameDialog(QWidget *parent = nullptr,QString oldname="",QStandardItemModel* model=0);
    ~ChangeNameDialog();
    QString foodName;

private slots:
    void on_cancelPB_clicked();

    void on_okPB_clicked();

private:
    Ui::ChangeNameDialog *ui;
    QStandardItemModel* mymodel;
};

#endif // CHANGENAMEDIALOG_H
