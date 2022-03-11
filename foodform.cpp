#include "foodform.h"
#include "ui_foodform.h"
#include"materialsdialog.h"
#include<QDebug>
#include<QMessageBox>

extern QStandardItemModel* MatModel;
extern QStandardItemModel* foodsmodel;

FooDForm::FooDForm(QWidget *parent, QString foodid) :
    QWidget(parent),foodID(foodid),
    ui(new Ui::FooDForm)
{
    ui->setupUi(this);
    ui->priceout->setText("0");
    editMode=0;

    setWindowModality(Qt::ApplicationModal);//Qt::ApplicationModal


    done=1;

    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);

    model->setHorizontalHeaderItem(0,new QStandardItem("ماده غذایی"));
    model->setHorizontalHeaderItem(1,new QStandardItem("مقدار"));
    model->setHorizontalHeaderItem(2,new QStandardItem("قیمت واحد"));
    model->setHorizontalHeaderItem(3,new QStandardItem("قیمت"));
    model->setHorizontalHeaderItem(4,new QStandardItem("MatID"));

    ui->tableView->setColumnHidden(4,1);

    valuedelegate* value=new valuedelegate(this,model,inputType::VALUE);
    ui->tableView->setItemDelegateForColumn(1,value);
    connect(value,SIGNAL(finished(QString)),this,SLOT(valuechanged(QString)));

    valuedelegate* price=new valuedelegate(this,model,inputType::PRICE);
    ui->tableView->setItemDelegateForColumn(3,price);
    connect(price,SIGNAL(finished(QString)),this,SLOT(pricechanged(QString)));

    if(!foodID.isEmpty()){
        editMode=1;
        ui->matnameInp->setReadOnly(1);
        readDB(foodID,0);
    }
}

void FooDForm::setReadOnly(bool ok)
{
    ui->matnameInp->setReadOnly(ok);
    ui->addPB->setEnabled(!ok);
    ui->add2list->setEnabled(!ok);
    if(ok)
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    else
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
}

FooDForm::~FooDForm()
{
    delete ui;
}

void FooDForm::on_add2list_clicked()
{
    done=0;
    MaterialsDialog* dlg=new MaterialsDialog(this,MatModel,model);
    dlg->exec();

    QList<QStandardItem*> dataList;
    QStandardItem* item=new QStandardItem;
    for (int i=0;i<dlg->list.size();i++) {
        QString name=dlg->list.at(i);
        bool state=dlg->states.at(i);
        QList<QStandardItem*>lst=MatModel->findItems(name,Qt::MatchExactly,0);
        if(!lst.isEmpty()){
            QList<QStandardItem*>lst2=model->findItems(name,Qt::MatchExactly,0);
            if(lst2.isEmpty() & state){
                QString unit = MatModel->item(lst.first()->row(),1)->data(Qt::EditRole).toString();
                QString price = MatModel->item(lst.first()->row(),2)->data(Qt::EditRole).toString();
                QString matid = dlg->IDlist.at(i);

                item->setEditable(0);
                item->setData(name,Qt::EditRole);
                dataList<<item->clone();

                item->setEditable(1);
                item->setData("0",Qt::EditRole);
                dataList<<item->clone();

                QStringList pulst=unit2gram(price,unit);
                item->setEditable(0);
                QString str=QString("هر %1، %2").arg(pulst.first()).arg(pulst.last());
                item->setData(str,Qt::EditRole);
                dataList<<item->clone();

                item->setEditable(1);
                item->setData("0",Qt::EditRole);
                dataList<<item->clone();

                item->setEditable(0);
                item->setData(matid,Qt::EditRole);
                dataList<<item->clone();

                model->insertRow(model->rowCount(),dataList);
                dataList.clear();
            }
            else if(!lst2.isEmpty() & !state){
                model->removeRow(lst2.first()->row());
            }
        }

    }
    delete item;
    delete dlg;

    setfinalPrice();
}

