#include <QtGui>

#include "serialdialog.h"
#include "ui_serialdialog.h"
//#include "myStaticClass.h"


SerialDialog::SerialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialDialog),
    quantity(-1)
{
    ui->setupUi(this);


    this->setWindowTitle(tr("串口调试工具 V1.0"));
    this->InitSerialDialog();
}

SerialDialog::~SerialDialog()
{
    delete ui;
}

void SerialDialog::InitSerialDialog()
{
    receive_count = 0;
    send_count = 0;

//    this->is_show = true;
//    this->is_auto_clear=false;
//    this->is_auto_save=false;
//    this->is_auto_send=false;
//    this->is_init=false;

    QStringList com_list;//串口号
    QStringList baud_list;//波特率
//    QStringList parity_list;//校验位
//    QStringList data_bits_list;//数据位
//    QStringList stop_bits_list;//停止位

    com_list.clear();
    com_list<<"ttymxc1"<<"ttymxc2"<<"ttymxc3"<<"ttymxc4";
    ui->portcombox->clear();
    ui->portcombox->addItems(com_list);
    ui->portcombox->setCurrentIndex(0);

    baud_list.clear();
    baud_list<<"4800"<<"9600"<<"19200"<<"57600"<<"115200";
    ui->baudcombox->clear();
    ui->baudcombox->addItems(baud_list);
    ui->baudcombox->setCurrentIndex(4);

    ui->sendcombox->clear();
    ui->displaydatatext->clear();

    //读取数据
    my_read_timer = new QTimer(this);
    my_read_timer->setInterval(200);//void	setInterval ( int msec )
    connect(my_read_timer,SIGNAL(timeout()),this,SLOT(ReadMyCom()));
    connect(ui->exitbutton,SIGNAL(clicked()),
            this,SLOT(reject()));

    this->ChangeEnable(false);
    this->ReadTxtData();
//    this->is_init = true;

    start = 0;
    epcnum = 0;
}


void SerialDialog::ChangeEnable(bool b)
{
    ui->portcombox->setEnabled(!b);
    ui->baudcombox->setEnabled(!b);

    ui->cleardatabutton->setEnabled(b);
    ui->senddatabutton->setEnabled(b);

    if (b)
    {
        this->my_read_timer->start();
    }
    else
    {
        this->my_read_timer->stop();
    }
}

QByteArray SerialDialog::GetHexValue(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toAscii();
        if((hstr == ' ') || (hstr == '\r'))
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toAscii();
        hexdata = ConvertChar2Hex(hstr);
        lowhexdata = ConvertChar2Hex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}

char SerialDialog::ConvertChar2Hex(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void SerialDialog::ReadMyCom()
{
//    MyHelper::DelayTime(100);//延时100毫秒

    QString strHex;//16进制数据
    QString strNomal;//正常数据
    QByteArray temp = my_com->readAll();

    if(!temp.isEmpty())
    {
        QDataStream out(&temp,QIODevice::ReadWrite);
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;
            QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

            if(str.length()>1)
            {
                strHex+=str+" ";
            }
            else
            {
                strHex+="0"+str+" ";
            }
        }

        strNomal = temp;

        if(ui->hexcheck->isChecked())
        {
            ui->displaydatatext->append(strHex.toUpper());
            recvbuf += strHex.toUpper();
        }
        else
        {
         ui->displaydatatext->append(strNomal);
         recvbuf += strNomal.toUpper();
        }

        receive_count=receive_count+temp.size();

        if(ui->autohexcheck->isChecked())
        {
            if(!(ui->hexcheck->isChecked()))
            {
                return;
            }
            on_processdatabutton_clicked();
        }
    }
}



