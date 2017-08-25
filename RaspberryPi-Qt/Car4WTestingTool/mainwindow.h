#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QSerialPort; //Definimos la nueva clase para el puerto serial

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:

        void on_pushButton_clicked();
        void on_pushButton_3_clicked();

        void fTimer();

public slots:

    private:

        Ui::MainWindow *ui;
        QSerialPort *hw; //Creamos el objeto
        QString hw_port_name;
        bool hw_is_available;
        QString control = "";
};

#endif // MAINWINDOW_H
