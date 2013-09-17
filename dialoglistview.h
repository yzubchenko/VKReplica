#ifndef DIALOGLISTVIEW_H
#define DIALOGLISTVIEW_H

#include <QListView>
#include <QObject>

class DialogListView : public QListView
{
    Q_OBJECT
public:
    explicit DialogListView(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *e);

signals:

public slots:

};

#endif // DIALOGLISTVIEW_H
