#ifndef CONTACTDELEGATE_H
#define CONTACTDELEGATE_H
#include <QStyledItemDelegate>

class ContactDelegate : public QStyledItemDelegate {

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
#endif // CONTACTDELEGATE_H
