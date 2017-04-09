#ifndef LISHWIDGET_H
#define LISHWIDGET_H

#include "utility.h"
#include <QObject>
#include <QListWidget>

class ListWidget : public QListWidget
{
public:
    explicit ListWidget(QWidget *parent = 0);
    void setFileslist(QVector<QUrl> *fileslist_);
    bool dragEnable;

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QVector<QUrl>* fileslist;
};

#endif // LISHWIDGET_H
