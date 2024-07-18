#ifndef COUNTER_H
#define COUNTER_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>

#include "lib/Protocol.h"

class counter : public QWidget
{
    Q_OBJECT
private:
    Protocol* protocol;
    QGridLayout* layout;
    QPixmap image;
    QLabel* imageL;
    QPushButton* run;
    QComboBox* mode;
    QComboBox* aquisition;
    QRadioButton* HzR;
    QRadioButton* sR;
private slots:
    void runButt(bool value);
    void modeU(int value);
    void aqU();
    void unitsUpdate();
public:
    explicit counter(Protocol* prot);

signals:

};

#endif // COUNTER_H
