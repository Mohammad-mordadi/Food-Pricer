#include "pricingform.h"
#include "ui_pricingform.h"
#include"foodselectdialog.h"
#include<QSqlQuery>
#include<QDebug>
#include<QMessageBox>
#include"changenamedialog.h"

extern QStandardItemModel* foodsmodel;

pricingForm::pricingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pricingForm)
{
    ui->setupUi(this);

    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(CheckStateChanged(QModelIndex)));

    model->setHorizontalHeaderItem(0,new QStandardItem("ماده غذایی"));
    model->setHorizontalHeaderItem(1,new QStandardItem("قیمت واحد"));
    model->setHorizontalHeaderItem(2,new QStandardItem("مقدار فعلی"));
    model->setHorizontalHeaderItem(3,new QStandardItem("قیمت"));
    model->setHorizontalHeaderItem(4,new QStandardItem("MatID"));
    model->setHorizontalHeaderItem(5,new QStandardItem("مقدار جدید"));
    model->setHorizontalHeaderItem(6,new QStandardItem("قیمت جدید"));

    ui->tableView->setColumnHidden(4,1);

    valuedelegate* value=new valuedelegate(this,model,inputType::VALUE);
    ui->tableView->setItemDelegateForColumn(5,value);
    connect(value,SIGNAL(finished(QString)),this,SLOT(valuechanged(QString)));

    valuedelegate* price=new valuedelegate(this,model,inputType::PRICE);
    ui->tableView->setItemDelegateForColumn(6,price);
    connect(price,SIGNAL(finished(QString)),this,SLOT(pricechanged(QString)));

}

pricingForm::~pricingForm()
{
    delete ui;
}


void pricingForm::valuechanged(QString str)
{
    QModelIndex index= ui->tableView->currentIndex();
    QString unitpriceSTR=model->item(index.row(),1)->data(Qt::EditRole).toString();
    double value=str.toDouble();
    QStringList lst=unitpriceSTR.split("،");
    double unitprice=0;
    if(lst.size()>1)
        unitprice=lst.at(1).toDouble();

    double newprice=value*unitprice;
    model->item(index.row(),6)->setData(QString::number(newprice,'f',2),Qt::EditRole);

}

void pricingForm::pricechanged(QString str)
{
    QModelIndex index= ui->tableView->currentIndex();
    double price=str.toDouble();
    setnewValue(index.row(),price);

}

void pricingForm::setnewValue(int row,double price)
{
    QString unitpriceSTR=model->item(row,1)->data(Qt::EditRole).toString();
    QStringList lst=unitpriceSTR.split("،");
    double unitprice=0;
    if(lst.size()>1)
        unitprice=lst.at(1).toDouble();

    double newvalue=price/unitprice;
    model->item(row,5)->setData(QString::number(newvalue,'f',3),Qt::EditRole);
}

void pricingForm::on_FoodselectPB_clicked()
{
    QStringList list,listId;
    for (int i=0;i<foodsmodel->rowCount();i++) {
        list.append(foodsmodel->item(i,0)->data(Qt::EditRole).toString());//name
        listId.append(foodsmodel->item(i,2)->data(Qt::EditRole).toString());//id
    }

    FoodSelectDialog* dlg=new FoodSelectDialog(0,list,listId);
    if(dlg->exec()){
        ui->foodName->setText(dlg->currentFood);
        currentId=dlg->currentId;
        fetchData(dlg->currentId);
        ui->sellPrice->setValue(0);
    }

    delete dlg;
}

