#include "contactdelegate.h"
#include "contactmodel.h"
#include <QStyleOptionViewItemV4>
#include <QStyle>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>

void ContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    Contact* contact = qvariant_cast<Contact*>(index.data());
    QIcon* statusIcon;
    if (contact->hasUnreadMessage) {
        statusIcon = new QIcon(":/contacts/resources/msg.png");
    } else {
        if (contact->isOnline) {
            statusIcon = new QIcon(":/contacts/resources/online.png");
        } else {
            statusIcon = new QIcon(":/contacts/resources/offline.png");
        }
    }
    QString displayName = contact->displayName;

    QFont font = QApplication::font();
    QColor color;
    if (contact->isOnline) {
        color.setRgb(0,100,0);
    } else {
        color.setRgb(128,128,128);
    }
    QFontMetrics fm(font);


    QRect statusIconRect = option.rect;
    QRect displayNameRect = option.rect;


    QSize statusIconsize = statusIcon->actualSize(option.decorationSize);
    statusIconRect.setLeft(option.rect.left()+4);
    statusIconRect.setRight(statusIconRect.left()+16);
    statusIconRect.setTop(option.rect.top()+(option.rect.height() - 16)/2);

    displayNameRect.setLeft(statusIconRect.right()+6);
    displayNameRect.setTop(displayNameRect.top()+(option.rect.height() -fm.height())/2);
    displayNameRect.setBottom(displayNameRect.top()+fm.height());

    painter->drawPixmap(QPoint(statusIconRect.left(),statusIconRect.top())
                        ,statusIcon->pixmap(statusIconsize.width(),statusIconsize.height()));

    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(displayNameRect,displayName);
//    QIcon icon = qvariant_cast<QIcon>(index.data(IconRole));
//    QString headerText = qvariant_cast<QString>(index.data(headerTextRole));
    painter->restore();
//    QStyleOptionViewItemV4 optionV4 = option;
//    initStyleOption(&optionV4, index);

//    QStyle *style = optionV4.widget? optionV4.widget->style() : QApplication::style();

//    QTextDocument doc;
//    doc.setHtml(optionV4.text);

//    /// Painting item without text
//    optionV4.text = QString();
//    style->drawControl(QStyle::CE_ItemViewItem, &optionV4, painter);

//    QAbstractTextDocumentLayout::PaintContext ctx;

//    // Highlighting text if item is selected
//    if (optionV4.state & QStyle::State_Selected)
//        ctx.palette.setColor(QPalette::Text, optionV4.palette.color(QPalette::Active, QPalette::HighlightedText));

//    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &optionV4);
//    painter->save();
//    painter->translate(textRect.topLeft());
//    painter->setClipRect(textRect.translated(-textRect.topLeft()));
//    doc.documentLayout()->draw(painter, ctx);
//    painter->restore();
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
