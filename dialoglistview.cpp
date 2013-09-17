#include "dialoglistview.h"

DialogListView::DialogListView(QWidget *parent) : QListView(parent){
}

void DialogListView::resizeEvent(QResizeEvent *e)
{
    reset();
    QListView::resizeEvent(e);
}
