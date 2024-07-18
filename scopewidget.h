#ifndef SCOPEWIDGET_H
#define SCOPEWIDGET_H

#include <QWidget>

#include "lib/qcustomplot.h"
#include "lib/Protocol.h"


class ScopeWidget : public QWidget
{
    Q_OBJECT
private:
    QCustomPlot* plot;

    Protocol* protocol;

    QWidget *controlPanel;
    QHBoxLayout* layout;
    QVBoxLayout* controlPanelL;
    QWidget *pwmPanel;

    QPushButton *run;
    QLabel *freqlabel;
    QLabel *triglabel;
    QSlider *freqSlider;
    QSlider *trigSlider;
    QLineEdit *freqLine;
    QLineEdit *trigLine;

    QPushButton *risingEdgeButton;
    QPushButton *fallingEdgeButton;
    QPushButton *autoButton;

    QWidget *channels;
    QGridLayout *channelsL;
    QCheckBox *channel1;
    QCheckBox *channel2;
    QCheckBox *channel3;
    QRadioButton *triggerRadio1;
    QRadioButton *triggerRadio2;
    QRadioButton *triggerRadio3;

    QPushButton *resetZ;

    double freq = 10000;
    double trigger = 1.5;
    char activeCh = 1;
private slots:
    void plotData();
    void resetZoom();
    void setFreq();
    void setFreq(int value);
    void setTrigg();
    void setTrigg(int value);

    void runButt(bool value);//comm+gui
    void autoButt(bool value);
    void reButt(bool value);
    void feButt(bool value);
    void channelsUpdate();
    void triggerUpdate();

public:
    explicit ScopeWidget(Protocol* prot);
    float voltage;
signals:

};

#endif // SCOPEWIDGET_H