void FooDForm::valuechanged(QString str)
{
    done=0;
    QModelIndex index= ui->tableView->currentIndex();
    QString unitpriceSTR=model->item(index.row(),2)->data(Qt::EditRole).toString();
    double value=str.toDouble();
    QStringList lst=unitpriceSTR.split("،");
    double unitprice=0;
    if(lst.size()>1)
        unitprice=lst.at(1).toDouble();

    double newprice=value*unitprice;
    model->item(index.row(),3)->setData(QString::number(newprice,'f',2),Qt::EditRole);

    setfinalPrice();
}

void FooDForm::pricechanged(QString str)
{
    done=0;
    QModelIndex index= ui->tableView->currentIndex();
    QString unitpriceSTR=model->item(index.row(),2)->data(Qt::EditRole).toString();
    double price=str.toDouble();
    QStringList lst=unitpriceSTR.split("،");
    double unitprice=0;
    if(lst.size()>1)
        unitprice=lst.at(1).toDouble();

    double newvalue=price/unitprice;


    model->item(index.row(),1)->setData(QString::number(newvalue,'f',3),Qt::EditRole);

    setfinalPrice();
}

void FooDForm::setfinalPrice()
{
    double price=0;
    for (int i=0;i<model->rowCount();i++) {
         price+=model->item(i,3)->data(Qt::EditRole).toDouble();
    }
    ui->priceout->setText(QString::number(price,'f',2));
}

//void FooDForm::updateCurrentValues()
//{
//    int rows=model->rowCount();
//    for (int i=rows-1;i>=0;i--) {
//        QString name=model->item(i,0)->data(Qt::EditRole).toString();
//        QList<QStandardItem*>lst=MatModel->findItems(name,Qt::MatchExactly,0);
//        if(lst.isEmpty()){
//            model->removeRow(i);
//        }
//        else{
//            QString Matunit=MatModel->item(i,1)->data(Qt::EditRole).toString();
//            QString Matprice=MatModel->item(i,2)->data(Qt::EditRole).toString();
//            QString str=QString("هر %1، %2").arg(Matunit).arg(Matprice);
//            QString unitprice=model->item(i,2)->data(Qt::EditRole).toString();
//            if(str!=unitprice)
//                model->item(i,2)->setData(str,Qt::EditRole);
//        }
//    }
//}

void FooDForm::showEvent(QShowEvent *event)
{
    done=1;
    QWidget::showEvent(event);
}

void FooDForm::closeEvent(QCloseEvent *event)
{
    if(done){
        deleteLater();

        return;
    }

    if(!editMode){
        int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                       tr("از بستن پنجره اطمینان دارید؟\n"),
                                       QMessageBox::Yes | QMessageBox::No);

        if(ret==QMessageBox::Yes){
            deleteLater();
        }
        else
            event->ignore();
    }
    else{
        on_addPB_clicked();
    }
}

QStringList FooDForm::unit2gram(QString unitprice, QString unit)
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

QString FooDForm::readDB(QString foodid, bool readPrice)
{
    QString foodName,price,matID,value,matName,unit,matPrice;
    QSqlQuery qry;
    qry.prepare("SELECT * FROM Foods WHERE FoodId=:foodid");
    qry.bindValue(":foodid",foodid);
    bool ok=qry.exec();

    while (qry.next()) {
        foodName=qry.value(1).toString();
        price=qry.value(2).toString();
        ui->matnameInp->setText(foodName);

    }

    if(!ok) return "";

    QSqlQuery qry2;
    qry2.prepare("SELECT * FROM FoodMats WHERE FoodId=:foodid");
    qry2.bindValue(":foodid",foodid);
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
        while (qry3.next()) {
            matName=qry3.value(1).toString();
            unit=qry3.value(2).toString();
            matPrice=qry3.value(3).toString();
            QStringList pulst=unit2gram(matPrice,unit);

            double matvalue=value.toDouble();
            double matprice=pulst.last().toDouble();
            double newprice=matvalue * matprice;

            if(!readPrice){
                item->setEditable(0);
                item->setData(matName,Qt::EditRole);
                dataList<<item->clone();

                item->setEditable(1);
                item->setData(value,Qt::EditRole);
                dataList<<item->clone();


                item->setEditable(0);
                QString str=QString("هر %1، %2").arg(pulst.first()).arg(pulst.last());
                item->setData(str,Qt::EditRole);
                dataList<<item->clone();


                item->setEditable(1);
                item->setData(QString::number(newprice,'f',2),Qt::EditRole);
                dataList<<item->clone();

                item->setEditable(0);
                item->setData(matID,Qt::EditRole);
                dataList<<item->clone();

                model->insertRow(model->rowCount(),dataList);
                dataList.clear();
            }


            finalprice+=newprice;

        }
        delete item;
    }

    if(!readPrice)
        setfinalPrice();

    return QString::number(finalprice,'f',2);

    //fill table
    //fill dialog list

}

