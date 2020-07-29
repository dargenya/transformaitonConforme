#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(vector<double> xCoordinates, vector<double> yCoordinates, vector<int> order, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_order = order;

    m_vertices = int(m_order.size());

    m_zcs = vector<complex<double>>(m_vertices);
    m_zetaCentroids = vector<complex<double>>(m_vertices);
    m_angles = vector<double>(m_vertices);

    m_shapes = vector<Shape>(0);
    m_drawAreas = vector<DrawArea*>(0);

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QGridLayout *mainGrid = new QGridLayout(this);
    mainWidget->setLayout(mainGrid);


//    first shape is created
    m_shapes.push_back(Shape(xCoordinates, yCoordinates));
    vector<double> alphas = m_shapes[0].m_alphas;

    m_drawAreas.push_back(new DrawArea(this, 0, 0));
    QObject::connect(m_drawAreas[0], SIGNAL(needsTransform(const complex<double>&, int))
            , this, SLOT(callTransform(const complex<double>&, int)));
    mainGrid->addWidget(m_drawAreas[0], 0, 0);


//following shapes are also created
    for(int i(1); i<m_vertices; i++){

        int j = m_order[i];

        m_shapes.push_back(Shape(j, alphas));

        m_drawAreas.push_back(new DrawArea(this, j, i));
        QObject::connect(m_drawAreas[i], SIGNAL(needsTransform(const complex<double>&, int))
                , this, SLOT(callTransform(const complex<double>&, int)));

        mainGrid->addWidget(m_drawAreas[i], i/4, i%4);
    }

    //last draw area has no shape or connection
    int last = m_vertices;
    m_drawAreas.push_back(new DrawArea(this, 0, last));

    mainGrid->addWidget(m_drawAreas.back(), last/4, last%4);


    m_drawAreas[0]->receiveDraw(m_shapes[0].m_zVertices, m_shapes[0].m_z, m_shapes[0].m_zetaRadius, m_shapes[0].m_minimise);


}

MainWindow::~MainWindow()
{
    for(int i(0); i<m_vertices+1; i++){
        delete m_drawAreas[i];
    }

    delete ui;
}

void MainWindow::callTransform(const complex<double> &zc, int senderOrderIndex)
{
    //orders given according to vertex and NOT the order in which the widgets appear on screen

    //receives signal from DrawArea #senderOrderIndex
    //procedes to transform Shape #senderOrderIndex
    //updates the z and z_vertices of Shapes onwards in display order
    //then draws updated shapes
    //draw circle for last DrawArea


//    store old zc
    m_zcs[senderOrderIndex] = zc;

    m_shapes[senderOrderIndex].m_zc = zc;
    if(senderOrderIndex+1<m_vertices){
        m_shapes[senderOrderIndex].transform(m_order[senderOrderIndex + 1]);
    } else{
//        if final transformation, then plot
        m_shapes[senderOrderIndex].transform(0);
        m_shapes.back().plot();
    }
    m_drawAreas[senderOrderIndex]->receiveDrawZc(zc);

//    store old centroids
    m_zetaCentroids[senderOrderIndex] = m_shapes[senderOrderIndex].m_zetaCentroid;
//    store old angles
    m_angles[senderOrderIndex] = m_shapes[senderOrderIndex].m_angle;


    //do and draw inverse of a single transformation
    vector<complex<double>> inv_mesh;

    inv_mesh = m_shapes[senderOrderIndex].invTransform();

    m_drawAreas[senderOrderIndex]->receiveInverseDraw(inv_mesh);
    //end inverse block

    for(int i(senderOrderIndex); i<m_vertices; i++){
//trickle-down update of shapes and draw areas
        vector<complex<double>> vertices, mesh;
//        complex<double> center;
        double radius, minimise;


        vertices = m_shapes[i].m_zetaVertices;
        mesh = m_shapes[i].m_zeta;
        m_shapes[i].m_zetaRadiusInit();
        m_shapes[i].m_minimiseInit();
        radius = m_shapes[i].m_zetaRadius;
        minimise = m_shapes[i].m_minimise;


        m_drawAreas[i+1]->receiveDraw(vertices, mesh, radius, minimise);

        if(i+1<m_vertices){
            m_shapes[i+1].m_zVertices=vertices;
            m_shapes[i+1].m_z=mesh;

            m_shapes[i+1].transform(m_order[i+1]);
        }

    }

    m_drawAreas.back()->receiveDrawCircle();

//    do total inverse
    if(senderOrderIndex==m_vertices-1){
        vector<complex<double>> totalInvMesh;

//        get last zeta mesh
        totalInvMesh = m_shapes.back().m_zeta;


        for(int i(m_vertices-1); i > -1; i--){
//            inverse of rotation, center, map

            totalInvMesh = m_shapes[i].invRotation(totalInvMesh, m_angles[i]);
            totalInvMesh = m_shapes[i].invCenter(totalInvMesh, m_zetaCentroids[i]);
            totalInvMesh = m_shapes[i].invMap(totalInvMesh, m_zcs[i]);
        }

        m_drawAreas[0]->receiveInverseDraw(totalInvMesh);

    }
}


