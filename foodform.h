#ifndef FOODFORM_H
#define FOODFORM_H

#include <QWidget>
#include<QStandardItemModel>
#include<QStyledItemDelegate>

namespace Ui {
class FooDForm;
}
namespace  inputType{
enum TYP{VALUE,PRICE};
}

class FooDForm : public QWidget
{
    Q_OBJECT

public:
    explicit FooDForm(QWidget *parent = nullptr, QString foodid="");
    bool done;
    void setReadOnly(bool ok);
    QString readDB(QString foodid,bool readPrice);

    ~FooDForm();

private slots:
    void on_add2list_clicked();
    void valuechanged(QString str);
    void pricechanged(QString str);

    void on_addPB_clicked();

private:
    Ui::FooDForm *ui;
    QStandardItemModel* model;
    void setfinalPrice();
//    void updateCurrentValues();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
//    QString getMaterials();
    QStringList foodsList;
    QString foodID;
    bool editMode;
    QStringList unit2gram(QString unitprice,QString unit);

signals:
    void showMakeFoodForm();

};

//---------------------------------------------------------------------
class valuedelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    valuedelegate(QObject* parent=0,QStandardItemModel* model=0,inputType::TYP type=inputType::VALUE);

private:
    QStandardItemModel* mymodel;
    QRegExp re;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    inputType::TYP mytype;
private slots:
    void CloseandEmit();


signals:
    void finished(QString);

};

#endif // FOODFORM_H
