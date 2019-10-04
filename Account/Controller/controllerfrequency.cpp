#include "controllerfrequency.h"

ControllerFrequency::ControllerFrequency()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/Generate.qml")));
    m_generate = m_eng.rootObjects().first();
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/FrequencyManager.qml")));
    
    m_manager = m_eng.rootObjects().last();
    
    
    connect(m_generate, SLOT(s_generate(QDate, QDate)), this, SIGNAL(generate(QDate, QDate)));
    
    QObject* add, *remove;
    
    add = m_manager->findChild<QObject*>("addFreq");
    remove = m_manager->findChild<QObject*>("removeFreq");
    
    if(add)
        connect(add, SIGNAL(s_addFrequency()), this, SLOT(addFrequency()));
    if(remove)
        connect(remove, SIGNAL(s_removeFrequency(int)), this, SLOT(removeFrequency(int)));
    
    QObject* cat = m_manager->findChild<QObject*>("category");
    
    if(cat)
        connect(cat, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
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
    
    auto cat = m_db->selectCategory();
    QStringList income = cat.values("income");
    income<<"";
    QStringList outcome = cat.values("outcome");
    outcome<<"";

    auto ee = m_manager->findChild<QObject*>("ref");
    ee->setProperty("incomeList", income);
    ee->setProperty("outcomeList", outcome);

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
  
    m_model.clear();
    
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

void ControllerFrequency::addFrequency()
{
    Frequency f;
    m_db->addFrequency(f);
    openManager();
}

void ControllerFrequency::removeFrequency(int id)
{
    m_db->removeFrequency(m_freqs[id]);
    openManager();
}

void ControllerFrequency::addNewCategory(QString cat)
{
    QObject* ref = m_manager->findChild<QObject*>("ref");
    QString type = ref->property("entry").value<Entry>().type();

    m_db->addCategory(type, cat);
    exec();
}
