#ifndef NOMBREDEPOINTS_H
#define NOMBREDEPOINTS_H

#include <QWidget>
#include "form.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDataStream>

namespace Ui {
class NombreDePoints;
}

//This first window asks the user how many corners the polygon has

class NombreDePoints : public QWidget
{
    Q_OBJECT

public:
    explicit NombreDePoints(QWidget *parent = 0);
    ~NombreDePoints();

private:
    Ui::NombreDePoints *ui;

    vector<double> m_xCoords;
    vector<double> m_yCoords;
    vector<double> m_order;

private slots:

//        opens a form window
        void formWindow();

//        loads a previously saved shape (saved in form window)
        void loadShape();
};

#endif // NOMBREDEPOINTS_H
