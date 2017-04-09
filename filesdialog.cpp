#include "filesdialog.h"
#include "ui_filesdialog.h"

FilesDialog::FilesDialog(QVector<QUrl>* fileslist_, bool drag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilesDialog)
{
    ui->setupUi(this);
    this->fileslist = fileslist_;
    ui->listWidget->setFileslist(fileslist);
    ui->listWidget->dragEnable = drag;

    if(fileslist != nullptr)
    {
        for(QVector<QUrl>::const_iterator it = fileslist->begin();
            it != fileslist->end(); ++it)
        {
            ui->listWidget->addItem(it->fileName());
        }
    }
}

FilesDialog::~FilesDialog()
{
    delete ui;
}

