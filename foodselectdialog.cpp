#include "foodselectdialog.h"
#include "ui_foodselectdialog.h"

FoodSelectDialog::FoodSelectDialog(QWidget *parent, QStringList list, QStringList listId) :
    QDialog(parent),mylistId(listId),
    ui(new Ui::FoodSelectDialog)
{
    ui->setupUi(this);
    setFoods(list);
    currentFood="";
    currentId="";
}

FoodSelectDialog::~FoodSelectDialog()
{
    delete ui;
}

void FoodSelectDialog::setFoods(QStringList lst)
{
    ui->comboBox->addItems(lst);
}

void FoodSelectDialog::on_okPB_clicked()
{
    currentFood=ui->comboBox->currentText();
    int ind=ui->comboBox->currentIndex();
    if(ind>=0)
        currentId=mylistId.at(ind);
    accept();
}

void FoodSelectDialog::on_cancelPB_clicked()
{
    reject();
}
