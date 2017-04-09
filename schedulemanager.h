#ifndef SCHEDULEMANAGER_H
#define SCHEDULEMANAGER_H

#include <QObject>
#include "utility.h"
#include "schedule.h"

class ScheduleManager: public QObject
{
    Q_OBJECT
public:
    ScheduleManager();
    void addSchedule(const Schedule& sche);
    void addScheduletoOneDay(const Schedule& sche, QDate date);
    void deleteSchedule(const Schedule& sche, QDate date, DeleteMode mode = DeleteMode::one);
    void editSchedule(Schedule before, Schedule after, QDate date);
    const QVector<Schedule>* getSchedule(QDate date);
    Schedule getSchedule(QDate date, QString name);
    const QMap<QDate, QVector<Schedule>>* getAllSchedule();

private:
    QMap<QDate, QVector<Schedule>>* allSchedule;

};

#endif // SCHEDULEMANAGER_H
