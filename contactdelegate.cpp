#include "contactdelegate.h"
#include "contactmodel.h"
#include <QStyleOptionViewItemV4>
#include <QStyle>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>

void ContactDelegate::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    Contact* contact = qvariant_cast<Contact*>(index.data());
    QIcon statusIcon;
    if (contact->hasUnreadMessage) {
        statusIcon = QIcon(":/contacts/resources/msg.png");
    } else {
        if (contact->isOnline) {
            statusIcon = QIcon(":/contacts/resources/online.png");
        } else {
            statusIcon = QIcon(":/contacts/resources/offline.png");
        }
    }
    QString displayName = contact->displayName;

    QFont font = QApplication::font();
    QColor color;
    if (contact->isOnline) {
        color.setRgb(0,0,0);
    } else {
        color.setRgb(128,128,128);
    }
    QFontMetrics fm(font);


    QRect statusIconRect = option.rect;
    QRect displayNameRect = option.rect;


    QSize statusIconsize = statusIcon.actualSize(option.decorationSize);
    statusIconRect.setLeft(option.rect.left()+4);
    statusIconRect.setRight(statusIconRect.left()+16);
    statusIconRect.setTop(option.rect.top()+(option.rect.height() - 16)/2);

    displayNameRect.setLeft(statusIconRect.right()+6);
    displayNameRect.setTop(displayNameRect.top()+(option.rect.height() -fm.height())/2);
    displayNameRect.setBottom(displayNameRect.top()+fm.height());

    painter->drawPixmap(QPoint(statusIconRect.left(),statusIconRect.top())
                        ,statusIcon.pixmap(statusIconsize.width(),statusIconsize.height()));

    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(displayNameRect,displayName);
    painter->restore();
}

QSize ContactDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setHtml(optionV4.text);
    doc.setTextWidth(optionV4.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}
