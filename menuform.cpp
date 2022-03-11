#include "menuform.h"
#include "ui_menuform.h"
#include<QMessageBox>
#include<QDebug>
#include"changenamedialog.h"
#include<QPainter>

QStandardItemModel* foodsmodel;


MenuForm::MenuForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuForm)
{
    ui->setupUi(this);

    foodsmodel=new QStandardItemModel(this);
    ui->tableView->setModel(foodsmodel);

    foodsmodel->setHorizontalHeaderItem(0,new QStandardItem("Food Name"));
    foodsmodel->setHorizontalHeaderItem(1,new QStandardItem("Price"));
    foodsmodel->setHorizontalHeaderItem(2,new QStandardItem("FoodID"));
    ui->tableView->setColumnHidden(2,1);
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showFood(QModelIndex)));


    fetchData();

}

MenuForm::~MenuForm()
{
    delete ui;
}

void MenuForm::fetchData()
{
    FooDForm foodform;
    while (foodsmodel->rowCount()>0) {
        foodsmodel->removeRow(foodsmodel->rowCount()-1);
    }

    QSqlQuery qry("SELECT * FROM Foods");
    qry.exec();
    QList<QStandardItem*> dataList;
    QStandardItem* item=new QStandardItem;
    while (qry.next()) {
        QString foodid=qry.value(0).toString();
        QString name=qry.value(1).toString();

        item->setEditable(0);
        item->setData(name,Qt::EditRole);
        dataList<<item->clone();

        QString str=foodform.readDB(foodid,1);
        item->setData(str,Qt::EditRole);
        dataList<<item->clone();

        item->setData(foodid,Qt::EditRole);
        dataList<<item->clone();

        foodsmodel->insertRow(foodsmodel->rowCount(),dataList);
        dataList.clear();
    }
    delete item;
}

void MenuForm::on_makeFood_clicked()
{
    FooDForm* foodform=new FooDForm(0);
    foodform->show();

}


void MenuForm::showFood(QModelIndex index)
{
    QString foodid=foodsmodel->item(index.row(),2)->data(Qt::EditRole).toString();
    FooDForm* foodform=new FooDForm(0,foodid);
    foodform->setReadOnly(1);
    foodform->show();

}

void MenuForm::on_changeMat_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Please select one entry.");
        return;
    }

    QString foodid=foodsmodel->item(index.row(),2)->data(Qt::EditRole).toString();
    FooDForm* foodform=new FooDForm(0,foodid);
    foodform->show();
}


void MenuForm::on_deleteFood_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Please select one entry.");
        return;
    }

    QString name=foodsmodel->item(index.row(),0)->data(Qt::EditRole).toString();
    int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                   QString("Are you sure you want to delete '%1'?").arg(name),
                                   QMessageBox::Yes | QMessageBox::No);

    if(ret==QMessageBox::No) return;


    QString foodid=foodsmodel->item(index.row(),2)->data(Qt::EditRole).toString();
    QSqlQuery qry;
    qry.prepare("DELETE FROM Foods WHERE FoodId=:foodid;");
    qry.bindValue(":foodid",foodid);
    bool ok=qry.exec();
    if(ok){
        foodsmodel->removeRow(index.row());

        QSqlQuery qry2;
        qry2.prepare("DELETE FROM FoodMats WHERE FoodId=:foodid;");
        qry2.bindValue(":foodid",foodid);
        ok=qry2.exec();
    }

    if(ok){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Successfully done.");
        return;
    }
    else{
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Delete operation failed!");
        return;
    }
}

void MenuForm::on_changeName_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Please select one entry.");
        return;
    }

    ChangeNameDialog* dlg=new ChangeNameDialog(0,foodsmodel->item(index.row(),0)->data(Qt::EditRole).toString(),foodsmodel);

    if(dlg->exec()){
        QString foodid=foodsmodel->item(index.row(),2)->data(Qt::EditRole).toString();
        QSqlQuery qry2;
        qry2.prepare("UPDATE Foods SET FoodName=:name WHERE FoodId=:foodid;");
        qry2.bindValue(":foodid",foodid);
        qry2.bindValue(":name",dlg->foodName);
       if(qry2.exec()){
           fetchData();//###
           QMessageBox::information(this,tr("Food Pri€er"),
                                    "Successfully done.");
       }
    }
    delete dlg;
}

