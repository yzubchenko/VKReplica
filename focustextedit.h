#ifndef FOCUSTEXTEDIT_H
#define FOCUSTEXTEDIT_H

#include <QTextEdit>

class FocusTextEdit : public QTextEdit {
    Q_OBJECT
public:
    explicit FocusTextEdit(QWidget* parent = 0);
    virtual void focusInEvent(QFocusEvent* event);
    virtual void keyPressEvent(QKeyEvent* e);
signals:
    void focusIn();
    void returnPressed();
};

#endif // FOCUSTEXTEDIT_H
