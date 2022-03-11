#include "materialsform.h"
#include "ui_materialsform.h"
#include<QDebug>
#include<QMessageBox>


QStandardItemModel* MatModel;


materialsForm::materialsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::materialsForm)
{
    ui->setupUi(this);

    MatModel=new QStandardItemModel(this);
    ui->tableView->setModel(MatModel);

    MatModel->setHorizontalHeaderItem(0,new QStandardItem("Ingredient"));
    MatModel->setHorizontalHeaderItem(1,new QStandardItem("Unit"));
    MatModel->setHorizontalHeaderItem(2,new QStandardItem("Price"));
    MatModel->setHorizontalHeaderItem(3,new QStandardItem("MatID"));

    ui->tableView->setColumnHidden(3,1);

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(showData(QModelIndex)));

    QRegExp re=QRegExp("^[A-Za-z0-9آ-یءچ]{1,1}[A-Za-z0-9آ-ی ء چ]{0,19}");
    QRegExpValidator *validator = new QRegExpValidator(re,ui->matnameInp);
    ui->matnameInp->setValidator(validator);

    fetchData();


}

materialsForm::~materialsForm()
{
    delete ui;
}

void materialsForm::fetchData()
{
    while (MatModel->rowCount()) {
        MatModel->removeRow(MatModel->rowCount()-1);
    }

    QSqlQuery qry("SELECT * FROM Materials;");
    qry.exec();
//    qDebug()<< qry.lastError().text();
    QList<QStandardItem*> dataList;
    QStandardItem* item=new QStandardItem;
    while (qry.next()) {
        QString matid=qry.value(0).toString();
        QString name = qry.value(1).toString();
        QString unit = qry.value(2).toString();
        QString price = qry.value(3).toString();

        item->setEditable(0);
        item->setData(name,Qt::EditRole);
        dataList<<item->clone();
        item->setData(unit,Qt::EditRole);
        dataList<<item->clone();
        item->setData(price,Qt::EditRole);
        dataList<<item->clone();
        item->setData(matid,Qt::EditRole);
        dataList<<item->clone();
        MatModel->insertRow(MatModel->rowCount(),dataList);
        dataList.clear();
    }
    delete  item;
}

bool materialsForm::addData(QString name, QString unit, qlonglong price)
{
    if(name.isEmpty()){
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Name field is needed.");
        return 0;
    }


    QSqlQuery qry;
    qry.prepare("SELECT * FROM Materials WHERE Material=:name");
    qry.bindValue(":name",name);
    qry.exec();

    bool ok=0;
    qlonglong matid=0;
    while (qry.next()) {
        ok=1;
        matid = qry.value(0).toLongLong();
    }
    if(ok){
        int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                       QString("Entry already exists in the database.\n"
                                          "Do you want to update \"%1\"").arg(name),
                                       QMessageBox::Yes | QMessageBox::No);
        if(ret==QMessageBox::Yes){
            QSqlQuery qry2;
            qry2.prepare("UPDATE Materials SET Material=:name,Unit=:unit,Price=:price WHERE MatId=:matid;");
            qry2.bindValue(":matid",matid);
            qry2.bindValue(":name",name);
            qry2.bindValue(":unit",unit);
            qry2.bindValue(":price",price);
            qry2.exec();
        }
        else ok=0;
    }
    else{
        int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                       tr("Insert to the Database?\n"),
                                       QMessageBox::Yes | QMessageBox::No);
        if(ret==QMessageBox::Yes){
            ok=1;
            QSqlQuery qry2;
            qry2.prepare("INSERT INTO Materials(Material,Unit,Price) VALUES(:name,:unit,:price);");
            qry2.bindValue(":name",name);
            qry2.bindValue(":unit",unit);
            qry2.bindValue(":price",price);
            qry2.exec();
        }
        else ok=0;
    }

    return ok;
}

bool materialsForm::removeData(QString name)
{
    QSqlQuery qry;
    qry.prepare("SELECT * FROM Materials WHERE Material=:name");
    qry.bindValue(":name",name);
    qry.exec();

    bool ok=0;
    qlonglong matid=0;
    while (qry.next()) {
        ok=1;
        matid = qry.value(0).toLongLong();
    }
    if(ok){
        QSqlQuery qry2;
        qry2.prepare("DELETE FROM Materials WHERE MatId=:matid;");
        qry2.bindValue(":matid",matid);
        qry2.exec();
    }
    else{
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Nothing found.");
    }

    return ok;
}

void materialsForm::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void materialsForm::showData(QModelIndex index)
{
    ui->matnameInp->setText(MatModel->item(index.row(),0)->data(Qt::EditRole).toString());
    ui->unitInp->setCurrentText(MatModel->item(index.row(),1)->data(Qt::EditRole).toString());
    ui->priceInp->setValue(MatModel->item(index.row(),2)->data(Qt::EditRole).toInt());

}

void materialsForm::on_addPB_clicked()
{
    if(addData(ui->matnameInp->text(),ui->unitInp->currentText(),ui->priceInp->value())){
        fetchData();
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Inserted successfully.");

    }
//    else
//        QMessageBox::information(this,tr("Food Pri€er"),
//                                 "Inserting operation failed!");

}

void materialsForm::on_deletPB_clicked()
{
    if(ui->matnameInp->text().isEmpty()) return;

    int ret = QMessageBox::warning(this, tr("Food Pri€er"),
                                   tr("Are you sure you want to delete the selected item?"),
                                   QMessageBox::Yes | QMessageBox::No);

    if(ret==QMessageBox::No) return;

    if(removeData(ui->matnameInp->text())){
        fetchData();
        QMessageBox::information(this,tr("Food Pri€er"),
                                 "Successfully deleted.");
    }
}
//-----------------------------------------------------------
Btn::Btn(QWidget *parent):
    QPushButton(parent)
{
    brush=QBrush(Qt::white,Qt::SolidPattern);
    pen.setColor(Qt::white);
    pen2.setColor(Qt::black);
    fnt.setFamily("Helvetica [Cronyx]");
    fnt.setBold(1);
    fnt.setPixelSize(15);
}

void Btn::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int w=2;
    painter.setBrush(brush);
    painter.setPen(pen);
    QRect newrect=QRect(w,w,this->rect().width()-2*w,this->rect().height()-2*w);
    painter.drawRect(this->rect());
    painter.drawPixmap(w,w,newrect.width(),newrect.height(),this->icon().pixmap(newrect.width(),newrect.height()));
    painter.setPen(pen2);
    painter.setFont(fnt);
    painter.drawText(newrect, Qt::AlignVCenter|Qt::AlignRight, this->text());
}

void Btn::enterEvent(QEvent *event)
{
    brush.setColor(QColor(Qt::green).lighter(170));
//     pen.setColor(Qt::blue);
    update();
}

void Btn::leaveEvent(QEvent *event)
{
    brush.setColor(QColor(Qt::white).lighter(170));
//     pen.setColor(Qt::blue);
    update();
}

void Btn::mousePressEvent(QMouseEvent *e)
{
    QPushButton::mousePressEvent(e);
}




