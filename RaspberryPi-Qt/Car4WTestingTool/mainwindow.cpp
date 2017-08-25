#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*/////////////////////////////////////
    //  Inserta imagen en el label_Logo  //
    /////////////////////////////////////*/

    QPixmap picture(":/rec/img/Car4W.jpg");
    ui->label_Logo->setPixmap(picture.scaled(350,500, Qt::KeepAspectRatio));

    /*////////////////////////
    //  Configura el Timer  //
    ////////////////////////*/

    QTimer *cronometro=new QTimer(this);
    connect(cronometro, SIGNAL(timeout()), this, SLOT(fTimer()));
    cronometro->start(500);

    /*////////////////////////////////
    //  Configura el puerto serial  //
    ////////////////////////////////*/

    hw_is_available = false;
    hw_port_name = "";
    hw = new QSerialPort;
    ui->lineEdit_3->setText("9600");

    ui->lineEdit->setText(QString::number(QSerialPortInfo::availablePorts().length()));

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            ui->lineEdit_2->setText(QString::number(serialPortInfo.vendorIdentifier()));
            ui->lineEdit_5->setText(QString::number(serialPortInfo.productIdentifier()));
            hw_port_name = serialPortInfo.portName();
            hw_is_available = true;
        }
    }

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(serialPortInfo.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*///////////////////////////////////////////////////////////////////////
// Esta funcion se ejecuta cada tiempo especificado en el constructor  //
///////////////////////////////////////////////////////////////////////*/

void MainWindow::fTimer()
{
    if(hw->isWritable())
    {
        /*////////////////////////////////////////////////////////////////////////
        // Envia el comando de la dirección de auto a través del puerto serial  //
        ////////////////////////////////////////////////////////////////////////*/

        if      (ui->radioButton_Forward->isChecked())      control = "Forward..";
        else if (ui->radioButton_Backward->isChecked())     control = "Backward.";
        else if (ui->radioButton_Left->isChecked())         control = "Left.....";
        else if (ui->radioButton_Right->isChecked())        control = "Right....";
        else if (ui->radioButton_Stop->isChecked())         control = "Stop.....";

        if(hw_is_available)
        {
            if(hw->isWritable())
            {
                hw->write(control.toUtf8());
                qDebug() << "Enviado" << control.toUtf8();
            }
            else
            {
                QMessageBox::information(this,"Error","No se puede escribir");
            }
        }
    }
}

/*//////////////////////////////////////////////////////
// Inicia la comunicación a través del puerto serial  //
//////////////////////////////////////////////////////*/

void MainWindow::on_pushButton_clicked()
{
    QString bauds = ui->lineEdit_3->text();
    int vbauds = bauds.toInt();

    if(vbauds == 9600)
    {
        hw ->setBaudRate(QSerialPort::Baud9600);
        ui->lineEdit_4->setText(bauds);
    }

    if(hw_is_available)
    {
        hw ->setPortName(hw_port_name);
        hw->open(QIODevice::ReadWrite);
        hw->setDataBits(QSerialPort::Data8);
        hw->setParity(QSerialPort::NoParity);
        hw->setStopBits(QSerialPort::OneStop);
        hw->setFlowControl(QSerialPort::NoFlowControl);
    }
    else
    {
        QMessageBox::information(this,"Error","No esta disponible el puerto serial");
    }

}

/*///////////////////////////////////////////////////
// Termina la comunicación y cierra la aplicación  //
///////////////////////////////////////////////////*/

void MainWindow::on_pushButton_3_clicked()
{
    hw->close();
    QMessageBox::information(this,"Fin de Comunicación","Comunicación Serial Terminada!");
    close();
}
