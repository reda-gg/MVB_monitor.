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
    if(data.size() < 9)
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

    QString dataField = "";
    quint8 crc = 0;

    // MASTER FRAME
    if(token == 0x20 || token == 0x21)
    {
        // Master frame has NO DATA
        dataField = "";

        if(data.size() > 9)
            crc = d[9];
    }

    // SLAVE FRAME
    else if(token == 0x22 || token == 0x23)
    {
        if(data.size() > 9)
        {
            quint8 dataByte = d[9];
            dataField = QString("0x%1").arg(dataByte,2,16,QChar('0')).toUpper();
        }

        if(data.size() > 10)
            crc = d[10];
    }

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row,0,new QTableWidgetItem(
                                         QString("0x%1").arg(length,2,16,QChar('0')).toUpper()));

    ui->tableWidget->setItem(row,1,new QTableWidgetItem(
                                         QString("0x%1").arg(token,2,16,QChar('0')).toUpper()));

    ui->tableWidget->setItem(row,2,new QTableWidgetItem(
                                         QString("0x%1").arg(status,2,16,QChar('0')).toUpper()));

    ui->tableWidget->setItem(row,3,new QTableWidgetItem(
                                         QString("0x%1").arg(timestamp,8,16,QChar('0')).toUpper()));

    ui->tableWidget->setItem(row,4,new QTableWidgetItem(
                                         QString("0x%1").arg(fcode,1,16).toUpper()));

    ui->tableWidget->setItem(row,5,new QTableWidgetItem(
                                         QString("0x%1").arg(address,3,16,QChar('0')).toUpper()));

    ui->tableWidget->setItem(row,6,new QTableWidgetItem(dataField));

    ui->tableWidget->setItem(row,7,new QTableWidgetItem(
                                         QString("0x%1").arg(crc,2,16,QChar('0')).toUpper()));
}
