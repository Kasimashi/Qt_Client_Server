#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "struct_ais.h"


struct_ais::~struct_ais() {
   qDebug() << "Object is being deleted";
}

int struct_ais::get_n_mess()
{
    return n_mess;
}

qint32 struct_ais::get_mmsi()
{
    return mmsi;
}
QString struct_ais::get_lat()
{
    return lat;
}
QString struct_ais::get_lng()
{
    return lng;
}
QString struct_ais::get_type_nav()
{
    return typ_nav;
}
qint16 struct_ais::get_sog()
{
    return sog;
}
qint16 struct_ais::get_cog()
{
    return sog;
}
QString struct_ais::get_stat_nav()
{
    return stat_nav;
}
QString struct_ais::get_rot()
{
    return rot;
}
QString struct_ais::get_destination()
{
    return destination;
}
float struct_ais::get_tir_eau()
{
    return tir_eau;
}

QString get_type_mess(qint8 v_typ_mess)
{
    switch (v_typ_mess)
    {
        case 1:
            return "Position Report Class A";
        case 2:
            return "Position Report Class A (Assigned schedule)";
        case 3:
            return "Position Report Class A (Response to interrogation)";
        case 4:
            return "Base Station Report";
        case 5:
            return "Static and Voyage Related Data";
        case 6:
            return "Binary Addressed Message";
        case 7:
            return "Binary Acknowledge";
        case 8:
            return "Binary Broadcast Message";
        case 9:
            return "Standard SAR Aircraft Position Report";
        case 10:
            return "UTC and Date Inquiry";
        case 11:
            return "UTC and Date Response";
        case 12:
            return "Addressed Safety Related Message";
        case 13:
             return "Safety Related Acknowledgement";
        case 14:
            return "Safety Related Broadcast Message";
        case 15:
            return "Interrogation";
        case 16:
            return "Assignment Mode Command";
        case 17:
            return "DGNSS Binary Broadcast Message";
        case 18:
            return "Standard Class B CS Position Report";
        case 19:
            return "Extended Class B Equipment Position Report";
        case 20:
            return "Data Link Management";
        case 21:
            return "Aid-to-Navigation Report";
        case 22:
            return "Channel Management";
        case 23:
            return "Group Assignment Command";
        case 24:
            return "Static Data Report";
        case 25:
            return "Single Slot Binary Message";
        case 26:
            return "Multiple Slot Binary Message With Communications State";
        case 27:
            return "Position Report For Long-Range Applications";
        default:
            return "Type unknown()";
    }
}
QString get_mess_1_2_3(){
    return "";
}
QString get_mess_5b(){
    return "";
}
QString get_mess_24a(){
    return "";
}
QString get_mess_24b(){
    return "";
}
QString get_mess_18b(){
    return "";
}

QString struct_ais::get_desc_ais(bool v_fich){
    QString out = "";
    if (v_fich){

    }else{

    }
    out = "Message Type :" + QString::number(n_mess) + "\nMMSI :" + QString::number(mmsi) + "\n" + stat_nav + "\nLat :"+ get_lat() + "\nLng :"+ get_lng() +"\nRot:" + get_rot() +"\nCOG:" + QString::number(cog) +"\nHDG:" + QString::number(hdg) +"\nTimeStamp:" + QString::number(timestamp);
    return out;
}

