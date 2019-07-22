#include "controllerfrequency.h"

ControllerFrequency::ControllerFrequency()
{
    
}

void ControllerFrequency::addEntry(int e)
{    
    if(m_freqs.contains(entry(e).frequency()))
        m_freqs[entry(e).frequency()]<<entry(e);
}

int ControllerFrequency::exec()
{
    return 0;
}
