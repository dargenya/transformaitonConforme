#ifndef FORM_H
#define FORM_H

#include "mainwindow.h"

#include <QWidget>
#include <vector>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDataStream>

using namespace std;

//form window which creates initial polygon

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:

//    constructor used if only the number of points is given in previous window
    explicit Form(int nombrePoints, QWidget *parent = 0);

//    constructor used if a file has been loaded
    explicit Form(vector<double> xCoords, vector<double> yCoords, vector<double> order, QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;

    vector<QDoubleSpinBox*> m_xBoxes;
    vector<QDoubleSpinBox*> m_yBoxes;
    vector<QSpinBox*> m_orderBoxes;



private slots:

//    opens the main window for transformations
    void mainWindow();

//    allows user to save polygon in .txt for future use
    void saveFigure();
};

#endif // FORM_H
