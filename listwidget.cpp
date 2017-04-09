#include "listwidget.h"

ListWidget::ListWidget
(QWidget* parent):
    QListWidget(parent)
{

}

void ListWidget::setFileslist(QVector<QUrl> *fileslist_)
{
    fileslist = fileslist_;
}

void ListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mouseMoveEvent(event);

    //获取所选file
    qDebug()<<"list widget  "<<dragEnable;
    if(!dragEnable)
        return;

    int index = currentRow();
    QUrl url = (*fileslist)[index];
    qDebug()<<url.toString();
//    QString path = url.path();
//    qDebug()<<"path = "<<path;
//    path = "file:///" + path;
//    qDebug()<<"path="<<path;
//    url.setUrl(path);

    QMimeData* QD = new QMimeData;
    QList<QUrl> urls;
    urls.append(url);
    QD->setUrls(urls);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(QD);
    drag->start();
    qDebug()<<urls;
}
