#include "draggableprogressbar.h"

DraggableProgressBar::DraggableProgressBar(QWidget *parent) :
    QProgressBar(parent),
    delimeter("/")
{

}

void DraggableProgressBar::setBaseText(QString text) {
    this->text = text;
    setFormat(this->text);
}

void DraggableProgressBar::setTotalText(QString text) {
    this->totalText = text;
}

void DraggableProgressBar::setDelimeter(QString delimeter) {
    this->delimeter = delimeter;
}

void DraggableProgressBar::setValueText(QString value) {
    QString format = text;
    format.append(" [").append(value).append(delimeter).append(totalText).append("]");
    setFormat(format);
}

void DraggableProgressBar::mouseReleaseEvent(QMouseEvent* e ) {
    int x = e->localPos().x();
    uint value = maximum()*(x+3)/width();
    this->setValue(value);
    emit mouseReleasedOnValue(value);
}

void DraggableProgressBar::mouseMoveEvent(QMouseEvent* e) {
    int x = e->localPos().x();
    uint value = maximum()*(x+3)/width();
    this->setValue(value);
}

void DraggableProgressBar::mousePressEvent(QMouseEvent* e) {
    int x = e->localPos().x();
    uint value = maximum()*(x+3)/width();
    emit mousePressedOnValue(value);
}
