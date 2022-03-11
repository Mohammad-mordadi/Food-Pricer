#include "materialsdialog.h"
#include "ui_materialsdialog.h"
#include<QDebug>

MaterialsDialog::MaterialsDialog(QWidget *parent, QStandardItemModel *matmodel, QStandardItemModel *model) :
    QDialog(parent),
    ui(new Ui::MaterialsDialog)
{
    ui->setupUi(this);

    layout = new QVBoxLayout;

    QCheckBox* cb;

    for (int i=0;i<matmodel->rowCount();i++) {
        QString name=matmodel->item(i,0)->data(Qt::EditRole).toString();
        cb=new QCheckBox(name);
        cb->setObjectName(matmodel->item(i,3)->data(Qt::EditRole).toString());//matid
        checkList.append(cb);
        layout->addWidget(cb);
        QList<QStandardItem*>lst= model->findItems(name,Qt::MatchExactly,0);
        cb->setChecked(lst.size()>0);
    }

    ui->scrollAreaWidgetContents->setLayout(layout);

}

MaterialsDialog::~MaterialsDialog()
{
    delete ui;
}



void MaterialsDialog::on_okPB_clicked()
{
    QCheckBox* cb;
    for (int i=0;i<checkList.size();i++) {
        cb=(checkList.at(i));
        list.append(cb->text());
        IDlist.append(cb->objectName());
        states.append(cb->isChecked());
    }

    close();
}

void MaterialsDialog::on_cancelPB_clicked()
{
    close();
}

void MaterialsDialog::on_selectallPB_clicked()
{
    QCheckBox* cb;
    for (int i=0;i<checkList.size();i++) {
        cb=(checkList.at(i));
        cb->setChecked(1);
    }
}

void MaterialsDialog::on_deselectallPB_clicked()
{
    QCheckBox* cb;
    for (int i=0;i<checkList.size();i++) {
        cb=(checkList.at(i));
        cb->setChecked(0);
    }
}
