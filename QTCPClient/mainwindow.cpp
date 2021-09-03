#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "struct_ais.h"

#include <QTime>
#include <QTimer>
#include <QDateTime>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this); //Création de l'instance de QTcpSocket

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    //socket->connectToHost(QHostAddress::LocalHost,PORT);

    connect(this,SIGNAL(newMessage(QString)),this,SLOT(displayMessage(QString)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readSocket()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(deconnecte()));
    connect(socket,SIGNAL(connected()),this,SLOT(connecte()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    /*
    if(socket->waitForConnected()) this->ui->statusBar->showMessage("Connected to Server");
    else{
        QMessageBox::critical(this,"QTCPClient", QString("The following error occurred: %1.").arg(socket->errorString()));
        //exit(EXIT_FAILURE);
    }
    */

    setWindowTitle("Analyseur de trames NMEA183");

    on_Connect_pushButton_clicked(); //Connection automatique au serveur
    //v_n = new struct_nmea();
    //posais = new tab_pos_ais();
    //d_ais = new tab_data_ais();
}

/* Destructeur */
MainWindow::~MainWindow()
{
    if(socket->isOpen())
        socket->close();
    delete ui;
}

/******* Gestion des erreurs de transmission IP ************/
void MainWindow::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
        case QAbstractSocket::HostNotFoundError:
            ui->textBrowser_receivedMessages->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifier l'IP et le port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            ui->textBrowser_receivedMessages->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifier si le programme \"serveur\" a bien été lancé. Vérifier l'IP et le port. </em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            ui->textBrowser_receivedMessages->append(tr("<em>ERREUR : le serveur a coupé la connexion</em>"));
            break;
        default:
            ui->textBrowser_receivedMessages->append("<em>ERREUR : " + socket->errorString());
    }
    ui->Connect_pushButton->setEnabled(true);
    ui->Port_spinBox->setEnabled(true);
    ui->ServerIP_lineEdit->setEnabled(true);
}

void MainWindow::connecte()
{
    displayMessage(tr("<em>Connection TCP IP réussie !</em>"));
    ui->statusBar->showMessage("Connected to Server");
    ui->Connect_pushButton->setEnabled(false);
    ui->Port_spinBox->setEnabled(false);
    ui->ServerIP_lineEdit->setEnabled(false);
}

void MainWindow::deconnecte()
{
    //if(socket->isOpen())
        //socket->close();
        //socket->abort();
    ui->textBrowser_receivedMessages->append(tr("<em>Déconnecté TCP IP du serveur !</em>"));
    ui->statusBar->showMessage("Disconnected to Server");
    ui->Connect_pushButton->setEnabled(true);
    ui->Port_spinBox->setEnabled(true);
    ui->ServerIP_lineEdit->setEnabled(true);

}

void MainWindow::on_Connect_pushButton_clicked()
{
    this->ui->textBrowser_receivedMessages->append(tr("<em>Tentative du connection TCP IP !</em>"));
    ui->Connect_pushButton->setEnabled(false);
    socket->abort();
    socket->connectToHost(ui->ServerIP_lineEdit->text(), ui->Port_spinBox->value());
}

void MainWindow::on_ServerDisconnectpushButton_clicked()
{
    socket->abort();
    //socket->deleteLater();
    //socket=nullptr;
    deconnecte();
}


void MainWindow::readSocket()
{
    QByteArray block = socket->readAll();

    QDataStream in(&block, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_12);

    while (!in.atEnd())
    {
        QString receiveString;
        in >> receiveString;
        receiveString.prepend(QString("%1 : ").arg(socket->socketDescriptor()));
        emit newMessage(receiveString);
    }
}

void MainWindow::discardSocket()
{
    socket->deleteLater();
    socket=nullptr;
    //if(socket->isOpen())
     //   socket->close();
    //socket->abort();
    this->ui->textBrowser_receivedMessages->append(tr("<em>Connection TCP IP perdue !</em>"));
    this->ui->statusBar->showMessage("Disconnected!");
    ui->Connect_pushButton->setEnabled(true);
    ui->Port_spinBox->setEnabled(true);
    ui->ServerIP_lineEdit->setEnabled(true);
}


void MainWindow::on_pushButton_sendMessage_clicked()
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = this->ui->lineEdit_message->text();

            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            out.setVersion(QDataStream::Qt_5_12);
            out << str;

            socket->write(block);
            this->ui->textBrowser_receivedMessages->append("<em>LOG:MessageSendToServer: </em>" + this->ui->lineEdit_message->text());
            this->ui->lineEdit_message->clear();
        }
        else
            QMessageBox::critical(this,"QTCPClient","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPClient","Not connected");
}

void MainWindow::displayMessage(const QString& str)
{
    this->ui->textBrowser_receivedMessages->append(str);
}

void MainWindow::on_actionHERMES_triggered()
{
    QMessageBox::critical(this,"A propos","Projet COVID-19- Alex LLORENTE.");
}

void MainWindow::showTime()
{
    QTime time=QTime::currentTime();
    QDateTime date=QDateTime::currentDateTimeUtc();
    QString time_text = time.toString("hh:mm:ss");
    QString data_text = date.toString();
    ui->Digital_Clock->setText(time_text);
    ui->Digital_Date->setText(data_text);

}

void MainWindow::on_actionOuvrir_un_fichier_AIS_txt_triggered()
{
    QStringList l_1;
    struct_ais *v_ais = nullptr;
    //QString l_1
    QString result;
    QString nom_fichier_ais = QFileDialog::getOpenFileName(this,"Ouvrir un fichier AIS", QString(), "Texte (*.txt)");
    qDebug() << nom_fichier_ais;
    if (nom_fichier_ais != NULL){
        QFile fichier(nom_fichier_ais);
        QString texte, v_ligne;
        if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //d_ais ->nb =0;
            //posais->nb = 0;
            QTextStream flux(&fichier);
            qDebug() << "Lecture du fichier en cours";
            while(!flux.atEnd()){
                v_ligne = flux.readLine();
                l_1 = v_ligne.split(',');
                if(l_1[0] == "!AIVDM"){
                    ui->textBrowser_receivedMessages->append(l_1[5]);
                    v_ais = new struct_ais('A',l_1[5]);
                    result = v_ais->get_desc_ais(true);
                    ui->textBrowser_receivedMessages->append("Début de la description de la trame");
                    ui->textBrowser_receivedMessages->append(result);
                    ui->textBrowser_receivedMessages->append("------------------------------------");
                    //result = v_ais->get_desc_ais(true);
                }
            }
            fichier.close();
            //QDesktopServices::openUrl(QUrl("aff_carte.html"));
        }
        else{
            ui->textBrowser_receivedMessages->append("Impossible d'ouvrir le fichier !!");
        }
    }
    delete(v_ais);
}

void MainWindow::on_ClearTextBrowerpushButton_clicked()
{
    ui->textBrowser_receivedMessages->clear();
}
