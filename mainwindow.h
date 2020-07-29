#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include <vector>
#include "shape.h"
#include "drawarea.h"

using namespace std;

//main window where mappings are visualised and interacted with

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(vector<double> xCoordinates, vector<double> yCoordinates ,vector<int> order, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    vector<Shape> m_shapes;
    vector<DrawArea*> m_drawAreas;

//    number of vertices
    int m_vertices;

//    order in which vertices will be transformed
    vector<int> m_order;

//    past zc for total inverse transformation
    vector<complex<double>> m_zcs;
//    past centroids, after mapping and before centering shape
    vector<complex<double>> m_zetaCentroids;
//    past angles used for rotation
    vector<double> m_angles;

private slots:

//    when a draw area is clicked, shapes are transformed and visualised in draw areas
    void callTransform(const complex<double>& zc, int senderOrderIndex);

};

#endif // MAINWINDOW_H
