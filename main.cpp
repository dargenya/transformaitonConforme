//Graphical user interface for Karman-Trefftz mapping of a polygon
//Project for an internship at ENSTA Bretagne (Brest, France) with the tutor Mr. Y-M Scolan
//summer (June, July, August) 2020
//Author: Yann d'Argenlieu (FISE 2021 naval engineering student at ENSTA Bretagne)


#include "nombredepoints.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NombreDePoints n;
    n.show();

    return a.exec();
}
