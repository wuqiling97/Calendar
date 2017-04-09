#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scheduledialog.h"
#include <windows.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{  
    ui->setupUi(this);
    calColor.append(QColor(255,255,255,0));
    calColor.append(QColor(255,0,0,120));
    calColor.append(QColor(255,255,120));
    calColor.append(QColor(0,255,0,120));
    calColor.append(QColor(0,255,255,120));
    calColor.append(QColor(0,100,255,120));
    calColor.append(QColor(255,0,255,120));
    //应用设置
    settings = new QSettings(INIpath, QSettings::IniFormat);
    language = settings->value("language", QString("Chinese")).toString();
    dragEnable = settings->value("dragEnable", true).toBool();
    qDebug()<<language;

    ui->checkBox->setChecked(dragEnable);

    if(language == "English")
    {
        setLocale(QLocale::English);
    }
    else
    {
        setLocale(QLocale::Chinese);
        qDebug()<<"come in install chinese";
        translator1 = new QTranslator;
        translator2 = new QTranslator;
        translator1->load("./chinese.qm");
        translator2->load("./qt_zh_CN.qm");
        qApp->installTranslator(translator1);
        qApp->installTranslator(translator2);

        ui->retranslateUi(this);

        language = settings->value("language", QString("Chinese")).toString();
    }
    //把保存的东西传入
    if(!ReadXML(XMLpath))
        mainScheMana = new ScheduleManager;

    if(language == "English")
    {
        qDebug()<<"set language box to english";
        ui->languageBox->setCurrentIndex(0);
    }
    else
    {
        qDebug()<<"set language box to chinese";
        ui->languageBox->setCurrentIndex(1);
    }
}

MainWindow::~MainWindow()
{
    settings->setValue("language", language);
    settings->setValue("dragEnable", dragEnable);
    WriteXML(XMLpath);
    delete settings;
    delete mainScheMana;
    delete ui;
}

void MainWindow::on_newButton_clicked()
{
    //新建schedule dialog,收集信息
    ScheduleDialog* dialog = new ScheduleDialog(this);
    dialog->exec(); //等待窗口关闭,才执行下一步
    if(dialog->result() == dialog->Rejected)
        return;

    QString memo = dialog->getmemo();
    RepeatPattern pattern = dialog->getpattern();
//    qDebug()<<memo<<"   "<<pattern;

    ui->listWidget->addItem(memo);
    QDate start = ui->calendar->selectedDate();
    QDate end;
    if(pattern == no)
        end = start;
    else
        end = ui->calendar->maximumDate();

    Schedule sche(memo, pattern, start, end);
    mainScheMana->addSchedule(sche);

    delete dialog;

}

void MainWindow::on_editButton_clicked()
{
    //获取当前所选日程,日期信息
    //未选中事件则返回
    int itemrow = ui->listWidget->currentRow();
    if(itemrow == -1) return;

    QListWidgetItem* item = ui->listWidget->currentItem();
    QDate date = ui->calendar->selectedDate();
    Schedule before = mainScheMana->getSchedule(date, item->text());

    //生成对话框获取修改信息
    ScheduleDialog* dialog = new ScheduleDialog(item->text(), before.repeatpat ,this);
    dialog->exec();
    //没点ok则返回
    if(dialog->result() == dialog->Rejected)
        return;

    QString memo = dialog->getmemo();
    RepeatPattern pattern = dialog->getpattern();

    ui->listWidget->takeItem(itemrow);
    ui->listWidget->addItem(memo);

    //数据库处理
    Schedule after = before;
    after.name = memo; after.repeatpat = pattern;
    if(before.repeatpat == no)
    {
        if(after.repeatpat == no)
        {
            after.start = after.end = before.start;
            mainScheMana->editSchedule(before, after, date);
        }
        else
        {
            after.start = before.start;
            after.end = ui->calendar->maximumDate();
            mainScheMana->editSchedule(before, after, date);
        }
    }
    else
    {//更改重复事件
        if(before.start == date || before.repeatpat == after.repeatpat)
        {
            //从事件的起始日期开始改
            mainScheMana->editSchedule(before, after, date);
        }
        else
        {
            Schedule newbefore = before;
            switch(before.repeatpat)
            {
            case everyday:
                newbefore.end = date.addDays(-1);
                break;
            case everyweek:
                newbefore.end = date.addDays(-7);
                break;
            case everymonth:
                newbefore.end = date.addMonths(-1);
                break;
            }
            mainScheMana->editSchedule(before, newbefore, date);
            after.start = date;
            after.end = ui->calendar->maximumDate();
            mainScheMana->addSchedule(after);
        }
    }

    delete dialog;
    delete item;
}

