#ifndef PASSWORDFORM_H
#define PASSWORDFORM_H

#include <QWidget>
#include"qjsonwebtoken.h"
#include<QSqlQuery>

namespace Ui {
class passwordForm;
}

class passwordForm : public QWidget
{
    Q_OBJECT

public:
    explicit passwordForm(QWidget *parent = nullptr);
    ~passwordForm();

private slots:
    void on_changePB_clicked();

private:
    Ui::passwordForm *ui;
    QJsonWebToken m_jwtObj;
    QString token;
};

#endif // PASSWORDFORM_H
