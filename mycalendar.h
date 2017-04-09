#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QCalendarWidget>
#include "utility.h"
#include "filesdialog.h"

class MyCalendar : public QCalendarWidget
{
    Q_OBJECT
public:
    MyCalendar(QWidget* parent = 0);
    bool dragEnable;

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void resizeEvent(QResizeEvent* event);
    virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
    QMap<QDate, QVector<QUrl>>* filesname;
    mutable QMap<QDate, QRect> datespos;
    QString toDir;
    QPen filePen;
    bool isPointInRect(const QPoint& point, const QRect& rect);

public slots:
    void resetDatepos();
    void showFiles(QDate date);

};

#endif // MYCALENDAR_H
