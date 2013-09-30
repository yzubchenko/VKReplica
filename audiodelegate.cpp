#include "audiodelegate.h"
#include "audiomodel.h"

#include <QTextDocument>

void AudioDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    Audio* audio = qvariant_cast<Audio*>(index.data());

    QString displayName = audio->displayName;

    painter->drawText(option.rect,displayName);
    painter->restore();
}

QSize AudioDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setHtml(optionV4.text);
    doc.setTextWidth(optionV4.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}
