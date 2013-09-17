#include "dialogmodel.h"
#include "messagedelegate.h"

#include <QStyleOptionViewItemV4>
#include <QStyle>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>
#include <QLabel>
#include <qabstractitemview.h>


QColor MessageDelegate::acquireBackgroundColor(Message* message) const
{
    QColor backgroundColor;
    if (message->isRead) {
        backgroundColor.setRgb(255,255,255);
    } else {
        backgroundColor.setRgb(225,231,237);
    }
    return backgroundColor;
}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 optionV4 = option;
    initStyleOption(&optionV4, index);

    //QStyledItemDelegate::paint(painter,optionV4,index);
    painter->save();
    Message* message = qvariant_cast<Message*>(index.data());

    QBrush backgroundColor = acquireBackgroundColor(message);

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
    font.setBold(true);

    QFontMetrics fm(font);

    QRect *fromDisplayNameRect = new QRect(optionV4.rect.left()+4,optionV4.rect.top()+4,(optionV4.rect.width()/2)-4,fm.height());
    QRect *dateRect = new QRect(fromDisplayNameRect->right(),optionV4.rect.top()+4,optionV4.rect.width()/2-8,fm.height());
    QRect *bodyRect = new QRect(optionV4.rect.left()+4,fromDisplayNameRect->bottom()+4,optionV4.rect.width()-4,optionV4.rect.height()-fm.height());

    painter->setFont(font);

    painter->fillRect(optionV4.rect, backgroundColor);

    font.setBold(true);
    painter->setFont(font);
    painter->setPen(fromDisplayNameColor);
    painter->drawText(*fromDisplayNameRect, Qt::AlignLeft, fromDisplayName);

    font.setBold(false);
    painter->setFont(font);
    painter->setPen(dateColor);
    painter->drawText(*dateRect, Qt::AlignRight, date);

    painter->setPen(bodyColor);
    painter->drawText(*bodyRect,Qt::AlignLeft|Qt::TextWordWrap, body);

    painter->restore();
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    QFont font = QApplication::font();
//    QFontMetrics fm(font);

    Message* message = qvariant_cast<Message*>(index.data());

    QAbstractItemView *view = dynamic_cast<QAbstractItemView*>(parent());
    QFontMetrics fontMetrics = view->fontMetrics();
    QRect messageRect = fontMetrics.boundingRect(message->body);
    int viewWidth = view->viewport()->width();
    int lineCount = (messageRect.width() / viewWidth);
    if (lineCount == 0 || messageRect.width()>viewWidth) {
        lineCount++;
    }
    return QSize(viewWidth, messageRect.height() * lineCount + fontMetrics.height()+12);

//    QAbstractItemView *view = dynamic_cast<QAbstractItemView*>(parent());
//       QFontMetricsF fontMetrics(view->fontMetrics());
//       QString str = message->body.trimmed();
//       QStringList words = str.split(QChar(' '));
//       QRectF boundingRect = fontMetrics.boundingRect(str);
//       int width = view->viewport()->width() - 6;
//       int times = 0;
//       while (words.size() > 0) {
//           times++;
//           qreal lineWidth = 0;
//           bool enoughSpace = true;
//           do {
//               QString word = words.first();
//               qreal wordWidth = fontMetrics.width(word);
//               if (wordWidth + lineWidth < width) {
//                   lineWidth += wordWidth;
//                   lineWidth += fontMetrics.width(QChar(' '));
//                   words.removeFirst();
//               } else
//                   enoughSpace = false;

//           } while (enoughSpace && words.size() > 0);
//       }
//       return QSize(width, boundingRect.height() * times - times + 1);
}
