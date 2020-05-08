#ifndef STRUCT_AIS_H
#define STRUCT_AIS_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QBitArray>

class struct_ais
{
public:
    struct_ais(unsigned char canal,QString v_ais);
    ~struct_ais(); //Destructeur
    int get_n_mess();
    QString get_desc_ais(bool v_fich); //v_fich : from_fichier
    bool get_geoloc();
    qint32 get_mmsi();
    QString get_lat();
    QString get_lng();
    QString get_type_nav();
    qint16 get_sog();
    qint16 get_cog();
    QString get_stat_nav();
    QString get_rot();
    QString get_destination();
    float get_tir_eau();

private:
    char canal_emi;
    QBitArray tbit;//Tableau des eb de la partie DATA de la trame AIS
    unsigned char n_mess; //Numéro (type) du message 6 bits
    qint32 mmsi; //code du navire
    QString lat; //Latitude
    QString lng; //Longitude
    QString typ_nav; //Type navire
    qint16 dim_nav; //Longueur navire
    QString nom_nav; //Nom du navire
    qint16 sog; //SOG
    qint16 cog; //COG
    qint16 hdg;
    qint16 timestamp;
    QString stat_nav; //Status de navigation
    QString rot; //Taux de virage
    float tir_eau; //Tirant d'eau
    QString destination; //Destination
    bool geoloc; //Bouton geoloc
    QString get_typ_nav(qint8 vtyp_nav);
    QString get_status_nav(qint8 v_stat_nav);
    unsigned get_tbit_num(int vdeb, int vfin);
    unsigned get_tbit_num_cad(int vdeb, int vfin);
    qint8 get_tbit_num_signe(int vdeb,int vfin);
    QString get_tbit_string(int vdeb, int vfin);
    QString get_type_mess(qint8 v_typ_mess);
    QString get_mess_1_2_3();
    QString get_mess_5b();
    QString get_mess_24a();
    QString get_mess_24b();
    QString get_mess_18b();
    bool memo_histo(qint32 vmmsi, QString v_data); //Lecture d'une trame en analyse

    struct mess_1_2_3
    {
        qint32 mmsi; //Code du bateau (8-37)
        QString stat_nav; //Status de navigation (38-41)
        QString rot; //ROT Taux de virage (42-49)
        qint16 sog; //SOG (50-59)
        QString longitude; //Longitude (61-88)
        QString latitude; //Latitude (89-115)
        qint16 cog; //COG (116-127)
        qint16 hdg; //HDG (128-136)
        QString timestamp; //TimeStamp : second of UTC (137-142)

    }s1_2_3;

    struct mess_5_B
    {
        qint32 mmsi; //code du bateau (8-37)
        QString ind_appel; //Indicatif d'appel (70-111)
        QString nom_nav; //Nom du navire (112-231)
        QString typ_nav; //Type du navire (232-239)
        qint16 dim_nav; //longeur du navire (240-248) et (249-257)
        int mois; //mois (274-277)
        int jour; //jour(278-282)
        int heure; //heure(283-287)
        int minute; //minutes (288-293)
        float tir_eau; //Tirant d'eau (294-301)
        QString destination; //destination (302-421)
    }s5_b;

    struct mess_18_B
    {
        qint32 mmsi; //code du bateau (8-37)
        qint16 sog; //SOG(46-55);
        QString longitude; //Longitude (57-84)
        QString latitude; //Latitude (85-111)
        qint16 cog; //COG(112-123) Course Over Ground
        qint16 hdg; //HDG(124-132) True Heading
    }s18_b;

    struct mess_24_A
    {
        qint32 mmsi; //code du bateau (8-37)
        QString nom_nav; //Nom du navire (20 cars ascii 6 bits) ( 40-159)
    }s24_a;

    struct mess_24_B
    {
        qint32 mmsi; //code du batau (8-37)
        QString typ_nav; //type du navire (40-47)
        QString id_vend; //ID vendeur (48-65)
        QString ind_appel; //Indicatif d'appel (90-131)
        qint16 dim_nav; //Longeur du navire (132-149)
    }s24_b;
};

#endif // AIS_H
