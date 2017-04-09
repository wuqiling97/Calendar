#ifndef FILESDIALOG_H
#define FILESDIALOG_H

#include <QDialog>
#include "utility.h"

namespace Ui {
class FilesDialog;
}

class FilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilesDialog(QVector<QUrl> *fileslist_, bool drag, QWidget *parent = 0);
    ~FilesDialog();



private:
    Ui::FilesDialog *ui;
    QVector<QUrl>* fileslist;
};

#endif // FILESDIALOG_H
