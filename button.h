#ifndef BUTTON_H
#define BUTTON_H

#include<QPushButton>
#include<QPainter>
#include<QMouseEvent>
#include<QBrush>

class Button : public QPushButton
{

    Q_OBJECT
public:
    explicit Button(QWidget* parent=0);
    static void setselected(Button* btn);
private:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    QBrush brush,brush2;
    QPen pen;
    QPen txtpen;
    QFont fnt;
    int w;

signals:
    void btnClicked();

};

#endif // BUTTON_H
