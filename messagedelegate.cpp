#include "dialogmodel.h"
#include "messagedelegate.h"

#include <QStyleOptionViewItemV4>
#include <QStyle>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>
#include <QLabel>


QBrush MessageDelegate::acquireBackgroundBrush(Message* message) const
{
    QBrush backgroundBrush;
    QColor color;
    if (message->isRead) {
        color.setRgb(255,255,255);
    } else {
        color.setRgb(225,231,237);
    }
    backgroundBrush.setColor(color);

    return backgroundBrush;
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    QStyledItemDelegate::paint(painter,optionV4,index);
    painter->save();
    Message* message = qvariant_cast<Message*>(index.data());

    QBrush backgroundBrush = acquireBackgroundBrush(message);

    QString fromDisplayName = message->fromDisplayName;
    QColor fromDisplayNameColor;
    fromDisplayNameColor.setRgb(43,88,122);

    QString date = message->date;
    QColor dateColor;
    dateColor.setRgb(153,153,153);

    QString body = message->body;
    QColor bodyColor;
    bodyColor.setRgb(0,0,0);

    QFont font = QApplication::font();
    QFontMetrics fm(font);
    font.setBold(true);



    QRect fromDisplayNameRect = optionV4.rect;
    QRect dateRect = optionV4.rect;
    QRect bodyRect = optionV4.rect;

    bodyRect.setTop(dateRect.top() + fm.height() + 4);

    painter->setFont(font);
    painter->setBackground(backgroundBrush);

    font.setBold(true);
    painter->setFont(font);
    painter->setPen(fromDisplayNameColor);
    painter->drawText(fromDisplayNameRect, Qt::AlignLeft, fromDisplayName);

    font.setBold(false);
    painter->setFont(font);
    painter->setPen(dateColor);
    painter->drawText(dateRect, Qt::AlignRight, date);

    painter->setPen(bodyColor);
    painter->drawText(bodyRect, body);

    painter->restore();
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QFont font = QApplication::font();
    QFontMetrics fm(font);


    Message* message = qvariant_cast<Message*>(index.data());

//    int bodyHeight;
//    if (message->body.length() > 0) {
//        int bw = fm.width(message->body);
//        QRect r =option.rect;
//        int rw = r.width();
//        bodyHeight = fm.width(message->body) / option.rect.width();
//    } else {
//        bodyHeight = fm.height();
//    }

    //QStyleOptionViewItemV4 optionV4 = option;
    //initStyleOption(&optionV4, index);

    QTextDocument doc;
    doc.setDefaultFont(font);
    doc.setHtml(message->body);
    doc.setTextWidth(option.rect.width());

    QAbstractItemView *view = dynamic_cast<QAbstractItemView*>(parent());
    QFontMetrics fontMetrics = view->fontMetrics();
    QRect rect = fontMetrics.boundingRect(index.data().toString());
    int proportion = (rect.width() / (view->width() + 4));
    if (proportion == 0 || rect.width() > view->width())
        proportion++;

    return QSize(view->width() - 4, rect.height() * proportion);

    return QSize(doc.idealWidth(), doc.size().height()+fm.height()+4);
}
