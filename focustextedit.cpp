#include "focustextedit.h"

#include <QKeyEvent>
#include <QTextEdit>

FocusTextEdit::FocusTextEdit(QWidget* parent) : QTextEdit(parent){

}

void FocusTextEdit::focusInEvent(QFocusEvent* event) {
    QTextEdit::focusInEvent(event);
    emit focusIn();
}

void FocusTextEdit::keyPressEvent(QKeyEvent* e) {
    QTextEdit::keyPressEvent(e);
    if ((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) && e->modifiers() == 0){
        emit returnPressed();
    }
}
