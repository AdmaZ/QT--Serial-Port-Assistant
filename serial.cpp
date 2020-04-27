/*------------------------------------
 *          user
 *------------------------------------*/
#include <include.h>

Serial::Serial(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serial)
{
    ui->setupUi(this);
    //用户添加
    Serial::setWindowTitle(tr("  串口调试助手V1.0 —— Adma 庄           个人网站：www.gudaobian.top"));
    Timer0_Init();
    Timer1_Init();
    systemInit();
}
Serial::~Serial()
{
    delete ui;
}
/*----------------------------------------------------------
 *          function
 *----------------------------------------------------------*/
void Serial::systemInit()
{
    globlePort.setParity(QSerialPort::NoParity);
    globlePort.setDataBits(QSerialPort::Data8);
    globlePort.setStopBits(QSerialPort::OneStop);

    //端口设定
    ui->baudRateBox->setCurrentIndex(4);
    //信号绑定到槽
    connect(ui->openButton,&QPushButton::clicked,this,&Serial::ButtonOpenPort);//打开串口信号
    connect(ui->sendTxtButton,&QPushButton::clicked,this,&Serial::ButtonSendPort);//发送文本信号
    connect(ui->pushButton_3,&QPushButton::clicked,this,&Serial::ButtonStopShow);//暂停显示文本信号
    connect(&globlePort,&QSerialPort::readyRead,this,&Serial::ReciveDate);//串口打开就接收消息
    connect(ui->pushButton_2,&QPushButton::clicked,this,&Serial::ButtonClear);//清空文本框信号
    connect(ui->checkBox,&QCheckBox::stateChanged,this,&Serial::AutoClear);//自动清除
    //connect(ui->checkBox_2,&QCheckBox::stateChanged,this,&Serial::ButtonClear);//16进制信号
}
void Serial::on_textBrowser_textChanged()
{
   //当文本框到底的时候自动下滑
   ui->textBrowser->moveCursor(QTextCursor::End);
}
/*--------------------------
 *      手动清除文本
 * ------------------------*/
void Serial::ButtonClear()

{
   ui->textBrowser->clear();
}
/*--------------------------
 *      暂停、开始显示文本
 * ------------------------*/
void Serial::ButtonStopShow()
{
    if(ui->pushButton_3->text() == QString("暂停显示"))
    {
        ui->pushButton_3->setText(QString("开始显示"));
    }
    else if(ui->pushButton_3->text() == QString("开始显示"))
    {
        ui->pushButton_3->setText(QString("暂停显示"));
    }
}
/*--------------------------
 *      自动清理
 * ------------------------*/
void Serial::AutoClear()
{
    if(ui->checkBox->isChecked()==true)
    {
        newTime->start();

    }
    else if(ui->checkBox->isChecked()==false)
    {
        newTime->stop();
    }
}
/*----------------------------------------------------------
 *          定时器
 *----------------------------------------------------------*/
void Serial::Timer0_Init()
{
    myTime = new QTimer();
    myTime->stop();
    myTime->setInterval(1000);//1ms定时器，类似于单片机的中断函数
    connect(myTime,SIGNAL(timeout()),this,SLOT(Timer0_Task()));
    //当记满1000时执行Timer0_Task
    myTime->start();
}
void Serial::Timer0_Task()
{
    Read_Serial_Connect_Success();
}
void Serial::Timer1_Init()
{
    newTime = new QTimer();
    newTime->stop();
    newTime->setInterval(4000);//1ms定时器，类似于单片机的中断函数
    connect(newTime,SIGNAL(timeout()),this,SLOT(Timer1_Task()));
    //当记满100时执行Timer1_Task
}
void Serial::Timer1_Task()
{    
    qDebug()<<"定时器1";
    ui->textBrowser->clear();
    /*
    qDebug()<<"定时器1";
    int z = 0;
    for(int i = 0; i < ui->portBox->count();i++)
    {
        for(int j = 0; j < ui->portBox->count();j++)
        {
            if(ui->portBox->itemText(i) == globlePort.portName())
            {
                z++;
            }

            //盒子里的和链表的不一样
        }
        if(z == 0)
        {
            ui->portBox->removeItem(i);
            //staticList[i].
        }
        z = 0;
    }
    */
}
/*----------------------------------------------------------
 *          COM自动识别
 *----------------------------------------------------------*/
