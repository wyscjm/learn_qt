#ifndef TOOLTAB_H
#define TOOLTAB_H

#include <QWidget>
//#include "serialdialog.h"


//class QWidget;
class QLabel;
class QPushButton;
class QDialog;
//class SerialDialog;
//class SocketDialog;

//设备工具
class ToolTab : public QWidget
{
    Q_OBJECT

public:
    ToolTab(QWidget *parent = 0);

private:
    QLabel *serialtoollabel;
    QPushButton *serialtoolbutton;
    QLabel *sockettoollabel;
 //   QPushButton *sockettoolbutton;
    QPushButton *socketserverbutton;
    QPushButton *socketclientbutton;
//    SerialDialog *serialdialog;
private slots:
    void Openserialtool();
//    void Closeserialtool();
    void Opensocketservertool();
    void Opensocketclienttool();
};

#endif // TOOLTAB_H
