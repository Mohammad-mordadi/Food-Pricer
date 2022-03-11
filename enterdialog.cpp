#include "enterdialog.h"
#include "ui_enterdialog.h"
#include<QDateTime>
#include<QProcess>
#include<QSettings>
#include<QDebug>
#include<QMessageBox>
#include<QClipboard>
#include<QDir>
#include<QSqlQuery>

QString defaultPass="123";//"*963./8527410"

EnterDialog::EnterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterDialog)
{
    ui->setupUi(this);

    ui->registeredLBL->setHidden(1);
    registered=0;

    //----------------------------------------------get computer id
    QProcess *myProcess = new QProcess(0);
    QString program = "wmic csproduct get UUID";
    myProcess->start(program);
    myProcess->waitForFinished(1000);
    QString PCID=QString(myProcess->readAllStandardOutput());
    PCID.remove("UUID");
    PCID=PCID.trimmed();
    ui->uuid->setText(PCID);

    //----------------------------------------------make token
    //private key!!!!
    m_jwtObj.setSecret("marzomadin");
    m_jwtObj.appendClaim("iss", "foodpricer");
    m_jwtObj.appendClaim("uuid", PCID);
    token=m_jwtObj.getToken();

    //----------------------------------------------read registers
    QSettings settings("foodpricer");
//    settings.setValue("lickey","salaaaaaaaam");
    QString reglic=settings.value("lickey").toString();

    registered=(reglic==token);
    ui->registeredLBL->setHidden(!registered);
    if(registered){
        ui->toolBox->setCurrentIndex(0);
    }
    else{
        ui->toolBox->setCurrentIndex(1);
    }

}

EnterDialog::~EnterDialog()
{
    delete ui;
}

void EnterDialog::on_registerPB_clicked()
{
    QString lic=ui->license->toPlainText();
    lic=lic.trimmed();
    lic.remove(' ');
    if(token==lic){
        QMessageBox::information(this,"register","registered successfully.");
        ui->registeredLBL->setHidden(0);
        QSettings settings("foodpricer");
        settings.setValue("lickey",token);
    }
    else{
        QMessageBox::information(this,"register","wrong license key.\nregisteration failed.");
        ui->registeredLBL->setHidden(1);
    }
}

void EnterDialog::on_copyPB_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
      clipboard->setText(ui->uuid->text());
}

void EnterDialog::on_loginPB_clicked()
{
    //----------------------------------------------get password from database


    QString password="",level="admin";

    QSqlQuery qry;
    qry.prepare("SELECT * FROM Passwords WHERE level=:level");
    qry.bindValue(":level",level);
    qry.exec();


    //SELECT COUNT (*) FROM Passwords where level="admin"
    while (qry.next()) {
        password = qry.value(2).toString();
    }

    if(password.isEmpty()){//default password
        if(ui->password->text()==defaultPass)//
            accept();
        else{
            QMessageBox::information(this,"login","رمز عبور اشتباه است.");
        }
    }
    else{
        //private key!!!!
        m_jwtObj.setSecret("my_pass_word");
        m_jwtObj.appendClaim("iss", "foodpricer");
        m_jwtObj.appendClaim("uuid", ui->password->text());
        QString enteredpass=m_jwtObj.getToken();
        if(enteredpass==password)
            accept();
        else{
            QMessageBox::information(this,"login","رمز عبور اشتباه است.");
        }

    }
}
