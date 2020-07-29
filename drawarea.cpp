#include "drawarea.h"


DrawArea::DrawArea(QWidget *parent, int vertexIndex, int orderIndex) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    m_scale = double(10.);

    m_vertices = QPolygonF(0);
    m_mesh = QPolygonF(0);

    m_zVertices = vector<complex<double>>(0);
    m_z = vector<complex<double>>(0);

    m_vertexIndex = vertexIndex;
    m_orderIndex = orderIndex;

    m_zc = complex<double>(0);
    m_clickPoint = QPointF(0,0);

//    m_zCentroid = complex<double>(0);
    m_radius = double(0);

    m_minimise = QString();

    transforming = bool(false);

    m_drawCircle = bool(false);
    m_zcircle = vector<complex<double>>(0);
    m_circle = QPolygonF(0);

    //inverse
    m_zinvMesh = vector<complex<double>>(0);
    m_inverseMesh = QPolygonF(0);
}

//int DrawArea::getm_vertexIndex()
//{
//    return m_vertexIndex;
//}

void DrawArea::receiveDraw(const vector<complex<double> > &z_vertices, const vector<complex<double> > &z_mesh, const double &radius, const double& minimise)
{
    m_zVertices=z_vertices;
    m_z = z_mesh;

    m_radius=radius;

    //setting scale based on average radius
    double minDim(0);
    minDim = min(this->frameSize().height(),this->frameSize().width());
    m_scale = 0.2 * minDim/m_radius;

    m_vertices = localToPaint(m_zVertices);
    m_mesh = localToPaint(m_z);

    m_minimise = QString::number(minimise);

    update();
}

void DrawArea::receiveDrawZc(const complex<double> &zc)
{
    m_zc = zc;
    m_clickPoint = localToPaint(m_zc);

    update();
}

void DrawArea::receiveDrawCircle()
{
    m_drawCircle = true;

    m_zcircle = vector<complex<double>>(0);

    for(double theta(-M_PI); theta<M_PI; theta += 0.01){
        m_zcircle.push_back( complex<double> (m_radius * cos(theta), m_radius * sin(theta)) );
    }

    m_circle = localToPaint(m_zcircle);

    update();
}

//inverse
void DrawArea::receiveInverseDraw(const vector<complex<double> > &z_invMesh)
{
    m_zinvMesh = z_invMesh;
    m_inverseMesh = localToPaint(m_zinvMesh);

    update();
}

void DrawArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QFont font = painter.font();
    font.setPixelSize(15);
    painter.setFont(font);

    //make pen to draw click point
    QPen clickPen;
    clickPen.setWidth(4);
    clickPen.setColor(Qt::green);

    //make pen to draw zvertice
    QPen yellowPen;
    yellowPen.setWidth(4);
    yellowPen.setColor(Qt::yellow);

    //make pen to nodes
    QPen nodePen;
    nodePen.setWidth(3);
    nodePen.setColor(Qt::blue);

    //make pen to draw mesh
    QPen meshPen;
    meshPen.setWidth(2);
    meshPen.setColor(Qt::red);

    //make pen for text
    QPen textPen;
    textPen.setWidth(4);
    textPen.setColor((Qt::black));

    //make pen to draw inverse mesh
    QPen invPen;
    invPen.setWidth(2);
    invPen.setColor(Qt::gray);

    if(m_drawCircle){

        painter.setPen(clickPen);
        painter.drawPolygon(m_circle);

//        QPointF topLeft = localToPaint(/*m_zCentroid +*/ complex<double>(-m_radius, m_radius));

//        QRectF rectangle(topLeft.x(), topLeft.y(), 2*m_radius*m_scale, 2*m_radius*m_scale);

//        painter.setPen(clickPen);
//        painter.drawEllipse(rectangle);

    }

    //draw inverse mesh
    painter.setPen(invPen);
    painter.drawPolygon(m_inverseMesh);

    painter.setPen(meshPen);
    //draw mesh
    painter.drawPolygon(m_mesh);

    painter.setPen(nodePen);
    //draw nodes
    painter.drawPoints(m_mesh);

    painter.setPen(clickPen);
    //draw click point
    painter.drawPoint(m_clickPoint);

    if(m_vertexIndex < m_vertices.size()){
        painter.setPen(yellowPen);
        //draw vertice
        QPointF aPoint = m_vertices.at(m_vertexIndex);
        painter.drawPoint(aPoint);
    }


    painter.setPen(textPen);
    //draw minimise text
    painter.drawText(1,15, m_minimise);

}

QPolygonF DrawArea::localToPaint(const vector<complex<double> > &zLocals)
{
    QPolygonF poly;
    for (unsigned int i(0); i<zLocals.size(); ++i ){

        double x,y;
        x = zLocals[i].real();
        y = zLocals[i].imag();

        x *= m_scale;
        y *= - m_scale;

        x += this->width()*3/4;
        y += this->height()/2;

        poly << QPointF(x,y);
    }

    return poly;
}

QPointF DrawArea::localToPaint(const complex<double> &z)
{
    double x,y;
    x = z.real();
    y = z.imag();

    x *= m_scale;
    y *= - m_scale;

    x += this->width()*3/4;
    y += this->height()/2;

    return QPointF(x,y);
}

complex<double> DrawArea::paintToLocal(const QPointF &paint_point)
{
    double x = paint_point.x();
    double y = paint_point.y();

    x -= this->width()*3/4;
    y -= this->height()/2;

    x /= m_scale;
    y /= - m_scale;


    return  complex<double>(x, y);
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        this->leftClick(event);

        transforming = true;
    }

    //Pass on event to parent
    QWidget::mousePressEvent(event);
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && transforming){
        this->leftClick(event);
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && transforming) {
        this->leftClick(event);
        transforming = false;
    }

}

void DrawArea::wheelEvent(QWheelEvent *event)
{
    double numPixels = event->pixelDelta().y();
    double numDegrees = event->angleDelta().y() / 8;

    if (numPixels != 0) {
        if (numPixels < 0){m_scale *= abs(numPixels) / 1.2;}
        else {m_scale *= 1.2 * numPixels;}
    } else if (numDegrees != 0) {
        double numSteps = numDegrees / 15;
        if (numSteps < 0){m_scale *= abs(numSteps) / 1.2;}
        else {m_scale *= 1.2 * numSteps;}
    }

    //do necessary transformations
    m_vertices = localToPaint(m_zVertices);
    m_mesh = localToPaint(m_z);

    m_clickPoint = localToPaint(m_zc);

    m_inverseMesh = localToPaint(m_zinvMesh);

    m_circle = localToPaint(m_zcircle);

    update();

    event->accept();

}

void DrawArea::leftClick(QMouseEvent *event)
{
    // handle Left mouse button here


    QPointF clickPoint;
    clickPoint = event->pos();

    //do the transformation
    emit needsTransform(paintToLocal(clickPoint), m_orderIndex);


}
