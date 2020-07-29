#include "nombredepoints.h"
#include "ui_nombredepoints.h"


NombreDePoints::NombreDePoints(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NombreDePoints)
{
    ui->setupUi(this);

    m_xCoords = vector<double>(0);
    m_yCoords = vector<double>(0);
    m_order = vector<double>(0);

    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(formWindow()));
    QObject::connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(loadShape()));
}

NombreDePoints::~NombreDePoints()
{
    delete ui;
}

void NombreDePoints::formWindow()
{

    Form *f = new Form(ui->nombreDePointsSpinBox->value());
    f->show();
}

void NombreDePoints::loadShape()
{
    m_xCoords = vector<double>(0);
    m_yCoords = vector<double>(0);
    m_order = vector<double>(0);

//    A QT window allows to select file to load
    QString fileName = QFileDialog::getOpenFileName(this);

    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream in(&file);

        double x, y , order;

//        read file
        while (!in.atEnd()) {

            in >> x;
            in >> y;
            in >> order;

            m_xCoords.push_back(x);
            m_yCoords.push_back(y);
            m_order.push_back(order);
        }
    }

//    create loaded form
    Form *f = new Form(m_xCoords, m_yCoords, m_order);
    f->show();
}
