#ifndef AUDIODELEGATE_H
#define AUDIODELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class AudioDelegate : public QStyledItemDelegate {
    Q_OBJECT
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

};

#endif // AUDIODELEGATE_H
