#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>

#include "qextserialport.h"

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();
    
private:
    Ui::mainWindow *ui;
    QextSerialPort *port;

    QByteArray incomingData;

public slots:
    void onReadyRead();
    void onDsrChanged(bool status);
private slots:
    void on_sendButton_clicked();
};

#endif // MAINWINDOW_H
