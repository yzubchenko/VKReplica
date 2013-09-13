#ifndef FOCUSTEXTEDIT_H
#define FOCUSTEXTEDIT_H

#include <QTextEdit>

class FocusTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit FocusTextEdit(QWidget *parent = 0);
    virtual void focusInEvent(QFocusEvent *e);
signals:
    void focusIn();
public slots:

};

#endif // FOCUSTEXTEDIT_H
