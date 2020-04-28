#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>
#include <include.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Serial; }
QT_END_NAMESPACE

class Serial : public QMainWindow
{
    Q_OBJECT

public:
    Serial(QWidget *parent = nullptr);
    ~Serial();


    //定时器相关定义
    QTimer *myTime;
    void Timer0_Init();//定时器初始化
    QTimer *newTime;
    void Timer1_Init();//定时器初始化

    CompilerPort compilerport;
    QStringList staticList ;

    void handleSerialError(QSerialPort::SerialPortError error);

private slots:   

    void Timer0_Task();//定时器任务
    void Timer1_Task();//定时器任务
    void ButtonOpenPort(bool);//打开端口功能
    void ButtonSendPort(bool);//发送文本功能
    void ReciveDate();//接收信息
    void ButtonClear();//清除文本框
    void ButtonStopShow();//暂停显示
    void on_textBrowser_textChanged();//文本框自动下拉
    void AutoClear();//自动清除

private:
    /*---------------function---------------*/

    void systemInit();//初始化
    void Read_Serial_Connect_Success(); //成功检测到串口
    void Serial_Connect_Success_Label_Text();//串口自动检测连接成功以后的信息
    void Serial_Connect_NG_Label_Text();//串口自动检测连接失败以后的信息
    QStringList scanSerial();//向box里写COMx

    /*---------------variable---------------*/

    Ui::Serial *ui;//ui对象
    QSerialPort globlePort;//串口对象
    QStringList serialStrList;//为实现scanSerial()而定义的，串口字符串链

};
#endif // SERIAL_H
