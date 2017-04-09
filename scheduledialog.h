#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include <QDialog>
#include "utility.h"

namespace Ui {
class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    //mode为0 new, mode为1 edit
    explicit ScheduleDialog(QWidget *parent = 0);
    explicit ScheduleDialog(QString ex_memo, RepeatPattern ex_pattern, QWidget *parent = 0);
    ~ScheduleDialog();
    QString getmemo() const;
    RepeatPattern getpattern() const;

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::ScheduleDialog *ui;
    QString memo;
    RepeatPattern pattern;

signals:
    void return_sche_info(QString, RepeatPattern);

public slots:
    void accept();
//    void reject();

};

#endif // SCHEDULEDIALOG_H
