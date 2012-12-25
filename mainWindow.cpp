#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QString>
#include <QDebug>

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    QString portName = "/dev/ttyUSB0";
    port = new QextSerialPort(portName,QextSerialPort::EventDriven,this);

    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(port->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::onReadyRead()
{
    QByteArray byteData;
    int s = port->bytesAvailable();
    byteData.resize(s);
    port->read(byteData.data(),byteData.size());
    incomingData.append(byteData);

    if(byteData.contains('\n'))
    {
        ui->inputLineEdit->setText(incomingData);
        qDebug()<<incomingData;
        incomingData.clear();
    }
}

void mainWindow::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}

void mainWindow::on_sendButton_clicked()
{
    QByteArray outData;
    outData.append(ui->sendLineEdit->text());
    outData.append('\n');
    port->write(outData);
}
