#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include "dialogmodel.h"

#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

class MessageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

private:
    QBrush acquireBackgroundBrush(Message* message) const;
signals:

public slots:

};

#endif // MESSAGEDELEGATE_H
