#include "shape.h"


Shape::Shape()
{
    m_zVertices = vector<complex<double>>(0);
    m_z = vector<complex<double>>(0);
    m_zInit();

    m_alphas = vector<double>(0);
    m_alphasInit();

    //default construcor initialises zeta=z
    m_zetaVertices = m_zVertices;
    m_zeta = m_z;

    m_zc = complex<double>(0);

    m_vertexIndex = int(0);

    m_zetaCentroid = complex<double>(0);
    m_zetaCentroidInit();

    m_zetaRadius = double(0);
    m_zetaRadiusInit();

    m_minimise = double(0);
    m_minimiseInit();

}

Shape::Shape(vector<double> xCoordinates, vector<double> yCoordinates)
{
    m_zVertices = vector<complex<double>>(0);
    m_z = vector<complex<double>>(0);
    m_zInit(xCoordinates, yCoordinates);

    m_alphas = vector<double>(0);
    m_alphasInit();

    //default construcor initialises zeta=z
    m_zetaVertices = m_zVertices;
    m_zeta = m_z;

    m_zc = complex<double>(0);

    m_vertexIndex = int(0);

    m_zetaCentroid = complex<double>(0);
    m_zetaCentroidInit();

    m_zetaRadius = double(0);
    m_zetaRadiusInit();

    m_minimise = double(0);
    m_minimiseInit();
}

Shape::Shape(int index, vector<double> alphas)
{
    m_zVertices = vector<complex<double>>(0);
    m_z = vector<complex<double>>(0);
    m_zInit();

//    m_alphas = vector<double>(0);
//    m_alphasInit();
    m_alphas = alphas;

    //default construcor initialises zeta=z
    m_zetaVertices = m_zVertices;
    m_zeta = m_z;

    m_zc = complex<double>(0);

    m_vertexIndex = index;

    m_zetaCentroid = complex<double>(0);
    m_zetaCentroidInit();

    m_zetaRadius = double(0);
    m_zetaRadiusInit();

    m_minimise = double(0);
    m_minimiseInit();
}

Shape::~Shape()
{

}

void Shape::transform(const int &alignVertexIndex)
{
//    A transformation is a map
//    then a shift to place the centroid at the origin,
//    then a rotation such that the next vertice is to the left of the centroid(now the origin)

    m_zeta = map(m_z, m_alphas[m_vertexIndex], m_zVertices[m_vertexIndex], m_zc);
    m_zetaVertices = map(m_zVertices, m_alphas[m_vertexIndex], m_zVertices[m_vertexIndex], m_zc);

    m_zetaCentroidInit();
    m_zeta = center(m_zeta);
    m_zetaVertices = center(m_zetaVertices);

//    m_zetaCentroidInit();
    m_zeta = rotation(m_zeta, alignVertexIndex);
    m_zetaVertices = rotation(m_zetaVertices, alignVertexIndex);

    m_zetaRadiusInit();
    m_minimiseInit();
}

vector<complex<double> > Shape::invTransform()
{
    vector<complex<double> > inverse;

    inverse = invRotation(m_zeta);
    inverse = invCenter(inverse);
    inverse = invMap(inverse, m_alphas[m_vertexIndex], m_zVertices[m_vertexIndex], m_zc);

    return inverse;
}



vector<complex<double> > Shape::map(const vector<complex<double> > &variables, const double &alpha, const complex<double> &zt, const complex<double> &zc)
{
//    explicit calculations of operations with complex numbers is done in the hope that transformation was conducted correctly (it did not)

    vector<complex<double> > zetas;
    double b(1./alpha);    //as done in the Fortran code by Mr.Scolan

    for(unsigned int i(0); i<variables.size(); i++){
        complex<double> z = variables[i];

        complex<double> a = (abs(z-zt)/abs(z-zc)) * exp(complex<double> (0, arg(z-zt) - arg(z-zc))) /*(z-zt) / (z-zc)*/;

        complex<double> aPb = pow(abs(a),b) * exp(complex<double> (0, arg(a) * b))/*pow(a,b)*/;

        zetas.push_back((abs(1.+aPb)/abs(1.-aPb)) * exp(complex<double> (0, arg(1.+aPb) - arg(1.-aPb))) /*(1.+aPb) / (1.-aPb)*/);
    }
    return zetas;
}

