#include "focustextedit.h"

#include <QTextEdit>

FocusTextEdit::FocusTextEdit(QWidget *parent) : QTextEdit(parent){

}

void FocusTextEdit::focusInEvent(QFocusEvent *event) {
    QTextEdit::focusInEvent(event);
    emit focusIn();
}