void MenuForm::on_duplicatePB_clicked()
{
    QModelIndex index=ui->tableView->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Please select one entry.");
        return;
    }

    QString oldname=foodsmodel->item(index.row(),0)->data(Qt::EditRole).toString();
    QString oldId=foodsmodel->item(index.row(),2)->data(Qt::EditRole).toString();
    ChangeNameDialog* dlg=new ChangeNameDialog(0,oldname,foodsmodel);

    if(dlg->exec()){
        bool ok=0;
        QSqlQuery qry1;
        qry1.prepare("INSERT INTO Foods(FoodName) VALUES(:name);");
        qry1.bindValue(":name",dlg->foodName);
//        qry1.bindValue(":price",ui->priceout->text());
        ok=qry1.exec();

        QSqlQuery qry11;
        qry11.prepare("SELECT * FROM Foods WHERE FoodName=:name");
        qry11.bindValue(":name",dlg->foodName);
        ok=qry11.exec();

        QString foodid;
        while (qry11.next()) {
            ok=1;
            foodid = qry11.value(0).toString();
        }

        QSqlQuery qry2;
        qry2.prepare("SELECT * FROM FoodMats WHERE FoodId=:foodid");
        qry2.bindValue(":foodid",oldId);
        ok=qry2.exec();

        while (qry2.next()) {
            QString matid = qry2.value(2).toString();
            QString value = qry2.value(3).toString();
            QSqlQuery qry3;
            qry3.prepare("INSERT INTO FoodMats(FoodId,MatId,Value) VALUES(:foodid,:matid,:value);");
            qry3.bindValue(":foodid",foodid);
            qry3.bindValue(":matid",matid);
            qry3.bindValue(":value",value);
            ok=qry3.exec();
        }

        if(!ok){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "Operation failed!");
            return;
        }
        else{
            fetchData();//###
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "Successfully done.");
        }

    }
    delete dlg;
}

void MenuForm::showEvent(QShowEvent *event)
{
    fetchData();
    QWidget::showEvent(event);
}

//-----------------------------------------------------------
BtnFood::BtnFood(QWidget *parent):
    QPushButton(parent)
{
    brush=QBrush(Qt::white,Qt::SolidPattern);
    pen.setColor(Qt::white);
    pen2.setColor(Qt::black);
    fnt.setFamily("Helvetica [Cronyx]");
    fnt.setBold(1);
    fnt.setPixelSize(15);
}

void BtnFood::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int w=2;
    painter.setBrush(brush);
    painter.setPen(pen);
    QRect newrect=QRect(w,w,this->rect().width()-2*w,this->rect().height()-2*w);
    QRect newrect2=QRect(w,w,this->rect().width()/1.35,this->rect().height()-2*w);
    painter.drawRect(this->rect());
    painter.setPen(pen2);
    painter.setFont(fnt);
    painter.drawText(newrect2, Qt::AlignVCenter|Qt::AlignRight, this->text());
    //    painter.drawPixmap(w,w,newrect.width()/5,newrect.height(),this->icon().pixmap(newrect.width(),newrect.height()));
    painter.drawPixmap(this->rect().width()/1.3,w,newrect.width()/5,newrect.height(),this->icon().pixmap(newrect.width()/5,newrect.height()));


}

void BtnFood::enterEvent(QEvent *event)
{
    brush.setColor(QColor(Qt::green).lighter(170));
//     pen.setColor(Qt::blue);
    update();
}

void BtnFood::leaveEvent(QEvent *event)
{
    brush.setColor(QColor(Qt::white).lighter(170));
//     pen.setColor(Qt::blue);
    update();
}

void BtnFood::mousePressEvent(QMouseEvent *e)
{
    QPushButton::mousePressEvent(e);
}







