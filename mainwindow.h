#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utility.h"
#include "schedulemanager.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QXmlStreamWriter>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString language;

protected:
    void mouseMoveEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
    ScheduleManager* mainScheMana = nullptr;
    bool locked = false;
    QSettings* settings = nullptr;
    bool dragEnable;
    bool ReadXML(QString dir);
    bool WriteXML(QString dir);
    QMap<QDate, int> datescolor;
    QString XMLpath = "./data.xml";
    QString INIpath = "./settings.ini";
    QTranslator* translator1;
    QTranslator* translator2;


public slots:
    void on_newButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();

private slots:
    void on_calendar_clicked(const QDate &date);
    void on_colorBox_currentIndexChanged(int index);
    void on_actionLock_triggered();
    void on_checkBox_toggled(bool checked);
    void on_languageBox_currentTextChanged(const QString &arg1);
    void on_actionSave_triggered();
    void on_actionImport_XML_triggered();
    void on_actionExport_XML_triggered();
};

#endif // MAINWINDOW_H
