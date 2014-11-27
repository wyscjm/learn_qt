#ifndef TCPSERVERDIALOG_H
#define TCPSERVERDIALOG_H

#include <QDialog>

class QTcpServer;
class QTcpSocket;

namespace Ui {
class TcpServerDialog;
}

class TcpServerDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TcpServerDialog(QWidget *parent = 0);
    ~TcpServerDialog();
    
private:
    Ui::TcpServerDialog *ui;

    QTcpServer *tcpserver;
    QTcpSocket *clientConnection;
private:
    void send_msg();
private slots:
    void on_openbutton_clicked();
    void processconnect();
    void on_clearbutton_clicked();
    void on_sendbutton_clicked();
    void deleteclient();
    void readmessage();

};

#endif // TCPSERVERDIALOG_H