void pricingForm::fetchData(QString foodId)
{
    while (model->rowCount()) {
        model->removeRow(model->rowCount()-1);
    }


    QString foodName,price,matID,value,matName,unit,matPrice;

//    QSqlQuery qry;
//    qry.prepare("SELECT * FROM Foods WHERE FoodId=:foodid");
//    qry.bindValue(":foodid",foodId);
//    bool ok=qry.exec();

//    while (qry.next()) {
//        foodName=qry.value(1).toString();
//        price=qry.value(2).toString();
//        ui->matnameInp->setText(foodName);

//    }

//    if(!ok) return;

    QSqlQuery qry2;
    qry2.prepare("SELECT * FROM FoodMats WHERE FoodId=:foodid");
    qry2.bindValue(":foodid",foodId);
    qry2.exec();


    double finalprice=0;
    while (qry2.next()) {
        matID=qry2.value(2).toString();
        value=qry2.value(3).toString();
        QSqlQuery qry3;
        qry3.prepare("SELECT * FROM Materials WHERE MatId=:matid");
        qry3.bindValue(":matid",matID);
        qry3.exec();
        QList<QStandardItem*> dataList;
        QStandardItem* item=new QStandardItem;
        QStandardItem* checkableitem=new QStandardItem;
        checkableitem->setCheckable(1);
        while (qry3.next()) {
            matName=qry3.value(1).toString();
            unit=qry3.value(2).toString();
            matPrice=qry3.value(3).toString();
            QStringList pulst=unit2gram(matPrice,unit);

            double matvalue=value.toDouble();
            double matprice=pulst.last().toDouble();
            double newprice=matvalue * matprice;

            item->setEditable(0);
            item->setData(matName,Qt::EditRole);
            dataList<<item->clone();

            QString str=QString("هر %1، %2").arg(pulst.first()).arg(pulst.last());
            item->setData(str,Qt::EditRole);
            dataList<<item->clone();

            item->setData(value,Qt::EditRole);
            dataList<<item->clone();

            item->setData(QString::number(newprice,'f',2),Qt::EditRole);
            dataList<<item->clone();

            item->setEditable(0);
            item->setData(matID,Qt::EditRole);
            dataList<<item->clone();

            checkableitem->setEditable(1);
            checkableitem->setData(value,Qt::EditRole);
            dataList<<checkableitem->clone();

            item->setEditable(1);
            item->setData(QString::number(newprice,'f',2),Qt::EditRole);
            dataList<<item->clone();

            model->insertRow(model->rowCount(),dataList);
            dataList.clear();


            finalprice+=newprice;

        }
        delete item;
        delete checkableitem;
    }

    ui->primePrice->setValue(finalprice);

}



//void pricingForm::on_profitPercent_editingFinished()
//{
//    float primeprice=0,sellprice=0,profit=0,profitpercnt=0;
//    primeprice=ui->primePrice->value();
//    if(primeprice>0.00){
//        sellprice=(1.0+(profitpercnt/100.0))*primeprice;
//        profit=sellprice-primeprice;
//    }

//    ui->sellPrice->setValue(sellprice);

//}

void pricingForm::on_sellPrice_editingFinished()
{
    double primeprice=0,sellprice=0,profit=0,profitpercent=0;
    primeprice=ui->primePrice->value();
    if(primeprice>0.00){
        sellprice=ui->sellPrice->value();
        profitpercent=((sellprice/primeprice)-1.0)*100.0;
        profit=sellprice-primeprice;
    }

}

//void pricingForm::on_Profit_editingFinished()
//{
//    float primeprice=0,sellprice=0,profit=0,profitpercent=0;
//    primeprice=ui->primePrice->value();
//    if(primeprice>0.00){
//        profit=ui->Profit->value();
//        sellprice=profit+primeprice;
//        profitpercent=((sellprice/primeprice)-1.0)*100.0;
//    }
//    ui->profitPercent->setValue(profitpercent);
//    ui->sellPrice->setValue(sellprice);

//}


void pricingForm::CheckStateChanged(QModelIndex ind)
{
    if(ind.column()==5){
        int cbvalue=ind.data(Qt::CheckStateRole).toInt();
        QColor color=QColor(Qt::white);
        if(cbvalue==2)
            color=QColor(Qt::gray);

        model->itemFromIndex(ind)->setData(color,Qt::BackgroundRole);
        model->item(ind.row(),6)->setData(color,Qt::BackgroundRole);
        model->item(ind.row(),5)->setEditable(cbvalue!=2);
        model->item(ind.row(),6)->setEditable(cbvalue!=2);
        model->item(ind.row(),5)->setData(model->item(ind.row(),2)->data(Qt::EditRole).toString(),Qt::EditRole);
        model->item(ind.row(),6)->setData(model->item(ind.row(),3)->data(Qt::EditRole).toString(),Qt::EditRole);
    }

}

QStringList pricingForm::unit2gram(QString unitprice, QString unit)
{
    QStringList lst;
    double price=unitprice.toDouble();
    if((unit=="گرم") | (unit=="عدد")){
        lst.append(unit);
        lst.append(QString::number(price,'f',2));
    }
    else if(unit=="کیلوگرم"){
        price=price/1000.0;
        lst.append("گرم");
        lst.append(QString::number(price,'f',2));
    }
    else if(unit == "تُن"){
        price=price/10000.0;
        lst.append("گرم");
        lst.append(QString::number(price,'f',2));
    }
    else{
        lst.append("??");
        lst.append("??");
    }
    return  lst;
}

