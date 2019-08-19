#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include "abstractcontroller.h"
#include "Model/frequency.h"
#include "Model/entry.h"

class ControllerFrequency: public AbstractController
{
private:
    QMap<int, Frequency> m_freqs;
    
public:
    ControllerFrequency();
    
    void addEntry(int);
    
    int exec();

};

#endif // CONTROLLERFREQUENCY_H
