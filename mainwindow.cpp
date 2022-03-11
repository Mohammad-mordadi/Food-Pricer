#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDir>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent, bool active)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->logo->setStyleSheet("border-radius:20px;\nborder-image: url(\\logo.png);");

   // border-radius:20px;\nborder-image: url(:/img/logo.png);


    ui->widget->setEnabled(active);

    bool ok=0;
    if(active){
        QDir dir2;
        QString DBpath=dir2.currentPath();

        //---------------------------------------------------------------make sqlite3.exe file
        bool res=QFile::copy(":/apps/sqlite3.exe",DBpath+"/sqlite3.exe");

        QDir dir(DBpath);
        QFileInfoList list = dir.entryInfoList();
        for (int i=0;i<list.size();i++) {//-------------------find databse
            if(list.at(i).fileName()=="FoodDB.db"){
                ok=1;
                break;
            }
        }


        if(!ok){
            res=QFile::copy(":/apps/FoodDB.db",DBpath+"/FoodDB.db");
            QFile(DBpath+"/FoodDB.db").setPermissions(QFileDevice::WriteUser);
            qDebug()<<res;
        }


        //-----------------------------------------------------connect to the databse
//        db=QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName(DBpath+"/FoodDB.db");

//        ok=db.open();

    }

    if(!active){
        ui->statusbar->showMessage("Not Connected!",0);
    }
    else
        ui->statusbar->showMessage("Connected.",0);

    if(ok){//-----------------------------------------------create tables(if not exist)
        QSqlQuery qry1("CREATE TABLE [Materials] (\
                       [MatId] INTEGER  NOT NULL PRIMARY KEY,\
                       [Material] NVARCHAR(100)  NULL,\
                       [Unit] NVARCHAR(10)  NULL,\
                       [Price] INTEGER NULL\
                       );  ");
        qry1.exec();

        QSqlQuery qry2("CREATE TABLE [Foods] (\
                       [FoodId] INTEGER  NOT NULL PRIMARY KEY,\
                       [FoodName] NVARCHAR(100)  NULL\
                       );  ");

        qry2.exec();

        QSqlQuery qry3("CREATE TABLE [FoodMats] (\
                       [id] INTEGER  NOT NULL PRIMARY KEY,\
                       [FoodId] INTEGER  NOT NULL,\
                       [MatId] INTEGER  NOT NULL,\
                       [Value] REAL DEFAULT 0\
                       );  ");

        qry3.exec();

        QSqlQuery qry4("CREATE TABLE [Passwords] (\
                       [id] INTEGER  NOT NULL PRIMARY KEY,\
                       [level] NVARCHAR(100)  NULL,\
                       [pass] STRING   NULL\
                       );  ");

        qry4.exec();


    }

    //--------------------------------------------------------------------
    matForm=new materialsForm(0);
    matForm->fetchData();
    ui->gridLayout->addWidget(matForm,0,0);
    matForm->hide();


    menuForm=new MenuForm(0);
    ui->gridLayout->addWidget(menuForm,0,0);
    menuForm->hide();

    pricingform=new pricingForm(0);
    ui->gridLayout->addWidget(pricingform,0,0);
    pricingform->hide();

    passform=new passwordForm(0);
    ui->gridLayout->addWidget(passform,0,0);
    passform->hide();


    connect(ui->PBfood,SIGNAL(btnClicked()),this,SLOT(selectsubWindow()));
    connect(ui->PBprice ,SIGNAL(btnClicked()),this,SLOT(selectsubWindow()));
    connect(ui->PBmaterial ,SIGNAL(btnClicked()),this,SLOT(selectsubWindow()));
    connect(ui->PBpassword ,SIGNAL(btnClicked()),this,SLOT(selectsubWindow()));

    ui->PBfood->btnClicked();
    Button::setselected(ui->PBfood);

    //-----------------------------------------------------------------------------------------

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::selectsubWindow()
{


    QString txt="";
    if(this->sender()->objectName().contains("Form",Qt::CaseInsensitive)){
        txt=this->sender()->objectName();
    }
    else{
        QPushButton* pb=static_cast<QPushButton*>(this->sender());
        txt= pb->text();
    }



    QObjectList lst=ui->widget->children();
    for (int i=0;i<lst.size();i++){
        QWidget* w=qobject_cast<QWidget*>(lst.at(i));
        if(w)
            w->hide();
    }

    if(txt.contains("ingredients",Qt::CaseInsensitive)){
        matForm->show();
    }
    else if(txt.contains("menu",Qt::CaseInsensitive)){
        menuForm->show();
    }
    else if(txt.contains("FooDForm",Qt::CaseInsensitive)){//triggered by signal showMakeFoodForm()
        menuForm->show();
    }
    else if(txt.contains("pricing",Qt::CaseInsensitive)){
        pricingform->show();
    }
    else if(txt.contains("password",Qt::CaseInsensitive)){
        passform->show();
    }
}