void Shape::m_zInit()
{
    //m_zVertices initialisation
    vector<double> x(0);
    vector<double> y(0);

    x.push_back(1.);
    x.push_back(1.);
    x.push_back(3.);
    x.push_back(3.);

    y.push_back(1.0);
    y.push_back(-1.);
    y.push_back(-1.);
    y.push_back(1.0);

    for(unsigned int i(0); i<x.size(); i++){
        complex<double> c(x[i], y[i]);
        m_zVertices.push_back(c);
    }

    //m_z initialisation
    //calculating perimeter is necessary for creating m_z , the mesh for m_zVertices

    vector<double> lengths(0);
    for(unsigned int i(0); i < x.size() - 1; i++){
        lengths.push_back(
                    pow(
                        pow((x[i+1] - x[i]),2) +
                        pow((y[i+1] - y[i]),2) ,
                    0.5));
    }

    double perim(0);
    for(unsigned int i(0); i<lengths.size(); i++){
        perim += lengths[i];
    }


    //number of nodes (or segments idk) for the mesh
    int subdivs(1200);

    //for every vertex except last
    for(unsigned int i(0); i < m_zVertices.size() - 1; i++){
        //n is number of subdivisions on edge #i
        double n = subdivs * lengths[i]/perim;

        for(double k(0); k < n; k++){
            m_z.push_back(m_zVertices[i] +
                                (m_zVertices[i+1] - m_zVertices[i]) *
                                k/n);
        }
    }

    //do the last edge
        double n = subdivs * lengths.back()/perim;

        for(double k(0); k < n; k++){
            m_z.push_back(m_zVertices.back() +
                                (m_zVertices[0] - m_zVertices.back()) *
                                k/n);
        }

}

void Shape::m_zInit(const vector<double> &xCoordinates, const vector<double> &yCoordinates)
{
    //m_zVertices initialisation
    vector<double> x(0);
    vector<double> y(0);

    for(unsigned int i(0); i<xCoordinates.size(); i++){
        x.push_back(xCoordinates[i]);
        y.push_back(yCoordinates[i]);
    }


    for(unsigned int i(0); i<x.size(); i++){
        complex<double> c(x[i], y[i]);
        m_zVertices.push_back(c);
    }

    //calculating perimeter is necessary for creating m_z , the mesh for m_zVertices

//    lengths of edges
    vector<double> lengths(0);
    for(unsigned int i(0); i < x.size() - 1; i++){
        lengths.push_back(
                    pow(
                        pow((x[i+1] - x[i]),2) +
                        pow((y[i+1] - y[i]),2) ,
                    0.5));
    }

//    total perimeter
    double perim(0);
    for(unsigned int i(0); i<lengths.size(); i++){
        perim += lengths[i];
    }


    //number of nodes (or segments idk) for the mesh
    int subdivs(1200);

    //for every vertex except last
    for(unsigned int i(0); i < m_zVertices.size() - 1; i++){
        //n is number of subdivisions on edge #i
        double n = subdivs * lengths[i]/perim;

        for(double k(0); k < n; k++){
            m_z.push_back(m_zVertices[i] +
                                (m_zVertices[i+1] - m_zVertices[i]) *
                                k/n);
        }
    }

    //do the last edge
        double n = subdivs * lengths.back()/perim;

        for(double k(0); k < n; k++){
            m_z.push_back(m_zVertices.back() +
                                (m_zVertices[0] - m_zVertices.back()) *
                                k/n);
        }
}

void Shape::m_alphasInit()
{

    //alphas calculated as done in Fortran code by Mr.Scolan
    //do angle #0 first
    m_alphas.push_back(innerAngle(arg(m_zVertices.back() - m_zVertices[0]), arg(m_zVertices[1] - m_zVertices[0]))
                        /M_PI);

    //do other angles
    for(unsigned int i(1); i<m_zVertices.size() - 1; i++){
        m_alphas.push_back(innerAngle(arg(m_zVertices[i-1] - m_zVertices[i]), arg(m_zVertices[i+1] - m_zVertices[i]))
                            /M_PI);
    }

    //do last angle
    m_alphas.push_back(innerAngle(arg(m_zVertices[m_zVertices.size()-2] - m_zVertices.back()), arg(m_zVertices[0] - m_zVertices.back()))
                        /M_PI);

}

void Shape::m_minimiseInit()
{
    //standard deviation of m_zetas from the circle centred at centroid and of average radius
    //aka Euclidian-norm aka N2
    double minimum(0);
    double denom(m_zeta.size());

    for(unsigned int i (0); i< denom; i++){
        minimum += pow(abs(/*m_zetaCentroid*/ - m_zeta[i])
                       - m_zetaRadius
                    ,2);
    }

    minimum /= denom;

    m_minimise = sqrt(minimum);

}

void Shape::plot()
{
    ofstream myfile;
    myfile.open ("radiusVariationPlot.txt");

//    myfile << "radius";
//    myfile<<";";
//    myfile<<"angle";
//    myfile<<";";
//    myfile<<"average radius";
//    myfile<<"\n";

//    columns: radius | angle | average radius
    for(unsigned int i(0); i< m_zeta.size(); i++){
        myfile << abs(/*m_zetaCentroid*/ - m_zeta[i]);
        myfile<<";";
        myfile<<arg(/*m_zetaCentroid*/ - m_zeta[i]);
        myfile<<";";
        myfile<<m_zetaRadius;
        myfile<<"\n";
    }

    myfile.close();
}

