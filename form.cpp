#include "form.h"
#include "ui_form.h"

Form::Form(int nombrePoints, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    m_xBoxes = vector<QDoubleSpinBox*>(0);
    m_yBoxes = vector<QDoubleSpinBox*>(0);
    m_orderBoxes = vector<QSpinBox*>(0);

    for(int i(0); i<nombrePoints; i++){
        m_xBoxes.push_back(new QDoubleSpinBox());
        m_xBoxes[i]->setMinimum(- m_xBoxes[i]->maximum());
        ui->gridLayout->addWidget(m_xBoxes[i], i + 2, 0);

        m_yBoxes.push_back(new QDoubleSpinBox());
        m_yBoxes[i]->setMinimum(- m_yBoxes[i]->maximum());
        ui->gridLayout->addWidget(m_yBoxes[i], i + 2, 1);

        m_orderBoxes.push_back(new QSpinBox());
        if(i==0){
            m_orderBoxes[i]->setValue(0);
            m_orderBoxes[i]->setRange(0, 0);
        }
        else{
            m_orderBoxes[i]->setRange(1, nombrePoints - 1);
        }
        ui->gridLayout->addWidget(m_orderBoxes[i], i + 2, 2);
    }


    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(mainWindow()));
    QObject::connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveFigure()));
}

Form::Form(vector<double> xCoords, vector<double> yCoords, vector<double> order, QWidget *parent) :
    Form::Form(order.size())
{   
//    Note that this constructor has called the 'default' constructor

    for(unsigned int i(0); i<order.size(); i++){
        m_xBoxes[i]->setValue(xCoords[i]);
        m_yBoxes[i]->setValue(yCoords[i]);
        m_orderBoxes[i]->setValue(order[i]);
    }
}

Form::~Form()
{
    delete ui;
}

void Form::saveFigure()
{
    QString fileName = QFileDialog::getSaveFileName(this);

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, "Unable to open file",
                file.errorString());
            return;
        }

        QTextStream out(&file);

        for(unsigned int i(0); i<m_orderBoxes.size(); i++){
            out << m_xBoxes[i]->value()<<" ";
            out << m_yBoxes[i]->value()<<" ";
            out << m_orderBoxes[i]->value();

            if(i<m_orderBoxes.size() - 1){
                out << endl;
            }
        }
    }
}

void Form::mainWindow()
{
    vector<int> order(0);
    vector<double> xCoordinates(0);
    vector<double> yCoordinates(0);

    for(unsigned int i(0); i<m_orderBoxes.size(); i++){
        xCoordinates.push_back(m_xBoxes[i]->value());
        yCoordinates.push_back(m_yBoxes[i]->value());
        order.push_back(m_orderBoxes[i]->value());
    }

    MainWindow *w = new MainWindow(xCoordinates, yCoordinates, order);
    w->show();
}