//QString FooDForm::getMaterials()
//{
//    QString str;
//    for (int i=0;i<model->rowCount();i++) {
//        QString matname=model->item(i,0)->data(Qt::EditRole).toString();
//        QString value=model->item(i,1)->data(Qt::EditRole).toString();
//        str+=matname+":"+value+",";
//    }
//    str.chop(1);
//    return  str;

//}


void FooDForm::on_addPB_clicked()//add to database
{
    QString name=ui->matnameInp->text();
    bool ok=0;

    QString message="غذای مورد نظر به روز شود؟";

    if(!editMode){
        if(name.isEmpty()){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "نام نمی تواند خالی باشد.");
            return;
        }

        QList<QStandardItem*>lst=foodsmodel->findItems(name,Qt::MatchExactly,0);
        if(!lst.isEmpty()){
            QMessageBox::information(this,tr("Food Pri€er"),
                                     "نام وارد شده تکراری است.\n"
                                     "نام دیگری انتخاب کنید.");
            return;
        }
        message="به پایگاه داده اضافه شود؟";
    }

    int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                   message,
                                   QMessageBox::Yes | QMessageBox::No);

    if(ret==QMessageBox::No){
        if(!editMode)
            return;
        else{
            if(!done)
                return;
            else{
                done=1;
                deleteLater();
                return;
            }
        }
    }

    if(!editMode){
        QSqlQuery qry1;
        qry1.prepare("INSERT INTO Foods(FoodName) VALUES(:name);");
        qry1.bindValue(":name",name);
        qry1.bindValue(":price",ui->priceout->text());
        qry1.exec();
    }

    QSqlQuery qry11;
    qry11.prepare("SELECT * FROM Foods WHERE FoodName=:name");
    qry11.bindValue(":name",name);
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

    if(editMode){
        QSqlQuery qry2;
        qry2.prepare("DELETE FROM FoodMats WHERE FoodId=:foodid;");
        qry2.bindValue(":foodid",foodid);
        ok=qry2.exec();
    }

    for (int i=0;i<model->rowCount();i++) {
        QSqlQuery qry2;
        qry2.prepare("INSERT INTO FoodMats(FoodId,MatId,Value) VALUES(:foodid,:matid,:value);");
        QString value=model->item(i,1)->data(Qt::EditRole).toString();
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

    if(!editMode){
        if(ok){
            done=1;
            QList<QStandardItem*> dataList;
            QStandardItem* item=new QStandardItem;

            item->setEditable(0);
            item->setData(name,Qt::EditRole);
            dataList<<item->clone();

            item->setData(ui->priceout->text(),Qt::EditRole);
            dataList<<item->clone();

            item->setData(foodid,Qt::EditRole);
            dataList<<item->clone();

            foodsmodel->insertRow(foodsmodel->rowCount(),dataList);
            delete item;

            while (model->rowCount()>0) {
                model->removeRow(model->rowCount()-1);
            }
            ui->matnameInp->clear();
        }
    }
    else{
        QList<QStandardItem*>lst= foodsmodel->findItems(name,Qt::MatchExactly,0);
        if(lst.size()){
            foodsmodel->item(lst.first()->row(),1)->setData(ui->priceout->text(),Qt::EditRole);
        }
    }
    done=1;
    close();
}