void Shape::m_zetaCentroidInit()
{
    double denom(m_zeta.size());
    double xmean(0);
    double ymean(0);

    for(int i(0); i< denom; i++){
        xmean += m_zeta[i].real();
        ymean += m_zeta[i].imag();
    }

    xmean /= denom;
    ymean /= denom;

    m_zetaCentroid.real(xmean);
    m_zetaCentroid.imag(ymean);
}

void Shape::m_zetaRadiusInit()
{
    double radius(0);
    double denom(m_zeta.size());

    for(int i(0); i< denom; i++){
        radius += abs(/*m_zetaCentroid*/ - m_zeta[i]);
    }

    radius /= denom;

    m_zetaRadius = radius;
}

vector<complex<double> > Shape::rotation(const vector<complex<double> > &variables, const int &alignVertexIndex)
{
    vector<complex<double> > zetas;
    double angle;
    complex<double> rot;

    //rotate shape so that centroid and vertex are aligned horizontally
    angle = arg(/*m_zetaCentroid*/ - m_zetaVertices[alignVertexIndex]);
    rot = exp(complex<double>(0.,- angle));

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] * rot);
    }

    //save for inverse
    m_angle = angle;

    return zetas;
}

vector<complex<double> > Shape::center(const vector<complex<double> > &variables)
{
    vector<complex<double> > zetas;

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] - m_zetaCentroid);
    }

    return zetas;
}

double Shape::innerAngle(double prevArg, const double &nextArg)
{
//    ensures that the inner angle is positive

    while(prevArg < nextArg){
        prevArg += 2 * M_PI;
    }

    return prevArg - nextArg;

}

vector<complex<double> > Shape::invMap(const vector<complex<double> > &variables, const double &alpha, const complex<double> &zt, const complex<double> &zc)
{
    vector<complex<double> > zeds;

    for(unsigned int i(0); i<variables.size(); i++){
        complex<double> zeta = variables[i];

        complex<double> c = (abs(zeta-1.)/abs(1.+zeta)) * exp(complex<double> (0, arg(zeta-1.) - arg(1.+zeta))) /*(zeta-1.) / (1.+zeta)*/;

        complex<double> a = pow(abs(c), alpha) * exp(complex<double> (0, arg(c) * alpha))/*pow((zeta-1.) / (1.+zeta),
                                alpha)*/;
        complex<double> axzc = (abs(a)*abs(zc)) * exp(complex<double> (0, arg(a) + arg(zc))) /*a*zc*/;

        zeds.push_back((abs(axzc - zt)/abs(a-1.)) * exp(complex<double> (0, arg(axzc - zt) - arg(a-1.))) /*(a*zc - zt) / (a-1.)*/);
    }
    return zeds;
}

vector<complex<double> > Shape::invMap(const vector<complex<double> > &variables, const complex<double> &zc)
{
    vector<complex<double> > zeds;

    for(unsigned int i(0); i<variables.size(); i++){
        complex<double> zeta = variables[i];

        complex<double> c = (abs(zeta-1.)/abs(1.+zeta)) * exp(complex<double> (0, arg(zeta-1.) - arg(1.+zeta))) /*(zeta-1.) / (1.+zeta)*/;


        complex<double> a = pow(abs(c), m_alphas[m_vertexIndex]) * exp(complex<double> (0, arg(c) * m_alphas[m_vertexIndex]))/*pow(c, m_alphas[m_vertexIndex])*/;

        complex<double> axzc = (abs(a)*abs(zc)) * exp(complex<double> (0, arg(a) + arg(zc))) /*a*zc*/;

        zeds.push_back( (abs(axzc - m_zVertices[m_vertexIndex])/abs(a-1.)) * exp(complex<double> (0, arg(axzc - m_zVertices[m_vertexIndex]) - arg(a-1.))) /*(a*zc - m_zVertices[m_vertexIndex]) / (a-1.)*/);
    }
    return zeds;
}

vector<complex<double> > Shape::invRotation(const vector<complex<double> > &variables)
{
    vector<complex<double> > zetas;
    complex<double> rot;

    rot = exp(complex<double>(0., m_angle));

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] * rot);
    }

    return zetas;
}

vector<complex<double> > Shape::invRotation(const vector<complex<double> > &variables, const double &angle)
{
    vector<complex<double> > zetas;
    complex<double> rot;

    rot = exp(complex<double>(0., angle));

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] * rot);
    }

    return zetas;
}

vector<complex<double> > Shape::invCenter(const vector<complex<double> > &variables)
{
    vector<complex<double> > zetas;

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] + m_zetaCentroid);
    }

    return zetas;
}

vector<complex<double> > Shape::invCenter(const vector<complex<double> > &variables, const complex<double> &old_zcentroid)
{
    vector<complex<double> > zetas;

    for(unsigned int i(0); i < variables.size(); i++){
        zetas.push_back(variables[i] + old_zcentroid);
    }

    return zetas;
}