void MainWindow::on_deleteButton_clicked()
{
    //确定删除?
    QListWidgetItem* item = ui->listWidget->currentItem();
    int itemrow = ui->listWidget->currentRow();
    if(itemrow == -1) return;
    QDate date = ui->calendar->selectedDate();
    Schedule sche = mainScheMana->getSchedule(date, item->text());

    int ans = QMessageBox::warning
            (this, tr("Warning"), tr("Do you really want to delete?"),
            QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No, QMessageBox::No);

    switch(ans)
    {
    case QMessageBox::No:
        return;
        break;
    case QMessageBox::Yes:
        ui->listWidget->takeItem(itemrow);
        mainScheMana->deleteSchedule(sche, date);
        break;
    case QMessageBox::YesToAll:
        ui->listWidget->takeItem(itemrow);
        mainScheMana->deleteSchedule(sche, date, all);
        break;
    }
}

void MainWindow::on_calendar_clicked(const QDate &date)
{
    //读取事件信息,设置list widget内容
    const QVector<Schedule>* temp = mainScheMana->getSchedule(date);
    ui->listWidget->clear();
    if(temp != nullptr)
    {
        for(int i=0; i<temp->size(); ++i)
        {
            if((*temp)[i].deleteFlag == false)
                ui->listWidget->addItem((*temp)[i].name);
        }
    }

    //读取颜色信息,设置combobox
    QTextCharFormat format = ui->calendar->dateTextFormat(date);
    QColor color = format.background().color();
//    qDebug("rgba = %d, %d, %d, %d", color.red(), color.green(), color.blue(), color.alpha());
    int index;
    for(index=0; index<calColor.size(); ++index)
    {
        if(calColor[index] == color)
            break;
    }
    if(index == calColor.size())
        index = 0;
//    qDebug()<<"index = "<<index;

    ui->colorBox->setCurrentIndex(index);
}

void MainWindow::on_colorBox_currentIndexChanged(int index)
{
    if(index == -1)
        return;

    QDate date = ui->calendar->selectedDate();
    QTextCharFormat format;
    format.setBackground(QBrush(calColor[index]));
    ui->calendar->setDateTextFormat(date, format);

    if(index != 0)//自行保存颜色, 方便写xml
        datescolor[date] = index;
    else if(datescolor.contains(date))
    {
        datescolor.remove(date);
    }
}

//锁定窗口
void MainWindow::on_actionLock_triggered()
{
    locked = !locked;
//    hide();
    ui->Frame->setVisible(!locked);
    ui->menuBar->setVisible(!locked);
    ui->listWidget->setEnabled(!locked);

    if(locked)
    {
        ui->actionLock->setText(tr("Unlock"));
        ui->actionLock->setToolTip(tr("Press to unlock"));

        setWindowOpacity(0.6);
//        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(),
                    GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
        show();

    }
    else
    {
        ui->actionLock->setText(tr("Lock"));
        ui->actionLock->setToolTip(tr("Press to lock"));

        setWindowOpacity(0.95);
//        setAttribute(Qt::WA_TranslucentBackground, false);
        setWindowFlags(Qt::MSWindowsOwnDC);
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(),
                    GWL_EXSTYLE) | WS_EX_LAYERED);
        show();
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"come in mouse move event"<<endl<<event->pos();
//    if(locked)
//    {
////        QPoint position = ui->toolBar->pos();
////        int x1 = position.x();
////        int y1 = position.y();
//        int x1 = 0; int y1 = 0;
//        int x2 = x1 + 100; //48为本机上该按钮的宽度
//        int y2 = y1 + ui->toolBar->height();
//        QPoint mouse = event->pos();
//        qDebug("(%d, %d), (%d, %d)", x1, y1, x2, y2);
//        //需要严格的在toolbar内,不能在边界上
//        if(mouse.x()>x1 && mouse.x()<x2 && mouse.y()>y1 && mouse.y()<y2)
//        {
//            qDebug()<<"in unlock button";
//            SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(),
//                          GWL_EXSTYLE) | WS_EX_LAYERED);
//        }
//        else
//            SetWindowLong((HWND)winId(), GWL_EXSTYLE,
//                          GetWindowLong((HWND)winId(), GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);

//    }
    QMainWindow::mouseMoveEvent(event);
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    dragEnable = checked;
    ui->calendar->dragEnable = dragEnable;
    qDebug()<<dragEnable;
}


void MainWindow::on_languageBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "English")
    {
        language = arg1;
    }
    else if(arg1 == "中文")
    {
        language = "Chinese";
    }

}

void MainWindow::on_actionSave_triggered()
{
    settings->setValue("language", language);
    settings->setValue("dragEnable", dragEnable);
    settings->sync();

    WriteXML(XMLpath);
}

