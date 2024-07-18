#include "scopewidget.h"
#define BUFFER_LEN 768
#define CHANNEL 1

ScopeWidget::ScopeWidget(Protocol* prot):QWidget(NULL){
    //plot etc.
    protocol=prot;
    setLayout(layout = new QHBoxLayout());
    layout->addWidget(plot = new QCustomPlot(),5,0);
    plot->addGraph();//TODO autoscale
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->addGraph();
    plot->graph(1)->setPen(QPen(Qt::green));
    plot->addGraph();
    plot->graph(2)->setPen(QPen(Qt::red));
    plot->addGraph();
    plot->graph(3)->setPen(QPen(Qt::yellow));

    plot->xAxis->setVisible(true);
    plot->xAxis->setLabel("Time [s]");
    plot->yAxis->setVisible(true);
    plot->yAxis->setLabel("Voltage [V]");
    plot->yAxis->setRange(-0.2,3.5);
    plot->xAxis->setRange(0,BUFFER_LEN/(activeCh*freq));

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->setMinimumSize(600,400);
    QObject::connect(protocol,SIGNAL(binaryReceived()),this,SLOT(plotData()));

    //dock-widgets, widget code
    layout->addWidget(controlPanel = new QWidget,0,Qt::AlignCenter);
    controlPanel->setLayout(controlPanelL = new QVBoxLayout());


    //RUN button
    controlPanelL->addWidget(run = new QPushButton("Run"),0);
    run->setCheckable(true);
    QObject::connect(run, SIGNAL(toggled(bool)), this, SLOT(runButt(bool)));

    controlPanelL->addWidget(freqlabel = new QLabel("Freq: "));
    controlPanelL->addWidget(freqLine = new QLineEdit(QString::number(this->freq,'f',2)));
    controlPanelL->addWidget(freqSlider = new QSlider(Qt::Horizontal));
    freqSlider->setMaximum(30000);
    freqSlider->setMinimumWidth(10);
    freqSlider->setMinimum(1);
    QObject::connect(freqSlider, SIGNAL(valueChanged(int)), this, SLOT(setFreq(int)));
    QObject::connect(freqLine, SIGNAL(editingFinished()), this, SLOT(setFreq()));
    controlPanelL->addWidget(triglabel = new QLabel("Trigger: "));
    controlPanelL->addWidget(trigLine = new QLineEdit(QString::number(trigger,'f',3)));
    controlPanelL->addWidget(trigSlider = new QSlider(Qt::Horizontal));
    trigSlider->setMaximum(3300);
    trigSlider->setMinimum(0);
    QObject::connect(trigSlider, SIGNAL(valueChanged(int)), this, SLOT(setTrigg(int)));
    QObject::connect(trigLine, SIGNAL(editingFinished()), this, SLOT(setTrigg()));
    setFreq(); setTrigg();
    controlPanelL->addWidget(risingEdgeButton = new QPushButton("Rising"));
    risingEdgeButton->setCheckable(true);
    risingEdgeButton->setChecked(true);
    controlPanelL->addWidget(fallingEdgeButton = new QPushButton("Falling"));
    fallingEdgeButton->setCheckable(true);
    controlPanelL->addWidget(autoButton = new QPushButton("Auto"));
    autoButton->setCheckable(true);
    QObject::connect(autoButton, SIGNAL(toggled(bool)), this, SLOT(autoButt(bool)));
    QObject::connect(risingEdgeButton, SIGNAL(toggled(bool)), this, SLOT(reButt(bool)));
    QObject::connect(fallingEdgeButton, SIGNAL(toggled(bool)), this, SLOT(feButt(bool)));

    controlPanelL->addWidget(channels = new QWidget);
    channels->setLayout(channelsL = new QGridLayout);
    channelsL->addWidget(channel1 = new QCheckBox("CH 1"),0,0);
    channelsL->addWidget(channel2 = new QCheckBox("CH 2"),1,0);
    channelsL->addWidget(channel3 = new QCheckBox("CH 3"),2,0);
    channelsL->addWidget(triggerRadio1 = new QRadioButton("Trigger"),0,1);
    channelsL->addWidget(triggerRadio2 = new QRadioButton("Trigger"),1,1);
    channelsL->addWidget(triggerRadio3 = new QRadioButton("Trigger"),2,1);
    triggerRadio1->setChecked(true);
    channel1->setChecked(true);
    QObject::connect(channel1, SIGNAL(toggled(bool)),this,SLOT(channelsUpdate()));
    QObject::connect(channel2, SIGNAL(toggled(bool)),this,SLOT(channelsUpdate()));
    QObject::connect(channel3, SIGNAL(toggled(bool)),this,SLOT(channelsUpdate()));
    QObject::connect(triggerRadio1, SIGNAL(clicked(bool)),this,SLOT(triggerUpdate()));
    QObject::connect(triggerRadio2, SIGNAL(clicked(bool)),this,SLOT(triggerUpdate()));
    QObject::connect(triggerRadio3, SIGNAL(clicked(bool)),this,SLOT(triggerUpdate()));
    controlPanelL->addWidget(resetZ = new QPushButton("Reset Zoom"));
    QObject::connect(resetZ, SIGNAL(clicked(bool)), this, SLOT(resetZoom()));
}

