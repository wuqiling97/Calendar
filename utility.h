#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QPoint>
#include <QTextCharFormat>

#include <QMap>
#include <QVector>
#include <QList>

#include <QMessageBox>
#include <QWidget>
#include <QString>
#include <QDate>
#include <QPixmap>
#include <QPicture>

#include <QDrag>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QResizeEvent>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include <QSettings>
#include <QLocale>

enum RepeatPattern{no, everyday, everyweek, everymonth};
enum DeleteMode{one, all};
enum {white, red, yellow, green, cyan, blue, purple};

static QVector<QColor> calColor;


//const QColor white(255,255,255,0);
//const QColor red(255,0,0,120);
//const QColor yellow(255,255,120);
//const QColor green(0,255,0,120);
//const QColor cyan(0,255,255,120);
//const QColor blue(0,0,255,120);
//const QColor purple(255,0,255,120);


#endif // UTILITY_H
