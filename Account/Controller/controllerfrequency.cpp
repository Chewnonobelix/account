#include "controllerfrequency.h"

ControllerFrequency::ControllerFrequency()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/Generate.qml")));
    m_generate = m_eng.rootObjects().first();
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/FrequencyManager.qml")));
    m_manager = m_eng.rootObjects().first();
    
    connect(m_generate, SLOT(s_generate(QDate, QDate)), this, SIGNAL(generate(QDate, QDate)));
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
    for(auto it: freqs)
        m_freqs[it.id()] = it;
    
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
    
    Entry ref = entry(m_freqs[freqId].referenceEntry());
    
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
