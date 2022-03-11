#ifndef ENTERDIALOG_H
#define ENTERDIALOG_H

#include <QDialog>
#include"qjsonwebtoken.h"


namespace Ui {
class EnterDialog;
}

class EnterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnterDialog(QWidget *parent = nullptr);
    ~EnterDialog();
    bool registered;


private slots:
    void on_registerPB_clicked();

    void on_copyPB_clicked();

    void on_loginPB_clicked();

private:
    Ui::EnterDialog *ui;
    QJsonWebToken m_jwtObj;
    QString token;


};

#endif // ENTERDIALOG_H