void SerialDialog::WriteMyCom()
{
    QString str=ui->sendcombox->currentText();
    QByteArray outData=str.toAscii();

    int size=outData.size();

    //如果串口没有打开
    if (!my_com->isOpen())
    {
        QMessageBox::warning(this,tr("Error"),tr("This Com is used!"),QMessageBox::Ok);
        return;

    }
    //如果发送数据为空
    if (outData.isEmpty())
    {
        ui->sendcombox->setFocus();
        return;
    }

    //linux下的回车符为一个字符\r
    if (outData[size-1]!='\r')
    {
        outData.resize(size+1);
        outData[size]='\r';
    }

    //默认16进制发送不加入回车符
    if (ui->hexcheck->isChecked())//转化为16进制发送
    {
        outData=this->GetHexValue(str);
        size=outData.size();
        my_com->write(outData);
    }
    else
    {
        size=outData.size();
        my_com->write(outData);
    }

    send_count=send_count+size;
}

void SerialDialog::ReadTxtData()
{
    ui->sendcombox->clear();
    QString fileName=QCoreApplication::applicationDirPath()+"/txdata.txt";

    QFile file(fileName);
    file.open(QFile::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line;
    do
    {
        line=in.readLine();

        if (line!="")
        {
            ui->sendcombox->addItem(line);
        }
    }
    while(!line.isNull());

    file.close();
}

void SerialDialog::on_openbutton_clicked()
{

    if (ui->openbutton->text() == tr("打开串口"))
    {
        QString portName="/dev/"+ui->portcombox->currentText();
        my_com = new Posix_QextSerialPort(portName, QextSerialBase::Polling);

        if (my_com->open(QIODevice::ReadWrite))
        {
            my_com->flush();
            BaudRateType my_baud;//"4800"<<"9600"<<"19200"<<"57600"<<"115200";
            switch(ui->baudcombox->currentIndex())
            {
            case 0:
                my_baud = BAUD4800;
                break;
            case 1:
                my_baud = BAUD9600;
                break;
            case 2:
                my_baud = BAUD19200;
                break;
            case 3:
                my_baud = BAUD57600;
                break;
            case 4:
                my_baud = BAUD115200;
                break;
            default:
                my_baud = BAUD115200;
                break;
            }
            my_com->setBaudRate(my_baud);
            my_com->setDataBits((DataBitsType)DATA_8);
            my_com->setParity((ParityType) PAR_NONE);
            my_com->setStopBits((StopBitsType) STOP_1);
            my_com->setFlowControl(FLOW_OFF);
            my_com->setTimeout(10);
            this->ChangeEnable(true);
//            this->();
            ui->openbutton->setText(tr("关闭串口"));

            //ui->labIsOpen->setText(tr("串口状态：打开"));
            /*
            QMessageBox msg;
            msg.setWindowTitle(tr("提示"));
            msg.setText(tr("串口打开"));
            msg.setIcon(QMessageBox::Information);
            msg.addButton(tr("确定"),QMessageBox::ActionRole);
            msg.exec();
            */
            return;
        }
        else
        {
            QMessageBox msg;
            msg.setWindowTitle(tr("提示"));
            msg.setText(tr("串口打开失败"));
            msg.setIcon(QMessageBox::Information);
            msg.addButton(tr("确定"),QMessageBox::ActionRole);
            msg.exec();
            return;
        }
    }
    else
    {
        this->ChangeEnable(false);
        ui->displaydatatext->document()->clear();
        ui->openbutton->setText("打开串口");
        my_com->close();
        /*
        QMessageBox msg;
        msg.setWindowTitle(tr("提示"));
        msg.setText(tr("关闭串口"));
        msg.setIcon(QMessageBox::Information);
        msg.addButton(tr("确定"),QMessageBox::ActionRole);
        msg.exec();
        */
        return;
    }
}


void SerialDialog::on_cleardatabutton_clicked()
{
   ui->displaydatatext->document()->clear();
}

void SerialDialog::on_senddatabutton_clicked()
{
    this->WriteMyCom();
//    recvbuf.clear();
}


void SerialDialog::on_processdatabutton_clicked()
{
//    ui->displaydatatext->append("01 00 00 00 02 00 00 00 0f 00 00 c9 ee 0a 00");
//    ui->displaydatatext->append("01 00 01 00 02 00 00 00 98 f6 0a 00 00 00 00");
//    QString str = ui->displaydatatext->toPlainText();
//    ui->displaydatatext->document()->clear();
//    ui->displaydatatext->append(str);
    QByteArray qba = GetHexValue(recvbuf);
    quint8 pktversion,pktflag;
    quint16  pkttype,pktlength,datalength;
    int index;
//    start = 0;
    DATAFROM df;
    df = R2000;
    if(!qba.length())
    {
        return;
    }
    switch(df)
    {
    case R2000:
      //  if(qba.length() < 6){return;}
        while(start < (qba.length()-8))
        {
            pktversion = qba.at(start);
            pktflag = qba.at(start+1);
            pkttype = qba.at(start+2) | (qba.at(start+3)<<8);
            pktlength = qba.at((start+4)) | (qba.at(start+5)<<8);
            ui->displaydatatext->append(tr("version is 0x%1,flag is 0x%2,type is 0x%3,length is 0x%4")
                                .arg(pktversion,2,16,QLatin1Char('0')).arg(pktflag,2,16,QLatin1Char('0')).arg(pkttype,2,16,QLatin1Char('0')).arg(pktlength,2,16,QLatin1Char('0')));

            if(pktlength > 8)//如果数据包长度太大，不符合数据，丢弃，96位/12Byte EPC的数据包时，数据包为7
            {
                start++;
                break;
            }
            if((qba.length()-start) >= (pktlength*4+8))
            {
                if((pkttype == 5)&&(pktversion == 1))//R2000 polling 数据包
                {
                    datalength = (pktlength-3)*4;
                    ui->displaydatatext->append(tr("EPC数据段："));
                    QString str;
                    for(index = 0;index < datalength; index++)
                    {
                       // ui->displaydatatext->moveCursor(QTextCursor::End);
                        qint8 q8= qba.at(start+20+index);
                        str = str +tr("%1").arg(q8&0xFF,2,16,QLatin1Char('0'))+" ";
                    }
                    epc[epcnum++] = str;
                    ui->displaydatatext->append(str);
                    start = (start + pktlength*4 + 8);
                }
                else if((pkttype == 0)&&(pktversion == 1)&&(pktlength) ==2)
                {
                    start = start + pktlength*4 + 8;
                    epcnum = 0;
                    ui->displaydatatext->append(tr("EPC起始包"));
                }
                else if((pkttype == 1)&&(pktversion == 1)&&(pktlength == 2))
                {
                    start = 0;
                    //end = 0;
                    //start = start + pktlength*4 + 8;
                    unsigned int uni;
                    recvbuf.clear();
                    uni = epcnum;
                    ui->displaydatatext->append(tr("EPC结束包"));
                    if(epcnum)
                    {
                        dealepc();
                    }
                    ui->displaydatatext->append(tr("数据包个数%1").arg(uni));
                    return;
                }
                else
                {
                    start++;
                    //ui->displaydatatext->append(tr("error1"));

                }
            }
            else
            {
                 //start++;
                // ui->displaydatatext->append(tr("error2"));
                //return;
            }
        }
        break;
    default:
        break;
    }
}


void SerialDialog::dealepc()
{
    unsigned int i,j;
    unsigned int oldnum;
    unsigned int newnum;
    bool flag;
    flag = 0;
    oldnum = epcnum;
    newnum = 1;
    for(i = 1; i < oldnum; i++)
    {
        for(j = 0;j < newnum; j++)
        {
            if(epc[i] == epc[j])
            {
                flag = 1;
            }
        }
        if(!flag)
        {
            newnum++;
        }
    }
    if(oldnum == 0)
    {
        epcnum = 0;
    }
    else
    {
            epcnum = newnum;
    }
    for(i = 0; i < newnum; i++)
    {
        ui->displaydatatext->append(tr("标签%1: ").arg(i+1)+epc[i]);
    }
    ui->displaydatatext->append(tr("标签数量%1").arg(epcnum));
}
