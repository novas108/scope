#include "mainwindow.h"
#define BUFFER_LEN 768

#include "lib/qcustomplot.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    //NULLING && init
    serialPort = NULL;
    protocol = new Protocol(1024);
    protocol->setDevice(NULL);

    //Window-init
    this->setMinimumSize(640,480);
    setWindowTitle(QString("scopeIA"));

    //menuBar
    fileMenu = menuBar()->addMenu("&Command");
    fileMenu->addAction(connA = new QAction(tr("&Connect"), this));
    QObject::connect(connA, SIGNAL(triggered()), this, SLOT(connDialog()));
    fileMenu->addAction(nextA = new QAction(tr("&Next config"), this));
    QObject::connect(nextA, SIGNAL(triggered()), this, SLOT(nextCfg()));
    fileMenu->addAction(exitA = new QAction(tr("E&xit"), this));
    QObject::connect(exitA, SIGNAL(triggered()), this, SLOT(quit()));

    //tabs
    this->setCentralWidget(tabW = new QTabWidget);
    tabW->setTabPosition(QTabWidget::South);//
    tabW->addTab(scopeW = new ScopeWidget(protocol),QString("Scope"));
    tabW->addTab(counterW = new counter(protocol),QString("Counter"));
    int toDisable = tabW->addTab(new QCustomPlot(),QString("Scope 2"));
    tabW->setTabEnabled(toDisable,false);

    //statusbar
    statusBar()->addWidget(new QLabel("COM:"),0);
    statusBar()->addWidget(comSB = new QLabel("_"),2);
    statusBar()->addWidget(new QLabel("Target:"),0);
    statusBar()->addWidget(targetSB = new QLabel("_"),2);
    statusBar()->addWidget(new QLabel("Voltage:"),0);
    statusBar()->addWidget(voltageSB = new QLabel("_"),2);
    statusBar()->addWidget(new QLabel("Configuration:"),0);
    statusBar()->addWidget(confSB = new QLabel("_"),2);
    statusBar()->addWidget(new QLabel("Status:"),0);
    statusBar()->addWidget(statusSB = new QLabel("_"),2);

    QObject::connect (protocol->getDeviceDescription(), SIGNAL(valuesChanged()), this, SLOT(changeTargetName()));
    QObject::connect (protocol, SIGNAL(supplyVoltageValue(double)), this, SLOT(setDestMax(double)));
}


MainWindow::~MainWindow()
{
}

void MainWindow::quit(){
    QApplication::quit();
}


void MainWindow::connDialog(){
    bool ok;
    QInputDialog *cDiag;
    QStringList *comPortList = new QStringList();

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(int i = 0;i < ports.size();++i) comPortList->append(ports[i].portName());
    cDiag = new QInputDialog(this);
    cDiag->setOptions(QInputDialog::UseListViewForComboBoxItems);
    QString port = cDiag->getItem(this,"Connection Settings", "Com ports:",*comPortList,0,false,&ok);
    if(ok){
        if(serialPort!=NULL){
            //serialPort->close();
            delete serialPort;
        }
        serialPort = new QSerialPort(port);

        if(serialPort->open(QIODevice::ReadWrite)){//zero-elab compatability
            serialPort->setBaudRate(115200);
            serialPort->setParity(QSerialPort::NoParity);
            serialPort->setStopBits(QSerialPort::OneStop);
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
            serialPort->setDataBits(QSerialPort::Data8);
            serialPort->setTextModeEnabled(false);

            protocol->setDevice(serialPort);
            QObject::connect (serialPort, SIGNAL(readyRead()), this,SLOT(readData()));//this, SLOT(readData()
            QMessageBox msgBox;
            msgBox.setText("Connected to: "+port);
            msgBox.exec();
            comSB->setText(serialPort->portName());
        }
        else {
            QString error = serialPort->errorString();
            QMessageBox msgBox;
            msgBox.setText("Unable to connect:\n"+error);
            msgBox.exec();
        }
    }
    delete cDiag;
    delete comPortList;
}

void MainWindow::readData() {protocol->processData();}

void MainWindow::nextCfg(){
    protocol->nextDeviceConfiguration();
}

void MainWindow::changeTargetName(){
    DeviceDescription* descr = protocol->getDeviceDescription();
    targetSB->setText(descr->name);
    confSB->setText(descr->configurationName);
    /*handle    descr->capabilities
    for(int i = 0;i < fw_vector.size(); ++i){
        bool enabled = protocol->getDeviceDescription()
                           ->hasCapability((DeviceDescription::Capabilities)functionIds[i]);
        functionWidgetButtons->button(i+1)->setEnabled(enabled);
    }*/
}

void MainWindow::setDestMax(double vcc){
    voltageSB->setText(QString::number(vcc)+" V");
    scopeW->voltage=(float)vcc;
}
