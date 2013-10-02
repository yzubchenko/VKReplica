#include "audiodelegate.h"
#include "audiomodel.h"

#include <QTextDocument>
#include <QApplication>

void AudioDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    Audio* audio = qvariant_cast<Audio*>(index.data());
    QString displayName = audio->displayName;

    QIcon playingIcon;
    if (audio->isPlaying) {
        playingIcon = QIcon(":/audioplayer/resources/playPlayer.png");
    } else {
        playingIcon = QIcon();
    }

    QFont font = QApplication::font();
    QFontMetrics fm(font);

    QRect playingIconRect = option.rect;
    QRect displayNameRect = option.rect;


    QSize playingIconsize = QSize(11,11);
    playingIconRect.setLeft(option.rect.left()+4);
    playingIconRect.setRight(playingIconRect.left()+playingIconsize.width()+4);
    playingIconRect.setTop(option.rect.top()+(option.rect.height() - playingIconsize.height())/2);

    displayNameRect.setLeft(playingIconRect.right());
    displayNameRect.setTop(displayNameRect.top()+(option.rect.height() - fm.height())/2);
    displayNameRect.setBottom(displayNameRect.top()+fm.height());

    painter->drawPixmap(QPoint(playingIconRect.left(),playingIconRect.top())
                        ,playingIcon.pixmap(playingIconsize.width(),playingIconsize.height()));


    painter->setFont(font);
    painter->drawText(displayNameRect,displayName);

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
