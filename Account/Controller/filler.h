#ifndef FILLER_H
#define FILLER_H

#include <QThread>
#include "Model/entry.h"

template<class Key, class Model>
class Filler: public QThread
{
protected:
    void run()
    {
        qDebug()<<"Begin"<<entries.size()<<model->size();
        for(auto it: entries)
            for(auto it2: *model)
                it2<<it;
        
        qDebug()<<"End";
    }
    
public:
    QMap<Key, Model>* model;
    QList<Entry> entries;
};

#endif // FILLER_H
