#ifndef STRUCT_NMEA_H
#define STRUCT_NMEA_H

#include <QMainWindow>
#include <sstream>
#include <QString>
//#include <iostream>

//#define SSTRHEX(x) static_cast< std::ostringstream & >(( std::ostringstream() << std::hex << x)).str()

//using namespace std;

struct ligne
{
    qint8 numl; //Numéro de la ligne de la description
    QString decript; //Description de la ligne
};

struct trame
{
    QString nm_trame; //Nom de la Trame
    bool suivi; //Trame suivie
    qint8 nbchp; // Nombre de champ de la trame
    ligne chp[21];
};

struct nmea
{
    trame deftram[10];
};

class struct_nmea
{
public:
    struct_nmea();
    ~struct_nmea();
    void init_nmea();
    QString affiche_def_tram(qint8 typ_trame);
    QString affiche_ligne_def_trame(qint8 typ_trame,qint8 nligne);
    bool trame_suivie(qint8 typ_trame);
    //qint8 struct_nmea::get_tram_id(QString typ_tr);
    bool get_chksum_trame();
    qint8 get_nb_ligne_desc_nmea(qint8 typ_trame);
    qint8 verif_tram_suivie(QString typ_tr);
    QString disseque_trame_ecran(QString ligne_trame);
    QString affiche_nmea_fichier(QString nom_fichier_nmea,qint8 ch_detail);
    void set_trame_suivie(qint8 v_tr, bool v_val);
    bool get_trame_suivie(qint8 v_tr);
private:
    nmea *v_nmea;
    bool chksum_trame;
};

#endif // STRUCT_NMEA_H
