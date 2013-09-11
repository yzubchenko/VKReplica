#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H
#include <QStyledItemDelegate>

class ContactDelegate : public QStyledItemDelegate
{
  // Q_OBJECT
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};
#endif // HTMLDELEGATE_H
