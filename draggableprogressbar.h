#ifndef DRAGGABLEPROGRESSBAR_H
#define DRAGGABLEPROGRESSBAR_H

#include <QKeyEvent>
#include <QProgressBar>

class DraggableProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit DraggableProgressBar(QWidget *parent = 0);
    void setBaseText(QString text);
    void setValueText(QString value);
signals:
    void mouseReleasedOnValue(uint);
    void mousePressedOnValue(uint);
protected:
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mousePressEvent(QMouseEvent* e);
private:
    QString text;
};

#endif // DRAGGABLEPROGRESSBAR_H
