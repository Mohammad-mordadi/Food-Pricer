#include "passwordform.h"
#include "ui_passwordform.h"
#include<QDebug>
#include<QMessageBox>

extern QString defaultPass;

passwordForm::passwordForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passwordForm)
{
    ui->setupUi(this);
}

passwordForm::~passwordForm()
{
    delete ui;
}

void passwordForm::on_changePB_clicked()
{

    QString password="",level="admin";
    QString oldpass=ui->oldpass->text();
    QString newpass=ui->newpass->text();
    QString newpass2=ui->newpass2->text();

    QSqlQuery qry1;
    qry1.prepare("SELECT COUNT (*) FROM Passwords where level=:level");
    qry1.bindValue(":level",level);
    qry1.exec();
    bool ok=0;
    while (qry1.next()) {
       ok= (qry1.value(0).toInt()>0);
    }


    QSqlQuery qry2;
    qry2.prepare("SELECT * FROM Passwords WHERE level=:level");
    qry2.bindValue(":level",level);
    qry2.exec();


    while (qry2.next()) {
        password = qry2.value(2).toString();
    }

    if(password.isEmpty()){
        if(oldpass!=defaultPass){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "Current password is wrong.");
            return;
        }
    }
    else{
        //private key!!!!
        m_jwtObj.setSecret("my_pass_word");
        m_jwtObj.appendClaim("iss", "foodpricer");
        m_jwtObj.appendClaim("uuid",oldpass);
        QString oldtoken=m_jwtObj.getToken();

        if(oldtoken!=password){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "Current password is wrong.");
            return;
        }
    }

    if(newpass!=newpass2){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "New passwords dont match!");
        return;
    }

    m_jwtObj.setSecret("my_pass_word");
    m_jwtObj.appendClaim("iss", "foodpricer");
    m_jwtObj.appendClaim("uuid",newpass);
    QString newtoken=m_jwtObj.getToken();

    if(!ok){
        QSqlQuery qry3;
        qry3.prepare("INSERT INTO Passwords(level,pass) VALUES(:level,:pass);");
        qry3.bindValue(":level",level);
        qry3.bindValue(":pass",newtoken);
        ok=qry3.exec();
    }
    else{
        QSqlQuery qry3;
        qry3.prepare("UPDATE Passwords SET pass=:pass WHERE level=:level;");
        qry3.bindValue(":level",level);
        qry3.bindValue(":pass",newtoken);
        ok=qry3.exec();
    }

    if(ok){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "The password successfully changed.");
    }
    else{
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Operation failed!");
    }


}
