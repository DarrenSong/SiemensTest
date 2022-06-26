#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->cob->clear();
    foreach(const QSerialPortInfo&info,QSerialPortInfo::availablePorts())
    {
        ui->cob->addItem(info.portName());
    }
    ui->cob_baudrate->addItem("9600");
    ui->cob_baudrate->addItem("19200");
    ui->cob_baudrate->addItem("115200");
    ui->cob_baudrate->setCurrentIndex(2);

    ui->btnClose->hide();

    connect(&serial,&QSerialPort::readyRead,this,&MainWindow::On_SerialPort_Received);
}

MainWindow::~MainWindow()
{
    serial.close();
    delete ui;
}

void MainWindow::InitSerialPort()
{
    serial.setPortName(ui->cob->currentText());//设置串口号
    serial.setBaudRate(ui->cob_baudrate->currentText().toInt());//设置波特率
    serial.setDataBits(QSerialPort::Data8);//设置串口数据位8
    serial.setParity(QSerialPort::NoParity);//无校验位
    serial.setStopBits(QSerialPort::OneStop);//1位停止位
    serial.setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::on_btnOpen_clicked()
{
    InitSerialPort();
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(ui->btnOpen->text()=="Open")
    {

        if(serial.open(QIODevice::ReadWrite))
        {
            ui->btnOpen->setText("Close");
            ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"Open serial port:"+ui->cob->currentText());
        }
        else
        {
            ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to open the serial port:"+ui->cob->currentText());
        }
    }
    else
    {
        serial.close();
        ui->btnOpen->setText("Open");
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"Close serial port:"+ui->cob->currentText());
    }


}


void MainWindow::on_btnClose_clicked()
{
    //serial.close();
}


void MainWindow::on_BtnConnect_clicked()
{
   char connect_buf[6]={ 0x5A,0x1C,0x0,0x0,0x0,0x5F};
   int len = serial.write(connect_buf,6);
   QDateTime curDateTime=QDateTime::currentDateTime();
   if(len>0)
   {
       ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the connect cmd to target.");
   }
   else
   {
       ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the connect cmd to target.");
   }
}


void MainWindow::on_DisConnect_clicked()
{
    char disconnect_buf[6]={0x5A,0x1D,0x0,0x0,0x0,0x5F};
    int len =serial.write(disconnect_buf,6);
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(len>0)
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the disconnect cmd to target.");
    }
    else
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the disconnect cmd to target.");
    }
}

void MainWindow::On_SerialPort_Received()
{
   QByteArray rx_buf = serial.readAll();

    if(rx_buf.at(0)==0x5B && rx_buf.at(5)==0x5F)
    {
        //buttun pressed
        if(rx_buf.at(1)==0x1c && rx_buf.at(2)==0x1b)
        {
            if(rx_buf.at(3)==0x1c)//Variant 2
            {//turn off led1
                char t_buf[6]={ 0x5A,0x1C,0x1e,0x0,0x0,0x5F};
                serial.write(t_buf,6);
            }
            else//Variant 1
            {//blink led1 3times/s,then goes off
                char t_buf[6]={ 0x5A,0x1C,0x1b,0x0,0x0,0x5F};
                serial.write(t_buf,6);
            }
            QDateTime curDateTime=QDateTime::currentDateTime();
            ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"Buttun was pressed,send ack to the target.");
            ui->txtlog->moveCursor(QTextCursor::End);
        }
        //led1 state
        if(rx_buf.at(1)==0x1c && rx_buf.at(2)==0x1e)
        {
            if(rx_buf.at(3)==0)
            {
                ui->txtLED1State->setText("LED1 Off");
            }
            else
            {
                 ui->txtLED1State->setText("LED1 On");
            }
        }

        //
    }

}

//0x5A	0x1C	0x1c	1/2/3/0	 0x0	0x5F
// 0: turn off led1
// 1: led1 blink 250ms
// 2: led1 blink 500ms
// 3: led1 blink 1000ms
void MainWindow::on_btnOff_clicked()
{
//0x5A	0x1C	0x1c	1/2/3/0		0x5F
    char offled_buf[6]={ 0x5A,0x1C,0x1c,0x0,0x0,0x5F};
    int len = serial.write(offled_buf,6);
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(len>0)
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the led1 off cmd to target.");
    }
    else
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the led1 off cmd to target.");
    }
}


void MainWindow::on_btn250ms_clicked()
{
    char offled_buf[6]={0x5A,0x1C,0x1c,0x1,0x0,0x5F};
    int len= serial.write(offled_buf,6);
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(len>0)
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the led1 blink250ms cmd to target.");
    }
    else
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the led1 blink250ms cmd to target.");
    }
}


void MainWindow::on_btn500ms_clicked()
{
    char offled_buf[6]={0x5A,0x1C,0x1c,0x2,0x0,0x5F};
    int len = serial.write(offled_buf,6);
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(len>0)
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the led1 blink500ms cmd to target.");
    }
    else
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the led1 blink500ms cmd to target.");
    }
}


void MainWindow::on_btn1s_clicked()
{
    char offled_buf[6]={0x5A,0x1C,0x1c,0x3,0x0,0x5F};
    int len = serial.write(offled_buf,6);
    QDateTime curDateTime=QDateTime::currentDateTime();
    if(len>0)
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"send the led1 blink1000ms cmd to target.");
    }
    else
    {
        ui->txtlog->append(curDateTime.toString("[yyyy-MM-dd hh:mm:ss:zzz]")+"failed to send the led1 blink1000ms cmd to target.");
    }
}

