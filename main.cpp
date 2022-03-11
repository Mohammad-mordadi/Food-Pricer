#include "mainwindow.h"
#include <QMessageBox>
#include<QFile>
#include <QApplication>
#include<QDebug>
#include<QDate>
#include"enterdialog.h"
#include<QSqlDatabase>
#include<QDir>
#include"qjsonwebtoken.h"
#include<QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db;
    //---------------------------------------------open database for whole application
    QDir dir2;
    QString DBpath=dir2.currentPath();
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DBpath+"/FoodDB.db");
    db.open();
    //--------------------------------------------------------------------------------

    //--------------------------------------------check trial period
    QSettings settings_cdate("_myapp_FP");
    QSettings settings_hashdate("a_software_tok_en");

    QString cdate=settings_cdate.value("cdate").toString();
    QJsonWebToken m_jwtObj;

    if(cdate.isEmpty()){//...........................................first run
        QString date=QDate::currentDate().toString("dd.MM.yyyy");
        qDebug()<<date;

        cdate=date;
        m_jwtObj.setSecret("app_trial_period");
        m_jwtObj.appendClaim("iss", "foodpricer");
        m_jwtObj.appendClaim("uuid", cdate);
        QString token=m_jwtObj.getToken();

        settings_cdate.setValue("cdate",cdate);
        settings_hashdate.setValue("hashdate",token);
    }
    else{
        m_jwtObj.setSecret("app_trial_period");
        m_jwtObj.appendClaim("iss", "foodpricer");
        m_jwtObj.appendClaim("uuid", cdate);
        QString token=m_jwtObj.getToken();
        QString hashdate=settings_hashdate.value("hashdate").toString();
        if(hashdate!=token){
            QMessageBox::warning(0,"FOOD PRICER","پایان زمان نسخه آزمایشی");
            return 0;
        }
    }

    QDate startdate=QDate::fromString(cdate,"dd.MM.yyyy");
    QDate finaldate=QDate::currentDate();


    EnterDialog* dlg=new EnterDialog;//login dialog

    if(!dlg->registered){
        qDebug()<<startdate<<finaldate<<startdate.daysTo(finaldate);//<<QDate::currentDate().toString("dd.MM.yyyy");;
        if(startdate.daysTo(finaldate)>30){
            QMessageBox::warning(0,"FOOD PRICER","پایان زمان نسخه آزمایشی\nلطفا رجیستر کنید");
            return 0;
        }
    }

    //--------------------------------------------------log in dialog
    bool ok=dlg->exec() | 1;


//    QFile file1("C:/Users/Mohammad/Desktop/mm.txt"); //resource files are read-only
//    file1.open(QIODevice::WriteOnly);
//    file1.write("hellloooooo");
//    file1.close();



    ok&=dlg->registered;//if logged in and registered

    if(!ok)
        db.close();

    MainWindow w(0,ok);
    delete dlg;
    w.show();
    return a.exec();
}
