#include "mycalendar.h"
#include <QDir>

MyCalendar::MyCalendar(QWidget *parent):
    QCalendarWidget::QCalendarWidget(parent)
{
    connect(this, SIGNAL(currentPageChanged(int,int)),
            this, SLOT(resetDatepos()));
    connect(this, SIGNAL(activated(QDate)),
            this, SLOT(showFiles(QDate)));

    filesname = new QMap<QDate, QVector<QUrl>>;
//    toDir = "C:/Users/Wuql/Qt/Calendar/files/";
    QDir dir;
    toDir = dir.currentPath();
    toDir += "/files/";
    filePen.setColor(Qt::blue);

    setAcceptDrops(true);
}

void MyCalendar::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
//    qDebug()<<"come in drag enter event"<<endl<<dragEnable;
    if ((event->mimeData()->hasFormat("text/uri-list")) && dragEnable)
        event->acceptProposedAction();
}

void MyCalendar::dropEvent(QDropEvent *event)
{
    QDate date;
    QPoint droppos = event->pos();
    //鼠标drop到了哪一天
    for(QMap<QDate, QRect>::iterator it = datespos.begin();
        it != datespos.end(); ++it)
    {
        if(isPointInRect(droppos, it.value()))
        {
            date = it.key();
            break;
        }
    }
//    qDebug()<<"drag to date "<<date;

    //把拖入的文件路径加入map:filesname中,并拷贝到当前目录
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls)
    {
        qDebug()<<url.fileName();
        QString target = toDir + url.fileName();
        qDebug()<<target;

        bool success = QFile::copy(url.toLocalFile(), target);
        qDebug()<<success;
        url.setPath(target);
        qDebug()<<url.toLocalFile();
        if(!(*filesname)[date].contains(url))
            (*filesname)[date].append(url);
        this->update();
    }
}

void MyCalendar::resizeEvent(QResizeEvent *event)
{
    datespos.clear();
//    for(int i=0; i<10; ++i)
//        qDebug()<<"in resizeevent reset";
    QCalendarWidget::resizeEvent(event);
}

void MyCalendar::paintCell
(QPainter *painter, const QRect &rect, const QDate &date) const
{
//    qDebug()<<"come in paintcell";
    if(!datespos.contains(date))
    {
        datespos[date] = rect;
//        qDebug()<<date<<rect;
    }
    QCalendarWidget::paintCell(painter, rect, date);

    //在日期中显示文件名
    QString datefilename; //日期中保存的文件
    const QVector<QUrl>& filelist = (*filesname)[date];
    for(QVector<QUrl>::const_iterator it = filelist.begin();
        it != filelist.end(); ++it)
    {
        datefilename += (it->fileName() + "\n");
    }

    painter->setPen(filePen);
    painter->drawText(rect, datefilename);
}

void MyCalendar::resetDatepos()
{
    datespos.clear();
}

void MyCalendar::showFiles(QDate date)
{
    QVector<QUrl>* fileslist;
    if(filesname->contains(date))
        fileslist = &(*filesname)[date];
    else
        fileslist = nullptr;

    FilesDialog* dialog = new FilesDialog(fileslist, dragEnable, this);
    dialog->exec();

    delete dialog;
}

bool MyCalendar::isPointInRect(const QPoint &point, const QRect &rect)
{
    int x1 = rect.x();
    float y1_ = float(rect.y()) + 22; //header height
    int y1 = y1_;
    int x2 = x1 + rect.width();
    int y2 = y1 + rect.height();
    return (point.x()>=x1 && point.x()<=x2 && point.y()>=y1 && point.y()<=y2);
}