struct_ais::struct_ais(unsigned char canal,QString v_ais)
{
    int v_eb6;
    tbit.resize(1024); //taille max data bin du message AIS (il peut y en avoir plus d'un)
    tbit.fill(false); //Tous les bits du tableau à 0
    qint32 postbit=0; //Lecture du bit en cours
    /**Le principe, on lit un octet (correspondant à un caractère ascii 8 bits
     * (ceux que l'on voit dans le message de base)) on soustrait 48
     * si c'est > à 40 on soustrait à nouveau 8
     * et on ne garde que les 6 bits de poids faibles,
     * ensuite on passe au caractère suivant.**/
    for (int i=0; i<v_ais.size();i++)
    {
        v_eb6 = v_ais[i].toLatin1() - 48;
        if (v_eb6 > 40) v_eb6 =v_eb6 - 8;
        if (i==0){
            n_mess = (int)v_eb6; //On ne regarde que les 6 bits de poids faible.
        }
        postbit += 6;
        qDebug() << "v_ais :" + v_ais + " nmess :"+ QString::number(postbit) + " Postbit :" + QString::number(postbit);
        for (int neb = 0; neb<6; neb++){
            tbit.setBit(postbit-neb,(((v_eb6 >> neb) & 1) == 1) ? true : false);
        }

        switch (n_mess) {
        case 1: case 2 : case 3 :
            if (postbit == 42) s1_2_3.mmsi = mmsi = get_tbit_num(9,38); // Code du bateau (8-37 (tbit : 9 à 38)
            if (postbit == 48) s1_2_3.stat_nav = stat_nav = get_status_nav((get_tbit_num(39,42))); //Status de Navigation (38-41)
            if (postbit == 132){
                qint8 vtemp = get_tbit_num_signe(43,50)/10;
                if(vtemp == 0){s1_2_3.rot = rot = "Ne tourne pas.";}
                else{if (vtemp < 0 && vtemp >= -126){s1_2_3.rot = rot = "Virage gauche lent de " + QString::number(vtemp) + "°";}
                else{if(vtemp >0 && vtemp <= 126){s1_2_3.rot = rot = "Virage droite lent de " + QString::number(vtemp) + "°";}
                else{if(vtemp == -127){s1_2_3.rot = rot = "Virage gauche rapide de " + QString::number(vtemp) + "°";}
                else{if(vtemp == 127 ){s1_2_3.rot = rot = "Virage droite rapide de " + QString::number(vtemp) + "°";}
               }}}}
            }
            if (postbit == 66) s1_2_3.sog = sog = (float)get_tbit_num(51,60)/10; //Il faut diviser par 10
            if (postbit == 90){ //Longitude (61-88)
                if (tbit.testBit(62)==false){
                       float v_long = get_tbit_num(62,89)/60;
                       lng = QString::number(v_long/10000,'g',6);
                       //qDebug() << "Vlong:" + QString::number(v_long);
                       s1_2_3.longitude = lng + " Est";
                }else{
                    float v_long = get_tbit_num_cad(62,89)/60;
                    lng = QString::number(v_long/10000*(-1),'g',6);
                    s1_2_3.longitude = lng + " Ouest";
                }
            }
            if (postbit ==120){//Latitude(89-115)
                if (tbit.testBit(90)==false){
                    float v_long = get_tbit_num(90,116)/60;
                    lat = QString::number(v_long/10000,'g',6);
                    s1_2_3.latitude = lat + " Nord";
                }else{
                    if (tbit.testBit(90)==false){
                    float v_long = get_tbit_num_cad(90,116)/60;
                    lat = QString::number(v_long/10000*(-1),'g',6);
                    s1_2_3.latitude = lat + " Sud";
                    }
                }
            }
            if (postbit == 132) s1_2_3.cog = cog = get_tbit_num(117,128)/10; //COG (116-117)
            if (postbit == 138) s1_2_3.hdg = hdg = get_tbit_num(129,137); //HDG (128-136)
            if (postbit == 144) s1_2_3.timestamp = timestamp = get_tbit_num(138,143); //TimeStamp : second of UTC (137-142)
            break;
        default:
                //qDebug() << "Type de message inconnu ... :(";
                break;
        }

    }
}

//Retourne une string à partir d'eb du tableau.
QString struct_ais::get_tbit_string(int vdeb, int vfin){
    QString lg1s;
    unsigned char lg1 = 0;
    qint16 cpt = vdeb;
    while (cpt <= vfin){
        if(vfin-cpt >= 6){
            lg1=0;
            for (int v_1=1; v_1 <=6 ; v_1++){
                lg1= lg1 << 1;
                if (tbit.testBit(cpt)==true) lg1 = lg1 | 0x01;
                cpt++;
            }
            lg1s += ((lg1<32) ? (char)lg1+64 : (char)lg1);
        }else{
            cpt = vfin +1;
        }
    }
    return lg1s;
}

//Retourne un qint8 signé à partir d'eb du tableau
qint8 struct_ais::get_tbit_num_signe(int vdeb, int vfin){
    qint8 lg1 = 0;
    for (int v_1=vdeb; v_1 <=vfin ; v_1++){
        lg1 = lg1 << 1;
        if (tbit.testBit(v_1)==true) lg1 = lg1 | 0x00000001;
    }
    return lg1;
}

//Retour un integer non signé à partir d'eb du tableau.
unsigned struct_ais::get_tbit_num(int vdeb, int vfin){
    unsigned lg1 = 0;
    for (int v_1=vdeb; v_1 <= vfin; v_1++){
        lg1 = lg1 << 1;
        if (tbit.testBit(v_1)==true) lg1 = lg1 | 0x00000001;
    }
    return lg1;
}

