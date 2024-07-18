#include "counter.h"
#define CHANNEL 6

counter::counter(Protocol* prot): QWidget(NULL)
{
    protocol=prot;
    setLayout(layout = new QGridLayout());

    image.load("fg.png");//TODO resources
    layout->addWidget(imageL = new QLabel(""),0,0,1,2, Qt::AlignCenter);
    imageL->setPixmap(image);

    layout->addWidget(new QLabel("Mode:"),1,0, Qt::AlignRight|Qt::AlignVCenter);
    layout->addWidget(mode=new QComboBox(),1,1, Qt::AlignLeft|Qt::AlignVCenter);
    mode->addItem("Freqency");
    mode->addItem("Period");

    layout->addWidget(new QLabel("Aquisition:"),2,0, Qt::AlignRight|Qt::AlignVCenter);
    layout->addWidget(aquisition=new QComboBox(),2,1, Qt::AlignLeft|Qt::AlignVCenter);
    modeU(0);

    QObject::connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(modeU(int)));
    QObject::connect(aquisition, SIGNAL(currentIndexChanged(int)), this, SLOT(aqU()));

    layout->addWidget(run = new QPushButton("Run"),3,0,1,2, Qt::AlignCenter);
    run->setCheckable(true);
    QObject::connect(run, SIGNAL(toggled(bool)), this, SLOT(runButt(bool)));


    layout->addWidget(HzR = new QRadioButton("Hz"),4,0, Qt::AlignCenter);
    layout->addWidget(sR = new QRadioButton("s"),4,1, Qt::AlignCenter);
    QObject::connect(HzR, SIGNAL(clicked(bool)),this,SLOT(unitsUpdate()));
    QObject::connect(sR, SIGNAL(clicked(bool)),this,SLOT(unitsUpdate()));
}

void counter::runButt(bool value){
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


void counter::modeU(int value){
    aquisition->clear();
    switch(value){
        case 0://freq
            aquisition->addItem("gatetime");
            aquisition->addItem("counting events");
            break;
        case 1://period
            aquisition->addItem("pwm in");
            aquisition->addItem("high pulse");
            aquisition->addItem("low pulse");
            break;
        default:
            break;
    }
}


void counter::aqU(){
    qDebug()<<"aqu\n";
    switch(mode->currentIndex()){
        case 0://freq
            switch(aquisition->currentIndex()){
                case 0://gatetime
                    image.load("fg.png");
                    break;
                case 1://events
                    image.load("fe.png");
                    break;
                default:
                    break;
            };break;
        case 1://perios
            switch(aquisition->currentIndex()){
                case 0://pwm
                    image.load("pp.png");
                    break;
                case 1://high
                    image.load("ph.png");
                    break;
                case 2://low
                    image.load("pl.png");
                    break;
                default:
                    break;
            };break;
        default:
            break;
    }
    imageL->setPixmap(image);
}

void counter::unitsUpdate(){

}