void pricingForm::updateValues_Prices()
{
    double primePrice=ui->primePrice->value();
    double newprice=ui->sellPrice->value();
    double constPrices=0.0;
    for (int i=0;i<model->rowCount();i++) {
        int ind=model->item(i,5)->data(Qt::CheckStateRole).toInt();
        if(ind==2){
            constPrices+=model->item(i,6)->data(Qt::EditRole).toDouble();
        }
    }

    double mul_factor=(newprice-constPrices)/(primePrice-constPrices);

    if(primePrice==constPrices){
        QMessageBox::warning(this,tr("Food Pri€er"),
                                 "حداقل یک متغیر باید غیر ثابت باشد.");
        return;
    }

    for (int i=0;i<model->rowCount();i++) {
        int ind=model->item(i,5)->data(Qt::CheckStateRole).toInt();
        if(ind!=2){
            double price=model->item(i,3)->data(Qt::EditRole).toDouble();
            double newp=price*mul_factor;
            model->item(i,6)->setData(QString::number(newp,'f',2),Qt::EditRole);
            setnewValue(i,newp);

        }
    }

}

void pricingForm::showEvent(QShowEvent *event)
{
    QList<QStandardItem*>lst= foodsmodel->findItems(currentId,Qt::MatchExactly,2);
    if(lst.isEmpty()){
        currentId="";
        ui->foodName->clear();
    }
    else
        fetchData(currentId);

    QWidget::showEvent(event);
}

void pricingForm::on_retrieveDataPB_clicked()
{
    fetchData(currentId);
}




void pricingForm::on_calculatePB_clicked()
{
    if(model->rowCount()==0){
        QMessageBox::warning(this,tr("Food Pri€er"),
                                 "جدول خالی است.");
        return;
    }
    updateValues_Prices();
}

void pricingForm::on_saveReplacePB_clicked()
{

    if(currentId.isEmpty()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "غذای انتخابی یافت نشد.");
        return;
    }

    int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                   "غذای مورد نظر به روز شود؟",
                                   QMessageBox::Yes | QMessageBox::No);

    if(ret==QMessageBox::No)
        return;

    QString name=ui->foodName->text();
    bool ok=0;

    QSqlQuery qry2;
    qry2.prepare("DELETE FROM FoodMats WHERE FoodId=:foodid;");
    qry2.bindValue(":foodid",currentId);
    ok=qry2.exec();

    if(!ok){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "عملیات انجام نشد.");
        return;
    }



    for (int i=0;i<model->rowCount();i++) {
        QSqlQuery qry2;
        qry2.prepare("INSERT INTO FoodMats(FoodId,MatId,Value) VALUES(:foodid,:matid,:value);");
        QString value=model->item(i,5)->data(Qt::EditRole).toString();
        QString matid=model->item(i,4)->data(Qt::EditRole).toString();
        qry2.bindValue(":foodid",currentId);
        qry2.bindValue(":matid",matid);
        qry2.bindValue(":value",value);
        ok=qry2.exec();
    }

    if(ok){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "عملیات با موفقیت انجام شد.");
        fetchData(currentId);
    }
    else{
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "عملیات انجام نشد.");
    }

}

void pricingForm::on_saveNewNamePB_clicked()
{
    if(currentId.isEmpty()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "غذای انتخابی یافت نشد.");
        return;
    }

    QString oldname=ui->foodName->text();

    ChangeNameDialog* dlg=new ChangeNameDialog(0,oldname,foodsmodel);

    if(dlg->exec()){
        bool ok=0;
        QString newname=dlg->foodName;

        QSqlQuery qry1;
        qry1.prepare("INSERT INTO Foods(FoodName) VALUES(:name);");
        qry1.bindValue(":name",newname);
        qry1.exec();


        QSqlQuery qry11;
        qry11.prepare("SELECT * FROM Foods WHERE FoodName=:name");
        qry11.bindValue(":name",newname);
        qry11.exec();

        QString foodid;
        while (qry11.next()) {
            ok=1;
            foodid = qry11.value(0).toString();
        }

        if(!ok){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "عملیات انجام نشد.");
            return;
        }


        for (int i=0;i<model->rowCount();i++) {
            QSqlQuery qry2;
            qry2.prepare("INSERT INTO FoodMats(FoodId,MatId,Value) VALUES(:foodid,:matid,:value);");
            QString value=model->item(i,5)->data(Qt::EditRole).toString();
            QString matid=model->item(i,4)->data(Qt::EditRole).toString();
            qry2.bindValue(":foodid",foodid);
            qry2.bindValue(":matid",matid);
            qry2.bindValue(":value",value);
            ok=qry2.exec();
        }

        if(ok){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "عملیات با موفقیت انجام شد.");
        }
    }

    delete dlg;
}