bool MainWindow::ReadXML(QString dir)
{
    qDebug()<<"come in read xml";
    QDomDocument document;
    QFile file(dir);
    if(!file.open(QIODevice::ReadOnly))//只读,失败返回
    {
        qDebug()<<"open failed";
        return false;
    }

    if(!document.setContent(&file))
    {
        qDebug()<<"setcontent failed";
        return false;
    }

    //清空mainschemana
    if(mainScheMana!=nullptr)
        delete mainScheMana;
    mainScheMana = new ScheduleManager;
    //清空日期颜色
    ui->calendar->setDateTextFormat(QDate(), QTextCharFormat());
    //清空自己记的颜色
    datescolor.clear();

    QDomElement root = document.documentElement();

    //read date schedule
    QDomElement son = root.firstChildElement("dateschedule");
    QDomNodeList dateslist = son.childNodes();//<date id="">list
    //读取date
    for(int i=0; i<dateslist.count(); ++i)
    {
        QDomElement date_tag = dateslist.item(i).toElement();//<date id="2010-10-10">
        QDate date = QDate::fromString(date_tag.attribute("id"), Qt::ISODate);//id -> date
        QDomNodeList scheduleslist = date_tag.childNodes(); //<schedule>list
        for(int j=0; j<scheduleslist.count(); ++j)
        {
            //记录日程信息
            QDomElement sche_tag = scheduleslist.item(j).toElement();//<schedule>
            Schedule sche;
            sche.name = sche_tag.firstChildElement("name").text();
            int repeatpat = sche_tag.firstChildElement("reppat").text().toInt();
            switch(repeatpat)
            {
            case 0: sche.repeatpat = no; break;
            case 1: sche.repeatpat = everyday; break;
            case 2: sche.repeatpat = everyweek; break;
            case 3: sche.repeatpat = everymonth; break;
            }
            sche.start = QDate::fromString(sche_tag.firstChildElement("start").text(), Qt::ISODate);
            sche.end = QDate::fromString(sche_tag.firstChildElement("end").text(), Qt::ISODate);
            sche.deleteFlag = sche_tag.firstChildElement("delete").text().toInt();
//            qDebug()<<date<<"  delete = "<<sche.deleteFlag;

            mainScheMana->addScheduletoOneDay(sche, date);
        }
    }

    //read date color
    son = root.firstChildElement("datecolor"); //<datecolor>
    QDomNodeList datecolorlist = son.childNodes();
    for(int i=0; i<datecolorlist.count(); ++i)
    {
        QDomElement date_tag = datecolorlist.item(i).toElement();//<date id="" color="">
        QDate date = QDate::fromString(date_tag.attribute("id"), Qt::ISODate);
        int colorindex = date_tag.attribute("color").toInt();

        if(colorindex != 0)//不是白色就存下来
            datescolor[date] = colorindex;

        QTextCharFormat format;
        format.setBackground(QBrush(calColor[colorindex]));
        ui->calendar->setDateTextFormat(date, format);
    }

    file.close();
    return true;
}

bool MainWindow::WriteXML(QString dir)
{
    qDebug()<<"come in write xml";
    QFile file(dir);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"write xml open file failed";
        return false;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0", true);
    writer.writeStartElement("calendar");                       //<calendar>
        writer.writeStartElement("dateschedule");               //<dateschedule>
            const QMap<QDate, QVector<Schedule>>* allSchedule =
                    mainScheMana->getAllSchedule();
            for(QMap<QDate, QVector<Schedule>>::const_iterator i = allSchedule->begin();
                i != allSchedule->end(); ++i)
            {
                writer.writeStartElement("date");               //<date id="">
                writer.writeAttribute("id", i.key().toString(Qt::ISODate));

                const QVector<Schedule>& schelist = i.value();
                for(int j=0; j<schelist.size(); ++j)
                {
                    const Schedule& sche = schelist[j];
                    writer.writeStartElement("schedule");       //<schedule>
                    writer.writeTextElement("name", sche.name);
                    writer.writeTextElement("reppat", QString::number(sche.repeatpat));
                    writer.writeTextElement("start", sche.start.toString(Qt::ISODate));
                    writer.writeTextElement("end", sche.end.toString(Qt::ISODate));
                    writer.writeTextElement("delete", QString::number(sche.deleteFlag));
                    writer.writeEndElement();
                }

                writer.writeEndElement();
            }
        writer.writeEndElement();

        writer.writeStartElement("datecolor");//<datecolor>
            for(QMap<QDate, int>::const_iterator i = datescolor.begin();
                i != datescolor.end(); ++i)
            {
                writer.writeStartElement("date");
                writer.writeAttribute("id", i.key().toString(Qt::ISODate));
                writer.writeAttribute("color", QString::number(i.value()));
                writer.writeEndElement();
            }
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
    qDebug()<<"write success";
    return true;
}

void MainWindow::on_actionImport_XML_triggered()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Select XML"), "/", "(*.xml)");
    ReadXML(dir);
}

void MainWindow::on_actionExport_XML_triggered()
{
    QString dir = QFileDialog::getSaveFileName(this, tr("Select XML"), "/", "(*.xml)");
    WriteXML(dir);
}
