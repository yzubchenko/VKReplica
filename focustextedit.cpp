#include "focustextedit.h"

#include <QTextEdit>

FocusTextEdit::FocusTextEdit(QWidget *parent) : QTextEdit(parent){

}

void FocusTextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e);
    emit focusIn();
}
