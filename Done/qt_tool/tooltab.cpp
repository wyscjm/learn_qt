#include <QtGui>
//#include <QObject>

#include "tooltab.h"
#include "serialdialog.h"
#include "tcpserverdialog.h"
#include "tcpclientdialog.h"


ToolTab::ToolTab(QWidget *parent)
    :QWidget(parent)
{
    QString str;
    serialtoollabel = new QLabel;
    serialtoolbutton = new QPushButton;
    str = tr("串口工具：");
    serialtoollabel->setText(str);
    str = tr("打开");
    serialtoolbutton->setText(str);

    sockettoollabel = new QLabel;
//    sockettoolbutton = new QPushButton;
    socketserverbutton = new QPushButton;
    socketclientbutton = new QPushButton;
    str = tr("网络工具：");
    sockettoollabel->setText(str);
    str = tr("服务器打开");
    socketserverbutton->setText(str);
    str = tr("客户端打开");
    socketclientbutton->setText(str);



    QHBoxLayout *seriallayout = new QHBoxLayout;
    seriallayout->addWidget(serialtoollabel);
    seriallayout->addWidget(serialtoolbutton);

    QHBoxLayout *socketlayout = new QHBoxLayout;
    socketlayout->addWidget(sockettoollabel);
    socketlayout->addWidget(socketserverbutton);
    socketlayout->addWidget(socketclientbutton);

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(seriallayout);
    mainlayout->addLayout(socketlayout);
    mainlayout->addWidget(new QLineEdit);

    connect(serialtoolbutton,SIGNAL(clicked()),
           this,SLOT(Openserialtool()));
    connect(socketserverbutton,SIGNAL(clicked()),
            this,SLOT(Opensocketservertool()));
    connect(socketclientbutton,SIGNAL(clicked()),
            this,SLOT(Opensocketclienttool()));

    setLayout(mainlayout);
}


void ToolTab::Openserialtool()
{
    /*
    //模态
    if(!serialdialog)
    {
        serialdialog = new SerialDialog(this);
        serialdialog->show();
        serialtoolbutton->setEnabled(0);
    }
    */
    //非模态
    SerialDialog *dialog = new SerialDialog(this);
    if(dialog->exec()){

    }
    delete dialog;
}

void ToolTab::Opensocketservertool()
{
    TcpServerDialog *dialog = new TcpServerDialog(this);
    if(dialog->exec()){

    }
    delete dialog;
}

void ToolTab::Opensocketclienttool()
{
    TcpClientDialog *dialog = new TcpClientDialog(this);
    if(dialog->exec()){

    }
    delete dialog;
}



