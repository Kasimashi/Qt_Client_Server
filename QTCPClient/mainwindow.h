#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void newMessage(QString);
private slots:
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void connecte();
    void deconnecte();
    void readSocket();
    void discardSocket();

    void displayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();
    void on_actionHERMES_triggered();
    void showTime();

    void on_actionOuvrir_un_fichier_AIS_txt_triggered();

    void on_Connect_pushButton_clicked();

    void on_ServerDisconnectpushButton_clicked();

    void on_ClearTextBrowerpushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket* socket;
};

#endif // MAINWINDOW_H
