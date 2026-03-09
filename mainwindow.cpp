#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    receiver = new UdpReceiver(this);

    connect(receiver,
            &UdpReceiver::frameReceived,
            this,
            &MainWindow::handleFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleFrame(QByteArray data)
{
    if(data.size() < 10)
        return;

    const unsigned char *d =
        reinterpret_cast<const unsigned char*>(data.data());

    quint8 length = d[0];
    quint8 token  = d[1];
    quint8 status = d[2];

    quint32 timestamp =
        (d[3] << 24) |
        (d[4] << 16) |
        (d[5] << 8)  |
        d[6];

    quint8 byte1 = d[7];

    quint8 fcode = (byte1 >> 4) & 0x0F;

    quint16 address =
        ((byte1 & 0x0F) << 8) |
        d[8];

    quint8 dataByte = d[9];

    quint8 crc = (data.size() > 10) ? d[10] : 0;

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(length)));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(token)));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(status)));
    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(timestamp)));
    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(fcode)));
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::number(address)));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::number(dataByte)));
    ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString::number(crc)));
}
