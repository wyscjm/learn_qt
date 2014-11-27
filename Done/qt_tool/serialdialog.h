#ifndef SERIALDIALOG_H
#define SERIALDIALOG_H

#include <QDialog>

#include "posix_qextserialport.h"

//class QTimer;

enum DATAFROM{
    R2000 = 1,
    PN512
};


namespace Ui {
class SerialDialog;
}

class SerialDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SerialDialog(QWidget *parent = 0);
    ~SerialDialog();
    
private:
    Ui::SerialDialog *ui;


public:
    int quantity;
private:
    Posix_QextSerialPort *my_com;
    QTimer *my_timer;
    QTimer *my_read_timer;//读取串口数据
//    QTimer *my_send_timer;//发送串口数据
//    QTimer *my_save_timer;//保存串口数据

    int send_count;//发送数据计数
    int receive_count;//接收数据计数
    bool is_show;//是否显示数据

    bool is_auto_send;//是否自动发送
    bool is_auto_save;//是否自动保存
    bool is_auto_clear;//是否自动清空

    bool is_init;//是否完成了初始化

    int targetnum;

    unsigned int epcnum;
    QString epc[1000];
    QString recvbuf;
    quint16 start;

private:
    void InitSerialDialog();//初始化界面以及其他处理
    void ChangeEnable(bool b);//改变状态

    QByteArray GetHexValue(QString str);//格式化16进制数据,参数为字符串
    char ConvertChar2Hex(char ch);

    void dealepc();//处理EPC是否相同

private slots:
    void on_cleardatabutton_clicked();
    void on_senddatabutton_clicked();
    void on_openbutton_clicked();
    void on_processdatabutton_clicked();

    void ReadMyCom();//读取串口数据
    void WriteMyCom();//写串口数据
    //void SaveMyCom();//保存串口数据
//    void SendData();//发送数据

//    void SaveData();//保存数据
    void ReadTxtData();//读取配置文件数据
};

#endif // SERIALDIALOG_H