//=---------------------------------------------------------------------------------
//=---------------------------------------------------------------------------------
valuedelegate::valuedelegate(QObject *parent, QStandardItemModel *model, inputType::TYP type):
    QStyledItemDelegate(parent),mymodel(model),mytype(type)
{
    if(mytype==inputType::VALUE){
        QString s1="(^(\\d{1,10}\\.\\d{1,3})|(\\d{1,10})$)";
        re=QRegExp(s1);
    }
    else if(mytype==inputType::PRICE){
        QString s1="(^(\\d{1,10}\\.\\d{1,3})|(\\d{1,10})$)";
        re=QRegExp(s1);
    }

}

QWidget *valuedelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* combo=new QLineEdit(parent);
    QRegExpValidator *validator = new QRegExpValidator(re,combo);
    combo->setValidator(validator);
    connect(combo,SIGNAL(editingFinished()),this,SLOT(CloseandEmit()));
    return combo;
}

void valuedelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value=mymodel->data(index,Qt::EditRole).toString();
    QLineEdit* combo=static_cast<QLineEdit*>(editor);
    combo->setText(value);
}

void valuedelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit* combo=static_cast<QLineEdit*>(editor);
    if(mytype==inputType::PRICE){
        double value=combo->text().toDouble();
        model->setData(index,QString::number(value,'f',3),Qt::EditRole);
    }
    else if(mytype==inputType::VALUE){
//        float value=combo->text().toFloat();
        model->setData(index,combo->text(),Qt::EditRole);
    }


    //    if((value.at(0)=='0')|(value.at(0)=='1')){
    //        if(value.contains('#')){
    //            int dot=value.indexOf('#');
//            int s=value.split('#').first().right(dot-1).toUInt();
//            if(s>65536)
//                s=65536;
//            else if(s==0)
//                s=1;
//            int k=value.split('#').at(1).toUInt();
//            if(k>16)
//                k=16;
//            else if(k==0)
//                k=1;
//            value=value.at(0)+QString("%1").arg(s,5,10,QChar('0'))+'#'+QString::number(k);
//        }
//        else{
//            int s=value.right(value.length()-1).toUInt();
//            if(s>65536)
//                s=65536;
//            else if(s==0)
//                s=1;
//            value=value.at(0)+QString("%1").arg(s,5,10,QChar('0'));
//        }
//    }
//    else{
//        if(value.contains('.')){
//            int dot=value.indexOf('.');
//            int s=value.split('.').first().right(dot-1).toUInt();
//            if(s>65536)
//                s=65536;
//            else if(s==0)
//                s=1;
//            int k=value.split('.').at(1).toUInt();
//            if(k>15)
//                k=15;
//            value=value.at(0)+QString("%1").arg(s,5,10,QChar('0'))+'.'+QString::number(k);
//        }
//        else{
//            int s=value.right(value.length()-1).toUInt();
//            if(s>65536)
//                s=65536;
//            else if(s==0)
//                s=1;
//            value=value.at(0)+QString("%1").arg(s,5,10,QChar('0'));
//        }
//    }
}

void valuedelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


void valuedelegate::CloseandEmit()
{
    QLineEdit* le=static_cast<QLineEdit*>(sender());
    QString str=le->text();
//    QString ss="";
//    if(str.contains('#'))
//        ss=lst3.first();
//    else if(str.startsWith('0') | str.startsWith('1') | str.contains('.'))
//        ss=lst1.first();
//    else if(str.startsWith('3') | str.startsWith('4') )
//        ss=lst2.first();
//    mymodel->item(mytable->currentIndex().row(),ModBusTable::type)->setData(ss,Qt::EditRole);
    emit finished(str);
}


//---------------------------------------------------------






