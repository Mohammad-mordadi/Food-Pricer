#include "button.h"
#include<QDebug>

static Button* selected=NULL;

Button::Button(QWidget *parent):
    QPushButton(parent)
{
    w=2;
    brush=QBrush(Qt::white,Qt::SolidPattern);
    brush2=brush;
    brush2.setColor(QColor(Qt::green).lighter(170));
    pen.setColor(Qt::blue);
    pen.setWidth(w);
    fnt.setFamily("Helvetica [Cronyx]");
    fnt.setBold(1);
    fnt.setPixelSize(15);
    txtpen.setColor(Qt::black);
}

void Button::setselected(Button *btn)
{
    selected=btn;
}

void Button::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(selected==this) painter.setBrush(brush2); else painter.setBrush(brush);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QRect newrect=QRect(w,w,this->rect().width()-2*w,this->rect().height()-2*w);
    QRect newrect2=QRect(w,w,this->rect().width()*0.9-2*w,this->rect().height()-2*w);
    painter.drawRoundedRect(newrect,20, 20);
    if(selected==this) txtpen.setColor(Qt::blue); else txtpen.setColor(Qt::black);
    painter.setPen(txtpen);
    painter.setFont(fnt);
//    painter.drawText(50,newrect.height()/1.4,this->text());
    painter.drawText(newrect2, Qt::AlignVCenter|Qt::AlignRight, this->text());
    if(selected==this)
        painter.drawPixmap(5,w,newrect.width()/4,newrect.height(),this->icon().pixmap(newrect.width()/4,newrect.height()));



}

void Button::enterEvent(QEvent *event)
{
    brush.setColor(QColor(Qt::blue).lighter(170));
//     pen.setColor(Qt::blue);
    update();
}

void Button::leaveEvent(QEvent *event)
{
    brush.setColor(Qt::white);
//     pen.setColor(Qt::red);
    update();
}

void Button::mousePressEvent(QMouseEvent *e)
{
    selected=this;
    static_cast<QWidget*>(this->parent())->update();
    emit btnClicked();//click emitted when mouse is pressed not released
    QPushButton::mousePressEvent(e);
}