void ScopeWidget::setFreq(int value){
    freq=exp10(value/5000.);
    freqLine->setText(QString::number(freq,'f',2));
    protocol->command('F',CHANNEL,(uint32_t)freq );
    plot->xAxis->setRange(0,BUFFER_LEN/(activeCh*freq));
}

void ScopeWidget::setFreq(){
    freq = (freqLine->text()).toDouble();
    freqSlider->setValue((int)5000*log10(freq));
    freqLine->setText(QString::number(freq,'f',2));
    protocol->command('F',CHANNEL,(uint32_t)freq );
    plot->xAxis->setRange(0,BUFFER_LEN/(activeCh*freq));
}

void ScopeWidget::setTrigg(int value){
    trigger=value/1000.;
    trigLine->setText(QString::number(trigger,'f',2));
    protocol->command('T',CHANNEL,(uint32_t)value);
}

void ScopeWidget::setTrigg(){
    trigger = (trigLine->text()).toDouble();
    trigSlider->setValue(1000*trigger);
    trigLine->setText(QString::number(trigger,'f',2));
    protocol->command('T',CHANNEL,(uint32_t)1000*trigger);
}

void ScopeWidget::runButt(bool value){
    //if(serialPort!=NULL){//TODO something else
        if(value) protocol->command('S',CHANNEL,1);
        else protocol->command('S',CHANNEL,0);
    /*}
    else{
        QMessageBox msgBox;
        msgBox.setText("No device connected");
        msgBox.exec();
    }*/
}

void ScopeWidget::autoButt(bool value){
    //if(serialPort!=NULL){//TODO something else
        if(value){
            protocol->command('M',CHANNEL,1);
            fallingEdgeButton->setChecked(0);
            risingEdgeButton->setChecked(0);
        }
        else fallingEdgeButton->setChecked(1);
    /*}
    else{
        QMessageBox msgBox;
        msgBox.setText("No device connected");
        msgBox.exec();
    }*/
}

void ScopeWidget::reButt(bool value){
    //if(serialPort!=NULL){//TODO something else
        if(value){
            protocol->command('M',CHANNEL,0);
            protocol->command('P',CHANNEL,1);
            fallingEdgeButton->setChecked(0);
            autoButton->setChecked(0);
        }
    /*}
    else{
        QMessageBox msgBox;
        msgBox.setText("No device connected");
        msgBox.exec();
    }*/
}
void ScopeWidget::feButt(bool value){
    //if(serialPort!=NULL){//TODO something else
        if(value){
            protocol->command('M',CHANNEL,0);
            protocol->command('P',CHANNEL,2);
            autoButton->setChecked(0);
            risingEdgeButton->setChecked(0);
        }/*}
    else{
        QMessageBox msgBox;
        msgBox.setText("No device connected");
        msgBox.exec();
    }*/
}


void ScopeWidget::channelsUpdate(){
    uint32_t out = 0;
    if(channel1->isChecked()) out|=1<<0;
    if(channel2->isChecked()) out|=1<<1;
    if(channel3->isChecked()) out|=1<<2;
    activeCh=0;
    for (char c=0;c<3;++c) if((out>>c)&1) ++activeCh;
    plot->xAxis->setRange(0,BUFFER_LEN/(activeCh*freq));//rescale plot
    protocol->command('C',CHANNEL,out);
}

void ScopeWidget::triggerUpdate(){
    if(triggerRadio1->isChecked()) protocol->command('R',CHANNEL,0);
    if(triggerRadio2->isChecked()) protocol->command('R',CHANNEL,1);
    if(triggerRadio3->isChecked()) protocol->command('R',CHANNEL,2);
}
void ScopeWidget::plotData(){
    BinaryTransfer* transfer = protocol->popTransfer(1);
    if(transfer != NULL){
        unsigned char *data = (unsigned char *) (transfer->getData());
        unsigned char Ch = (data[1])>>4;//channels first buffer
        int len = (transfer->getSize()-data[0]-1)/Ch/2;
        data+=data[0];//number of spec
        data+=1;//skip number of spec itself

        QVector<double> x(len), plotDat(len), y(len), z(len),l(len/2);
        for(int j=0; j<len;++j){
            if(j<len/2) l[j]=(double)j*this->freq/len;
            x[j]=(double)j/this->freq;
            plotDat[j] = (double)(data[Ch*2*j]+(data[Ch*2*j+1]<<8))*voltage/4095.0;
            if(Ch>1) y[j]=(double)(data[Ch*2*j+2]+(data[Ch*2*j+3]<<8))*voltage/4095.0;
            if(Ch>2) z[j]=(double)(data[Ch*2*j+4]+(data[Ch*2*j+5]<<8))*voltage/4095.0;
        }
        plot->graph(0)->setData(x,plotDat);
        if(Ch>1) plot->graph(1)->setData(x,y);
        else plot->graph(1)->clearData();
        if(Ch>2) plot->graph(2)->setData(x,z);
        else plot->graph(2)->clearData();
        plot->replot();
    }
}

void ScopeWidget::resetZoom(){
    protocol->command('D',CHANNEL,0);//zero pretirgger
    plot->yAxis->setRange(-0.2,voltage);
    plot->xAxis->setRange(0,BUFFER_LEN/(activeCh*freq));
}
