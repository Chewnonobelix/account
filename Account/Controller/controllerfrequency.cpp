#include "controllerfrequency.h"

ControllerFrequency::ControllerFrequency()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/Generate.qml")));
    m_generate = m_eng.rootObjects().first();
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/FrequencyManager.qml")));
    
    m_manager = m_eng.rootObjects().last();
    
    
    connect(m_generate, SLOT(s_generate(QDate, QDate)), this, SIGNAL(generate(QDate, QDate)));
//    QObject* model = m_manager->findChild<QObject*>("frequencyList");
//    model->setProperty("model", QVariant::fromValue(m_model));    
    
}

void ControllerFrequency::addEntry(int e)
{    
    if(m_freqs.contains(entry(e).frequency()))
        m_freqs[entry(e).frequency()]<<entry(e);
}

int ControllerFrequency::exec()
{
    auto freqs = m_db->selectFrequency();
    
    m_freqs.clear();
    Frequency t, t2;
    t.setId(3);
    Entry e;
    Information i;
    i.setCategory("Test freq");
    e.setInfo(i);
    t.setReferenceEntry(e);
    m_freqs[3] = t;
    
    t2.setId(4);
    i.setCategory("Test 2");
    e.setInfo(i);
    t2.setReferenceEntry(e);
    
    m_freqs[4] = t2;
    for(auto it: freqs)
        m_freqs[it.id()] = it;
    
    qDebug()<<"Freq size 2"<<m_freqs.size();
    return 0;
}

void ControllerFrequency::generate(QDate begin, QDate end)
{
    QDate it = begin;
    Account::FrequencyEnum freq = Account::FrequencyEnum::unique;
    int freqId, freqGroup = 0;
    
    freqId = m_generate->property("freqId").toInt();
    freqGroup = m_generate->property("freqGroup").toInt();
    
    m_freqs[freqId].setNbGroup(freqGroup);
    
    Entry ref = m_freqs[freqId].referenceEntry();
    
    do
    {
        auto n = m_freqs[freqId].clone(ref);
        n.setMetadata("freqGroup", freqGroup);
        it = n.date();
        AbstractController::addEntry(n);
        
    }
    while(freq != Account::FrequencyEnum::unique && it <= end);
    
    QMetaObject::invokeMethod(m_generate, "close");
    exec();
}

void ControllerFrequency::openGenerate(int id)
{
    m_generate->setProperty("freqId", id);
    m_generate->setProperty("freqGroup", m_freqs[id].nbGroup() + 1);
    
    QMetaObject::invokeMethod(m_generate, "open");
}

void ControllerFrequency::openManager()
{
    exec();
  
    m_model.clear();;
    
    for(auto it = m_freqs.begin(); it != m_freqs.end(); it++)
        m_model<<QVariant::fromValue(*it);

    QObject* model = m_manager->findChild<QObject*>("frequencyList");
    model->setProperty("model", m_model);    

    QMetaObject::invokeMethod(m_manager, "show");    
}

void ControllerFrequency::closeManager()
{
    QMetaObject::invokeMethod(m_manager, "close");
}
