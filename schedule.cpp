#include "schedule.h"


Schedule::Schedule(QString name, RepeatPattern repeatpat, QDate start, QDate end, bool deleteFlag)
{
    this->name = name;
    this->repeatpat = repeatpat;
    this->start = start;
    this->end = end;
    this->deleteFlag = deleteFlag;
}

Schedule::Schedule()
{

}

Schedule::Schedule(const Schedule &src)
{
    name = src.name;
    repeatpat = src.repeatpat;
    start = src.start;
    end = src.end;
    deleteFlag = src.deleteFlag;
}

bool Schedule::operator==(const Schedule& right)
{
    return (name==right.name &&
            repeatpat==right.repeatpat &&
            start==right.start && end==right.end);
}
