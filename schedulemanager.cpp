#include "schedulemanager.h"
#include "scheduledialog.h"

ScheduleManager::ScheduleManager()
{
    allSchedule = new QMap<QDate, QVector<Schedule>>;
}

void ScheduleManager::addScheduletoOneDay
(const Schedule &sche, QDate date)
{
    //date想加入日程的日期
    if(allSchedule->contains(date))
    {   //map中这一天有事件
        (*allSchedule)[date].append(sche);
//        //取出vector,插入新事件
//        QVector<Schedule> temp = allSchedule[date];
//        temp.append(sche);
//        //删除原map元素
//        allSchedule->erase(it_start);
//        allSchedule[sche.start] = temp;
    }
    else
    {   //map中当天无事件
        QVector<Schedule> temp;
        temp.append(sche);
        (*allSchedule)[date] = temp;
    }
}

void ScheduleManager::addSchedule(const Schedule &sche)
{
    switch(sche.repeatpat)
    {
    case no:
        //不重复
        addScheduletoOneDay(sche, sche.start);
        break;
    case everyday:
    //隔几天插入一个事件
        for(QDate i = sche.start; i<=sche.end; i=i.addDays(1))
        {
            addScheduletoOneDay(sche, i);
        }
        break;
    case everyweek:
        for(QDate i = sche.start; i <= sche.end; i=i.addDays(7))
        {
            addScheduletoOneDay(sche, i);
        }
        break;
    case everymonth:
        for(QDate i = sche.start; i <= sche.end; i=i.addMonths(1))
        {
            addScheduletoOneDay(sche, i);
        }
        break;
    }

}

void ScheduleManager::deleteSchedule
(const Schedule &sche, QDate date, DeleteMode mode)
{
    //如果当天没有事件, 会把map中的该天删除
    if(sche.repeatpat == no)
    {
        //用引用取出来,删除
        QVector<Schedule>& temp = (*allSchedule)[sche.start];
        //查找序号, 删除
        int index = temp.indexOf(sche);
        if(index == -1)
            qDebug()<<"unexisted schedule, don't repeat";
        else
        {
            temp.remove(index);
            if(temp.size() == 0)
                allSchedule->remove(date);
        }
    }
    else if(mode == one)
    {   //删序列事件中的单个
        QVector<Schedule>& temp = (*allSchedule)[date];
        int index = temp.indexOf(sche);
        if(index == -1)
            qDebug()<<"unexsited schedule, repeat pattern ="
                   <<sche.repeatpat<<"  delete one";
        else
            temp[index].deleteFlag = true;
    }
    else
    {
        //从start日开始,按照重复模式删
        QDate i =sche.start;
        while(i <= sche.end)
        {
            QVector<Schedule>& temp = (*allSchedule)[i];
            int index = temp.indexOf(sche);
            if(index == -1)
                qDebug()<<"unexsited schedule, repeat pattern "<<sche.repeatpat;
            else
            {
                temp.remove(index);
                if(temp.size() == 0)
                    allSchedule->remove(i);
            }
            //i递增
            switch(sche.repeatpat)
            {
            case everyday:
                i=i.addDays(1);
                break;
            case everyweek:
                i=i.addDays(7);
                break;
            case everymonth:
                i=i.addMonths(1);
                break;
            }
        }
    }
}

void ScheduleManager::editSchedule(Schedule before, Schedule after, QDate date)
{

    if(before.repeatpat == no)
    {   //先删后加
        deleteSchedule(before, before.start);
        addSchedule(after);
    }
    else //if(before.repeatpat == after.repeatpat)
    {
        deleteSchedule(before, before.start, all);
        addSchedule(after);
    }
}

Schedule ScheduleManager::getSchedule(QDate date, QString name)
//查找某天的某事件
{
    //无找不到的处理机制
    QVector<Schedule>& temp = (*allSchedule)[date];
    QVector<Schedule>::iterator it = temp.begin();
    for(; it!=temp.end(); ++it)
    {
        if(it->name == name)
        {
            return *it;
        }
    }
}

const QVector<Schedule>* ScheduleManager::getSchedule(QDate date)
{
    if(allSchedule->contains(date))
    {
        return &(*allSchedule)[date];
    }
    return nullptr;
}

const QMap<QDate, QVector<Schedule>>* ScheduleManager::getAllSchedule()
{
    return allSchedule;
}