static int Read_Serial_Port_Info()
{

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description :"<<info.description();
        if(info.portName() != NULL)
            return true;
        else
            return false;
    }
    return false;
}
void Serial::Read_Serial_Connect_Success()
{
    qDebug() << "寻找串口 返回值："<<Read_Serial_Port_Info() << endl;
    if(Read_Serial_Port_Info())
    {
        Serial_Connect_Success_Label_Text();
    }
    else
        Serial_Connect_NG_Label_Text();
}
void Serial::Serial_Connect_Success_Label_Text()
{
    serialStrList.clear();
    serialStrList = scanSerial();
    for(int z = 0;z < serialStrList.size();z++)
        qDebug()<<"链表1为"<<serialStrList[z];
    qDebug()<<"size:"<<serialStrList.size();
    //如果有新增的COM口，则scanSerial()会变为："COMx",这样将以前的字符串添加在新增的前面即可
    for(int i = 0;i<staticList.size();i++)
    {
        serialStrList.insert(i, staticList[i]);
    }
    if(staticList != serialStrList)
    {
        for(int q = 0;q < serialStrList.size();q++)
            qDebug()<<"链表2为"<<serialStrList[q];
        staticList = serialStrList;
        staticList = compilerport.compiler_port(staticList,staticList.size());
        for(int c = 0;c < staticList.size();c++)
            qDebug()<<"链表3为"<<staticList[c];
        ui->portBox->clear();
        for (int i=0; i<staticList.size(); i++)
        {
            ui->portBox->addItem(staticList[i]); // 在comboBox那添加串口号
        }
    }
    QFont font ( "已连接", 12, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    ui->label_1->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::darkGreen);
    ui->label_1->setPalette(pe);
    ui->label_1->setText("已连接");
}
void Serial::Serial_Connect_NG_Label_Text()
{
    ui->portBox->clear();//当所有串口都拔出是清空
    staticList.clear();
    QFont font ( "等待中", 12, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    ui->label_1->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_1->setPalette(pe);
    ui->label_1->setText("等待中");
}
/*----------------------------------------------------------
 *          判定串口是否重复
 *----------------------------------------------------------*/
QStringList Serial::scanSerial()
{
    //读取串口信息
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        int judge = 0;//标志位；
        //1.利用portBox->count()统计portBox里标签的数量
        //2.利用for(i=0;count;)循环，以此判定标签里有没有重复
        //3.若没有重复则 标志位为0，否则为1.只有为0才会append()
        globlePort.setPort(info);
        for(int i=0;i<ui->portBox->count();i++)
        {
            if(globlePort.portName() == ui->portBox->itemText(i))
            {
                judge++;
            }
            qDebug()<<judge<<"key测试";
        }
        if(judge == 0)
        {
            serialStrList.append(globlePort.portName());
        }     
    }
    return serialStrList;
}
/*----------------------------------------------------------
 *          slots
 *----------------------------------------------------------*/
void Serial::ButtonOpenPort(bool)
{
    if(ui->openButton->text() == QString("打开串口"))  //串口未打开
        {
            //设置端口号
            globlePort.setPortName(ui->portBox->currentText());
            //设置波特率
            globlePort.setBaudRate(ui->baudRateBox->currentText().toInt());
            //设置数据位
            qDebug()<<"数据位"<<ui->DateBitsBox->currentText().toInt();
            switch (ui->DateBitsBox->currentText().toInt())
            {             
                case 8: globlePort.setDataBits(QSerialPort::Data8); break;
                case 7: globlePort.setDataBits(QSerialPort::Data7); break;
                case 6: globlePort.setDataBits(QSerialPort::Data6); break;
                case 5: globlePort.setDataBits(QSerialPort::Data5); break;
                default: break;
            }
            //设置停止位
            qDebug()<<"停止位"<<ui->stopBitsBox->currentText().toInt();
            switch (ui->stopBitsBox->currentText().toInt())
            {

                case 1: globlePort.setStopBits(QSerialPort::OneStop);break;
                case 2: globlePort.setStopBits(QSerialPort::TwoStop);break;
                default:break;
            }
            //设置校验方式
            switch (ui->ParityBox->currentIndex())
            {
                case 0: globlePort.setParity(QSerialPort::NoParity);break;
                default:break;
            }
            //设置流控制模式
            globlePort.setFlowControl(QSerialPort::NoFlowControl);
            //打开串口
            if(globlePort.open(QIODevice::ReadWrite)==false)
            {
                //QMessageBox::warning(NULL , "提示", "串口打开失败！");
                return;
            }
            // 失能串口设置控件
            ui->portBox->setEnabled(false);
            ui->ParityBox->setEnabled(false);
            ui->baudRateBox->setEnabled(false);
            ui->DateBitsBox->setEnabled(false);
            ui->stopBitsBox->setEnabled(false);
            //ui->search_Button->setEnabled(false);
            //调整串口控制按钮的文字提示
            ui->openButton->setText(QString("关闭串口"));
        }
        else       //串口已经打开
        {
            globlePort.close();
            // 使能串口设置控件
            ui->portBox->setEnabled(true);
            ui->ParityBox->setEnabled(true);
            ui->baudRateBox->setEnabled(true);
            ui->DateBitsBox->setEnabled(true);
            ui->stopBitsBox->setEnabled(true);
            //ui->search_Button->setEnabled(true);
            //调整串口控制按钮的文字提示
            ui->openButton->setText(QString("打开串口"));
         }
}
void Serial::ButtonSendPort(bool)
{
    QString date = ui->textEdit->toPlainText();
    QByteArray array = date.toLatin1();
    globlePort.write(array);
}
/*----------------------------------------------------------
 *          recive Date
 *----------------------------------------------------------*/
//读取接收到的数据
void Serial::ReciveDate()
{   
    static QByteArray Serial_buff;//定义static，否则会被清理
    Serial_buff += globlePort.readAll();
    if(ui->pushButton_3->text() == QString("暂停显示"))
    {
        //判断标志结束
        if(ui->checkBoxHex->isChecked() == false)
        {
            if(Serial_buff.endsWith("\n"))
            {
                qDebug()<<Serial_buff;
                QString string = QString::fromLocal8Bit(Serial_buff);//中文字符输出
                ui->textBrowser->insertPlainText(string);
                Serial_buff.clear();//数据清理
            }
        }
        else if(ui->checkBoxHex->isChecked() == true)
        {
            if(Serial_buff.endsWith("\n"))
            {
                QString strDis;
                QByteArray hexData = Serial_buff.toHex();
                hexData = hexData.toUpper ();//转换为大写
                for(int i = 0;i<hexData.length ();i+=2)//填加空格
                {
                    QString st = hexData.mid (i,2);
                    strDis += st;
                    strDis += "  ";
                }
                Serial_buff.clear();//先清空，以防已接收的部分变成乱码
                Serial_buff.append(strDis);
                ui->textBrowser->insertPlainText(Serial_buff);
                Serial_buff.clear();//数据清理
            }
        }
    }
    else if(ui->pushButton_3->text() == QString("开始显示"))
    {
        Serial_buff.clear();
    }
}
