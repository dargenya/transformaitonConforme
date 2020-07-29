#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <complex>

//write file
#include <iostream>
#include <fstream>

using namespace std;

class Shape
{
public:

//    A default constructor that was used before the first polygon option was implemented
    Shape();
//    constructor used for the first polygon
    Shape(vector<double> xCoordinates, vector<double> yCoordinates);
//    constructor used for subsequent shapes, and where the index must be defined and the angles are passed on
    Shape(int index, vector<double> alphas);
    ~Shape();

    vector<complex<double>> m_zVertices;
    vector<complex<double>> m_z;

//    image after mapping
    vector<complex<double>> m_zetaVertices;
    vector<complex<double>> m_zeta;

//    centroid of the zeta shape (after transformation)
    complex<double> m_zetaCentroid;
//    radius of the zeta shape; an average, of centre = zetaCentroid
    double m_zetaRadius;
    //parameter which is small when shape is close to circle
    double m_minimise;

    complex<double> m_zc;

//    function which does the mapping, centering and rotation
    void transform(const int& alignVertexIndex);

    //inverse
    vector<complex<double>> invTransform();

//    obsolete
    void m_zetaCentroidInit();


    void m_zetaRadiusInit();
    void m_minimiseInit();

    //    plot radius vs angle into a txt file
    void plot();

    //inner angles of the polygon
    vector<double> m_alphas;

    //useful for inverse
    double m_angle;

    //inverse
    vector<complex<double>> invMap(const vector<complex<double> > &variables, const double &alpha, const complex<double> &zt, const complex<double> &zc);
    vector<complex<double>> invMap(const vector<complex<double> > &variables, const complex<double>& zc);
    vector<complex<double>> invRotation(const vector<complex<double>>& variables);
    vector<complex<double>> invRotation(const vector<complex<double>>& variables, const double& angle);
    vector<complex<double>> invCenter(const vector<complex<double>>& variables);
    vector<complex<double>> invCenter(const vector<complex<double>>& variables, const complex<double>& old_zcentroid);



private:
    //vertex index used for mapping
    int m_vertexIndex;

//    Obsolete default constructor that was used before the first polygon option was implemented
    void m_zInit();
//    constructor used for the first polygon
//    This is where the number of nodes is implemented!
    void m_zInit(const vector<double>& xCoordinates, const vector<double>& yCoordinates);
    void m_alphasInit();

//Karman-Trefftz mapping
    vector<complex<double>> map(const vector<complex<double> > &variables, const double &alpha, const complex<double> &zt, const complex<double> &zc);
//    rotation so that centroid and the next vertex used for transformation are aligned horizontally
    vector<complex<double>> rotation(const vector<complex<double>>& variables, const int& alignVertexIndex );
//    translates shape so that centroid is at (0, 0)
    vector<complex<double>> center(const vector<complex<double>>& variables);

//    subroutine to calculate the inner angles (alphas) correctly. Note that the corners of the polygon , must be given in trig order
    double innerAngle(double prevArg, const double& nextArg);
};

#endif // SHAPE_H
