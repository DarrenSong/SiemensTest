#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitSerialPort();

private slots:
    void on_btnOpen_clicked();

    void on_btnClose_clicked();

    void on_BtnConnect_clicked();

    void on_DisConnect_clicked();

    void On_SerialPort_Received();

    void on_btnOff_clicked();

    void on_btn250ms_clicked();

    void on_btn500ms_clicked();

    void on_btn1s_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
};
#endif // MAINWINDOW_H
