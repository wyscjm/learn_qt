#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>
#include "tcpserverdialog.h"
#include "ui_tcpserverdialog.h"

TcpServerDialog::TcpServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpServerDialog)
{
    ui->setupUi(this);
    tcpserver = new QTcpServer(this);
    clientConnection = new QTcpSocket(this);

    QStringList port_list;
    port_list.clear();
    port_list<<"8000"<<"8001"<<"8002";
    ui->portcombox->clear();
    ui->portcombox->addItems(port_list);
    ui->portcombox->setCurrentIndex(1);

    ui->openbutton->setText(tr("打开服务器"));
    ui->sendbutton->setEnabled(false);
}

TcpServerDialog::~TcpServerDialog()
{
    delete ui;
    /*
    if(clientConnection != NULL)
    {
        delete clientConnection;
    }
    if(tcpserver != NULL)
    {
        delete tcpserver;
    }
    */
}

void TcpServerDialog::on_openbutton_clicked()
{
    quint16 port = ui->portcombox->currentText().toUInt();
    if(ui->openbutton->text() == tr("打开服务器"))
    {

        if(!tcpserver->listen(QHostAddress::Any,port))
        {
            return;
        }
        connect(tcpserver,SIGNAL(newConnection()),
                    this,SLOT(processconnect()));
        ui->portcombox->setEnabled(false);
        ui->displaytext->append(tr("打开服务器，监听端口%1").arg(port));
        ui->openbutton->setText(tr("关闭服务器"));
    }
    else
    {
        tcpserver->close();
        clientConnection->abort();
        clientConnection->close();
        ui->portcombox->setEnabled(true);
        ui->displaytext->append(tr("关闭服务器，监听端口%1").arg(port));
        ui->openbutton->setText(tr("打开服务器"));
    }
}

void TcpServerDialog::processconnect()
{
    if(!clientConnection->isOpen())
    {
    ui->displaytext->append(tr("have new connect"));
    ui->sendbutton->setEnabled(true);
    clientConnection = tcpserver->nextPendingConnection();
    connect(clientConnection,SIGNAL(readyRead()),
            this,SLOT(readmessage()));
    connect(clientConnection,SIGNAL(disconnected()),
            this,SLOT(deleteclient()));
    }
}

void TcpServerDialog::on_clearbutton_clicked()
{
    ui->displaytext->clear();
}

void TcpServerDialog::on_sendbutton_clicked()
{
    send_msg();
}

void TcpServerDialog::send_msg()
{
    QString str = ui->sendline->text();
    if(!str.isEmpty())
    {
    clientConnection->write(str.toStdString().c_str(),strlen(str.toStdString().c_str()));
    }
}

void TcpServerDialog::readmessage()
{
    QByteArray qba = clientConnection->readAll(); //读取
    QString str = QVariant(qba).toString();
    ui->displaytext->append(str);
}

void TcpServerDialog::deleteclient()
{
//    clientConnection->disconnect();
    clientConnection->abort();
    clientConnection->close();
    ui->displaytext->append(tr("关闭新链接"));
    ui->sendbutton->setEnabled(false);
}