//Retour un integer non signé en complément à 2 à partir d'eb du tableau.
unsigned struct_ais::get_tbit_num_cad(int vdeb, int vfin){
    unsigned lg1 = 0;
    for (int v_1=vdeb; v_1 <= vfin; v_1++){
        lg1 = lg1 << 1;
        if (tbit.testBit(v_1)==false) lg1 = lg1 | 0x00000001;
    }
    return lg1 + 1;
}

//Liste du type des navires.
QString struct_ais::get_typ_nav(qint8 vtype_nav){
    QString styp_nav = "Inconnu";
    switch(vtype_nav){
        case 0 : styp_nav = "Non disponible (par défaut)" ; break;
        case 1 : case 2 :case 3 :case 4 :case 5 :case 6 :case 7 :case 8 : case 9 :case 10 : case 11: case 12 : case 13 : case 14 : case 15 : case 16 : case 17 : case 18 : case 19 : styp_nav = "Réservé pour une utilisation future"; break;
        case 20 : styp_nav = "Wing in Ground (WIG), tous les navires de ce type"; break;
        case 21 : styp_nav = "Wing in Ground (WIG) cat danger A"; break;
        case 22 : styp_nav = "Wing in Ground (WIG) cat danger B"; break;
        case 23 : styp_nav = "Wing in Ground (WIG) cat danger C"; break;
        case 24 : styp_nav = "Wing in Ground (WIG) cat danger D"; break;
        case 25 : case 26 : case 27: case 28 : case 29 : styp_nav = "Réservé pour une utilisation future"; break;
        case 30 : styp_nav = "Pêche"; break;
        case 31 : styp_nav = "Remorquage"; break;
        case 32 : styp_nav = "Remorquage long. > 200m ou larg > 25m"; break;
        case 33 : styp_nav = "Draguage ou opérations sous marines"; break;
        case 34 : styp_nav = "Navire en opération de plongée"; break;
        case 35 : styp_nav = "Navire en opération militaires"; break;
        case 36 : styp_nav = "Voilier"; break;
        case 37 : styp_nav = "Bateau de plaisance"; break;
        case 40 : case 41 : case 42 : case 43 : case 44 : case 45 : case 46 : case 47 : case 48 : case 49 : styp_nav = "Embarcation à grande vitesse"; break;
        case 50 : styp_nav="Navire pilote."; break;
        case 51 : styp_nav="Navire de Recherche / Sauvetage."; break;
        case 52 : styp_nav="Navire remorqueur"; break;
        case 53 : styp_nav="Embarcation portuaire"; break;
        case 54 : styp_nav="Navire doté d'équipements antipollution"; break;
        case 55 : styp_nav="navire des forces de l'ordre"; break;
        case 56 : styp_nav="Réservé navires locaux"; break;
        case 57 : styp_nav="Réservé navires locaux"; break;
        case 58 : styp_nav="Transport sanitaire."; break;
        case 59 : styp_nav="Navire/Aéroned non eng. dans un conflit armé."; break;
        case 60 : case 61 : case 62 : case 63 : case 64 : case 65 : case 66 : case 67 : case 68 : case 69 : styp_nav="Navire de croisière (passagers)"; break;
        case 70 : case 71 : case 72 : case 73 : case 74 : case 75 : case 76 : case 77 : case 78 : case 79 : styp_nav="Cargo"; break;
        case 80 : case 81 : case 82 : case 83 : case 84 : case 85 : case 86 : case 87 : case 88 : case 89 : styp_nav="Pétrolier"; break;
    }
    return styp_nav;
}

//Liste des types de navire
QString struct_ais::get_status_nav(qint8 v_stat_nav){
    QString s_stat_nav = "Inconnu";
    switch (v_stat_nav) {
        case 0 : s_stat_nav = "En route avec moteur."; break;
        case 1 : s_stat_nav = "Ancré."; break;
        case 2 : s_stat_nav = "Pas sous commandement."; break;
        case 3 : s_stat_nav = "Manoeuvrabilité restreinte."; break;
        case 4 : s_stat_nav = "Contraint par son projet."; break;
        case 5 : s_stat_nav = "Amarré."; break;
        case 6 : s_stat_nav = "Echoué."; break;
        case 7 : s_stat_nav = "En action de pêche."; break;
        case 8 : s_stat_nav = "En navigation."; break;
        case 9 : s_stat_nav = "Réservé modif future HSC."; break;
        case 10 : s_stat_nav = "Réservé modif future WIG"; break;
        case 11 : case 12 : case 13 : s_stat_nav = "Réservé utilisation future."; break;
        case 14 : s_stat_nav = "AIS-SART est actif"; break;
        case 15 : s_stat_nav = "Non défini. (Par défaut)"; break;
    }
    return s_stat_nav;
}
