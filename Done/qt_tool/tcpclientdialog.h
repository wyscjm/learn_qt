#ifndef TCPCLIENTDIALOG_H
#define TCPCLIENTDIALOG_H

#include <QDialog>

class QTcpSocket;
class InputMethod;

namespace Ui {
class TcpClientDialog;
}

class TcpClientDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TcpClientDialog(QWidget *parent = 0);
    ~TcpClientDialog();
    
private:
    Ui::TcpClientDialog *ui;

private:
    QTcpSocket *tcpsocket;
    InputMethod *im;

private slots:
    void on_openbutton_clicked();
    void on_sendbutton_clicked();
    void on_clearbutton_clicked();
    void sendmessage();
    void readmessage();
    void deletesocket();
    void create_im_connect(QString objname);
};

#endif // TCPCLIENTDIALOG_H
