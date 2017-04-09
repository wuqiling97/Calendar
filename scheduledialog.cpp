#include "scheduledialog.h"
#include "ui_scheduledialog.h"

ScheduleDialog::ScheduleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    ui->radioButton_0->setChecked(true);
}

ScheduleDialog::ScheduleDialog
(QString ex_memo, RepeatPattern ex_pattern, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    ui->memo->setText(ex_memo);
    switch(ex_pattern)
    {
    case no:
        ui->radioButton_0->setChecked(true);
        break;
    case everyday:
        ui->radioButton_1->setChecked(true);
        break;
    case everyweek:
        ui->radioButton_2->setChecked(true);
        break;
    case everymonth:
        ui->radioButton_3->setChecked(true);
        break;
    }
}

ScheduleDialog::~ScheduleDialog()
{
    delete ui;
}

void ScheduleDialog::closeEvent(QCloseEvent *event)
{
    QDialog::closeEvent(event);
}

void ScheduleDialog::accept()
{
    memo = ui->memo->text();
    if(ui->radioButton_0->isChecked()) pattern = no;
    if(ui->radioButton_1->isChecked()) pattern = everyday;
    if(ui->radioButton_2->isChecked()) pattern = everyweek;
    if(ui->radioButton_3->isChecked()) pattern = everymonth;
    emit return_sche_info(memo, pattern);
    QDialog::accept();
}

QString ScheduleDialog::getmemo() const
{
    return memo;
}

RepeatPattern ScheduleDialog::getpattern() const
{
    return pattern;
}
