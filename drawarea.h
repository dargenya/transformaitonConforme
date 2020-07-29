#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <vector>
#include <complex>

using namespace std;

class DrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent = nullptr, int vertexIndex = 0, int orderIndex = 0);

//    int getm_vertexIndex();

    void receiveDraw(const vector<complex<double>>& z_vertices, const vector<complex<double>>& z_mesh, const double& radius, const double& minimise);
    void receiveDrawZc(const complex<double>& zc);
    void receiveDrawCircle();

    //inverse
    void receiveInverseDraw(const vector<complex<double>>& z_invMesh);

protected:
//    called in every update(), does the actual drawing
    void paintEvent(QPaintEvent *event);

//when mouse is clocked, held, released
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

//    wheel zooms in or out by changing m_scale
    void wheelEvent(QWheelEvent *event);

private:

//    scale when going from the complex plane to the drawing plane
    double m_scale;

//    the current vertex number, used for rotating
    int m_vertexIndex;
//    the order in which a DrawArea appears in the mainwindow, used by MainWindow::callTransform()
    int m_orderIndex;

//    vertices and mesh in the drawing plane
    QPolygonF m_vertices;
    QPolygonF m_mesh;

//    vertices and mesh in the complex plane
    vector<complex<double>> m_zVertices;
    vector<complex<double>> m_z;

//radius of the shape, used to set an initial scale
    double m_radius;

//    display the minimmisation performance factor
    QString m_minimise;

    bool m_drawCircle;
//    circle in complex plane
    vector<complex<double>> m_zcircle;
//    circle in drawing plane
    QPolygonF m_circle;

//    zc (used in mapping) in complex plane
    complex<double> m_zc;
    //    zc (used in mapping) in drawing plane
    QPointF m_clickPoint;

//    transformations between complex and drawing planes
    QPolygonF localToPaint(const vector<complex<double>>& zLocals);
    QPointF localToPaint(const complex<double>& z);
    complex<double> paintToLocal(const QPointF& paint_point);

    //is left mouse button held down and thus transformation
    bool transforming;
    //what to do while left click
    void leftClick(QMouseEvent *event);

    //inverse
    vector<complex<double>> m_zinvMesh;
    QPolygonF m_inverseMesh;


signals:
//    when clicked, emits signal to mainwindow
    void needsTransform(const complex<double> &zc, int senderIndex);

};

#endif // DRAWAREA_H
