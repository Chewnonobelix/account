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
        for(auto it: entries)
            for(auto& it2: *model)
                it2<<it;
    }
    
public:
    QMap<Key, Model>* model = nullptr;
    QList<Entry> entries;
};

#endif // FILLER_H
