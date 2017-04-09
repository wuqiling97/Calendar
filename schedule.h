#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "utility.h"

class Schedule
{
public:
    QString name;
    RepeatPattern repeatpat;
    QDate start, end;
    bool deleteFlag = 0;

    bool operator==(const Schedule& right);
    Schedule(QString name, RepeatPattern repeatpat, QDate start, QDate end, bool deleteFlag = 0);
    Schedule();
    Schedule(const Schedule& src);

};

#endif // SCHEDULE_H
