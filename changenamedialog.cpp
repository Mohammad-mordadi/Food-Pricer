#include "changenamedialog.h"
#include "ui_changenamedialog.h"
#include<QMessageBox>

ChangeNameDialog::ChangeNameDialog(QWidget *parent, QString oldname, QStandardItemModel *model) :
    QDialog(parent),mymodel(model),
    ui(new Ui::ChangeNameDialog)
{
    ui->setupUi(this);
    ui->oldname->setText(oldname);
}

ChangeNameDialog::~ChangeNameDialog()
{
    delete ui;
}

void ChangeNameDialog::on_cancelPB_clicked()
{
    reject();
}

void ChangeNameDialog::on_okPB_clicked()
{
    foodName=ui->newname->text();
    if(foodName.isEmpty()){
        QMessageBox::warning(this, "Food Pri€er",
                             QString("نام غذا باید وارد شود."));
        return;
    }

     QList<QStandardItem*>lst=mymodel->findItems(foodName,Qt::MatchExactly,0);

    if(!lst.isEmpty()){
        QMessageBox::warning(this, "Food Pri€er",
                             QString("نام وارد شده در لیست غذاهای موجود وجود دارد.\n"
                                     "لطفا نام جدیدی انتخاب کنید."));
        return;
    }

    accept();
}
