#include <QtGui>
#include <QTcpSocket>
#include "tcpclientdialog.h"
#include "ui_tcpclientdialog.h"

TcpClientDialog::TcpClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClientDialog)
{
    ui->setupUi(this);



    QStringList ip_list;
    ip_list.clear();
    ip_list<<"127.0.0.1"<<"192.168.0.30"<<"192.168.0.250";
    ui->remoteipcombox->addItems(ip_list);
    ui->remoteipcombox->setCurrentIndex(0);

    QStringList port_list;
    port_list.clear();
    port_list<<"8000"<<"8001"<<"8002";
    ui->portcombox->clear();
    ui->portcombox->addItems(port_list);
    ui->portcombox->setCurrentIndex(1);

    ui->openbutton->setText(tr("连接服务器"));
    ui->sendbutton->setEnabled(false);
    /*

    im = new InputMethod;
    ui->remoteipcombox->installEventFilter(im);
    ui->portcombox->installEventFilter(im);
    ui->sendline->installEventFilter(im);

    ui->remoteipcombox->setObjectName("remoteip");
    ui->portcombox->setObjectName("port");
    ui->sendline->setObjectName("sendline");

    connect(im,SIGNAL(set_im_objectname(QString)),this,SLOT(create_im_connect(QString)));
    */
}

TcpClientDialog::~TcpClientDialog()
{
    delete ui;
}

void TcpClientDialog::on_openbutton_clicked()
{
    quint16 port = ui->portcombox->currentText().toUInt();
    if(ui->openbutton->text() == tr("连接服务器"))
    {
        tcpsocket = new QTcpSocket(this);
        tcpsocket->abort();
        tcpsocket->connectToHost(ui->remoteipcombox->currentText(),port);
        if(tcpsocket)
        {
            connect(tcpsocket,SIGNAL(readyRead()),
                    this,SLOT(readmessage()));
            connect(tcpsocket,SIGNAL(disconnected()),
                    this,SLOT(deletesocket()));
            ui->portcombox->setEnabled(false);
            ui->remoteipcombox->setEnabled(false);
            ui->displaytext->append(tr("连接服务器%1，端口%2").arg(ui->remoteipcombox->currentText()).arg(port));
            ui->openbutton->setText(tr("断开服务器"));
            ui->sendbutton->setEnabled(true);
        }
    }
    else
    {
        //tcpsocket->close();
        //tcpsocket->deleteLater();
        //ui->displaytext->append(tr("断开服务器%1，端口%2").arg(ui->remoteipcombox->currentText()).arg(ui->portcombox->currentText()));
        if(tcpsocket->isOpen())
        {
            deletesocket();
        }
    }
}

void TcpClientDialog::on_clearbutton_clicked()
{
    ui->displaytext->clear();
}

void TcpClientDialog::on_sendbutton_clicked()
{
    sendmessage();
}

void TcpClientDialog::readmessage()
{
    QByteArray qba = tcpsocket->readAll();
    QString str = QVariant(qba).toString();
    ui->displaytext->append(str);
}

void TcpClientDialog::sendmessage()
{
    QString str = ui->sendline->text();
    if(!str.isEmpty())
    {
    tcpsocket->write(str.toStdString().c_str(),strlen(str.toStdString().c_str()));
    }
}

void TcpClientDialog::deletesocket()
{
    //tcpsocket = NULL;
    ui->portcombox->setEnabled(true);
    ui->remoteipcombox->setEnabled(true);
    tcpsocket->abort();
    tcpsocket->deleteLater();
    ui->displaytext->append(tr("断开服务器%1，端口%2").arg(ui->remoteipcombox->currentText()).arg(ui->portcombox->currentText()));
    ui->openbutton->setText(tr("连接服务器"));
    ui->sendbutton->setEnabled(false);
}


void TcpClientDialog::create_im_connect(QString objname)
{
    /*
    disconnect(im->keyboard,SIGNAL(set_value(QString)),ui->portcombox,SLOT(setEditText(QString)));
    disconnect(im->keyboard,SIGNAL(set_value(QString)),ui->remoteipcombox,SLOT(setEditText(QString)));
    disconnect(im->keyboard,SIGNAL(set_value(QString)),ui->sendline,SLOT(setText(QString)));
    if(objname == "port")
    {
        connect(im->keyboard,SIGNAL(set_value(QString)),ui->portcombox,SLOT(setEditText(QString)));
    }
    if(objname == "remoteip")
    {
        connect(im->keyboard,SIGNAL(set_value(QString)),ui->remoteipcombox,SLOT(setEditText(QString)));
    }

    if(objname == "sendline")
    {
        connect(im->keyboard,SIGNAL(set_value(QString)),ui->sendline,SLOT(setText(QString)));
    }
    */
}
