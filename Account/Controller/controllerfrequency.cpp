#include "controllerfrequency.h"

void Worker::run()
{
    for(auto it = 0; it < list.size(); it++)
    {
        emit s_add(list[it]);
        int temp = (double)it/(list.size()-1) * 10000;
        m_progress = temp / 100.0;
        emit s_progressChanged(m_progress);
    }

    emit s_finish(name);
}

double Worker::progress() const
{
    return m_progress;
}

Worker& Worker::operator =(const Worker&)
{
    return *this;
}


ControllerFrequency::ControllerFrequency()
{
    
}

QSharedPointer<FeatureBuilder> ControllerFrequency::build(QQmlApplicationEngine * engine, QObject * root, QList<AbstractController *> contollers)
{    
    Q_UNUSED(contollers)
    Q_UNUSED(root)

    QQmlComponent frequencyComp(engine, QUrl("qrc:/Frequency/FrequencyManager.qml"));
    QObject* frequency = frequencyComp.create();
    
    auto freqs = QSharedPointer<ControllerFrequency>::create();
    
    connect(frequency, SIGNAL(s_open()), freqs.data(), SLOT(openManager()));
    

    freqs->setManager(frequency);

    freqs->exec();
    
    freqs->view = frequency;
    return freqs;
}

void ControllerFrequency::setManager(QObject * manager)
{
    m_manager = manager;
    
    QObject* add, *remove, *ref, *type;
    
    add = m_manager->findChild<QObject*>("addFreq");
    remove = m_manager->findChild<QObject*>("removeFreq");
    
    if(add)
        connect(add, SIGNAL(s_addFrequency()), this, SLOT(addFrequency()));
    if(remove)
        connect(remove, SIGNAL(s_removeFrequency(QVariant)), this, SLOT(removeFrequency(QVariant)));
    
    QObject* cat = m_manager->findChild<QObject*>("category");
    
    if(cat)
        connect(cat, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
    
    
    ref = m_manager->findChild<QObject*>("ref");
    
    if(ref)
    {
        connect(ref, SIGNAL(valueChanged(QVariant, double)), this, SLOT(updateFreqValue(QVariant,double)));
        connect(ref, SIGNAL(titleChanged(QVariant, QString)), this, SLOT(updateFreqName(QVariant,QString)));
        connect(ref, SIGNAL(catChanged(QVariant, QString)), this, SLOT(updateFreqCat(QVariant,QString)));
        connect(ref, SIGNAL(supportChanged(QVariant, int)), this, SLOT(updateFreqSupport(QVariant,int)));
    }
    
    QObject* when = m_manager->findChild<QObject*>("whenCombo");
    
    if(when)
        connect(when, SIGNAL(s_freq(QVariant, int)), this, SLOT(updateFreqFreq(QVariant, int)));
    
    type = m_manager->findChild<QObject*>("type");
    if(type)
        connect(type, SIGNAL(s_updateType(QVariant, QString)), this, SLOT(updateFreqType(QVariant,QString)));
    
    QObject* button = m_manager->findChild<QObject*>("generateOpen");

    if(button)
        connect(button, SIGNAL(s_open(QVariant)), this, SLOT(openGenerate(QVariant)));
 
    QObject* el = m_manager->findChild<QObject*>("entryList");
    
    if(el)
        connect(el, SIGNAL(s_display(QVariant)), this, SLOT(displayEntry(QVariant)));
    
    m_generate = m_manager->findChild<QObject*>("generate");
        
    connect(m_generate, SIGNAL(s_generate(QString, QString)), this, SLOT(generate(QString, QString)));
    
    QObject* freqList = m_manager->findChild<QObject*>("frequencyList");
    
    if(freqList)
        connect(freqList, SIGNAL(s_modelChanged(QString)), this, SLOT(setWorker(QString)));

    connect((QThread*)&m_filler, SIGNAL(finished()), this, SLOT(endFill()));

    QObject* freqEndless = m_manager->findChild<QObject*>("endless");
    if(freqEndless)
        connect(freqEndless, SIGNAL(s_endless(QVariant, bool)), this, SLOT(updateFreqEndless(QVariant,bool)));
    
    connect(m_db, &InterfaceDataSave::s_updateFrequency, this, &ControllerFrequency::exec);
}

void ControllerFrequency::setWorker(QString name)
{
    QObject* w = m_manager->findChild<QObject*>("worker");
    
    w->setProperty("worker", QVariant::fromValue(m_workers[name]));
}

void ControllerFrequency::endThread(QString)
{
    exec();

    int index = parent()->metaObject()->indexOfMethod("buildModel()");
    parent()->metaObject()->method(index).invoke(parent(), Qt::DirectConnection);

    index = parent()->metaObject()->indexOfMethod("pageChange()");
    parent()->metaObject()->method(index).invoke(parent(), Qt::DirectConnection);
}

void ControllerFrequency::loadCat()
{
    auto cat = m_db->selectCategory();
    QStringList income = cat.values("income");
    income<<"";
    QStringList outcome = cat.values("outcome");
    outcome<<"";
    
    auto ee = m_manager->findChild<QObject*>("ref");
    ee->setProperty("incomeList", income);
    ee->setProperty("outcomeList", outcome);    
}

void ControllerFrequency::endFill()
{
    m_model.clear();
        
    QObject* model = m_manager->findChild<QObject*>("frequencyList");
    QUuid id = model->property("currentModel").isNull() ? QUuid() : model->property("currentModel").value<Frequency>().id();
    int index = -1;
    for(auto it = m_freqs.begin(); it != m_freqs.end(); it++)
    {
        m_model<<QVariant::fromValue(*it);
        if(it->id() == id)
            index = m_model.count() - 1;
    }

    model->setProperty("model", m_model);
    model->setProperty("currentIndex", index);
}

int ControllerFrequency::exec()
{
    auto freqs = m_db->selectFrequency();
    
    m_freqs.clear();
    for(auto it: freqs)
    {
        m_freqs[it.id()] = it;
        if(it.endless() && ((it.end() < QDate::currentDate()) || it.nbGroup() == 0))
        {
            m_generate->setProperty("freqId", it.id());
            m_generate->setProperty("freqGroup", it.nbGroup()+1);
            auto date = it.end().addDays( Account::nbDay(it.end(), it.freq()));
            generate(date.toString("dd-MM-yyyy"), date.addYears(1).toString("dd-MM-yyyy"));
        }
    }

    if(!m_filler.model)
        m_filler.model = &m_freqs;

    m_filler.entries = m_db->selectEntry(currentAccount()).values();

    m_filler.start();

    loadCat();
    
    return 0;
}

void ControllerFrequency::generate(QString begin, QString end)
{
    QDate it = QDate::fromString(begin, "dd-MM-yyyy");
    Account::FrequencyEnum freq = Account::FrequencyEnum::Unique;
    int freqGroup = 0;
    QUuid freqId;
    
    freqId = m_generate->property("freqId").toUuid();
    freqGroup = m_generate->property("freqGroup").toInt();
    freq = m_freqs[freqId].freq();
    m_freqs[freqId].setNbGroup(freqGroup);
    m_db->updateFrequency(m_freqs[freqId]);
    
    Entry ref = m_freqs[freqId].referenceEntry();
    QList<Entry> lr;

    do
    {
        ref.setDate(it);
        auto n = m_freqs[freqId].clone(ref);
        n.setMetadata("freqGroup", freqGroup);
        int t = Account::nbDay(it, m_freqs[freqId].freq());
        
        QString lab = n.info().title();
        Information inf = n.info();
        inf.setEstimated(it > QDate::currentDate());
        inf.setTitle(lab+"_"+it.toString("dd-MM-yyyy"));
        n.setInfo(inf);
        it = n.date().addDays(t);
        
        lr<<n;
    }
    while(freq != Account::FrequencyEnum::Unique && it <= QDate::fromString(end, "dd-MM-yyyy"));
    
    if(!m_workers[m_freqs[freqId].name()])
    {
        m_workers[m_freqs[freqId].name()]= new Worker;
        connect(m_workers[m_freqs[freqId].name()], &Worker::s_add, m_db, &InterfaceDataSave::addEntry, Qt::DirectConnection);
        connect(m_workers[m_freqs[freqId].name()], &Worker::s_finish, this, &ControllerFrequency::endThread);
        m_workers[m_freqs[freqId].name()]->name = m_freqs[freqId].name();
    }
    
    m_workers[m_freqs[freqId].name()]->list = lr;
    setWorker(m_freqs[freqId].name());

    m_workers[m_freqs[freqId].name()]->start();
    QMetaObject::invokeMethod(m_generate, "close");
}


void ControllerFrequency::openGenerate(QVariant id)
{
    m_generate->setProperty("freqId", id);
    m_generate->setProperty("freqGroup", m_freqs[id.toUuid()].nbGroup() + 1);
    
    QMetaObject::invokeMethod(m_generate, "open");
}

void ControllerFrequency::openManager()
{
    exec();
}

void ControllerFrequency::closeManager()
{
    QMetaObject::invokeMethod(m_generate, "close");
}

void ControllerFrequency::addFrequency()
{
    Frequency f;
    m_db->addFrequency(f);
    openManager();
}

void ControllerFrequency::removeFrequency(QVariant id)
{
    m_db->removeFrequency(m_freqs[id.toUuid()]);
    openManager();
}

void ControllerFrequency::addNewCategory(QString cat)
{
    QObject* ref = m_manager->findChild<QObject*>("ref");
    QString type = ref->property("entry").value<Entry>().type();
    
    m_db->addCategory(cat, type);
    loadCat();
    QUuid id = ref->property("entry").value<Entry>().id();
    updateFreqCat(id, cat);
}

void ControllerFrequency::updateFreqName(QVariant id, QString name)
{
    
    Entry ref = m_freqs[id.toUuid()].referenceEntry();
    Information inf = ref.info();
    inf.setTitle(name);
    ref.setInfo(inf);
    
    m_freqs[id.toUuid()].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::updateFreqValue(QVariant id, double value)
{
    Entry ref = m_freqs[id.toUuid()].referenceEntry();
    ref.setValue(value);
    m_freqs[id.toUuid()].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::updateFreqCat(QVariant id, QString cat)
{
    Entry ref = m_freqs[id.toUuid()].referenceEntry();
    Information inf = ref.info();
    inf.setCategory(cat);
    ref.setInfo(inf);
    
    m_freqs[id.toUuid()].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::updateFreqType(QVariant id, QString type)
{
    Entry ref = m_freqs[id.toUuid()].referenceEntry();
    ref.setType(type);
    Information in = ref.info();
    in.setCategory("");
    ref.setInfo(in);
    m_freqs[id.toUuid()].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::updateFreqFreq(QVariant id, int f)
{
    m_freqs[id.toUuid()].setFreq((Account::FrequencyEnum)f);
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::displayEntry(QVariant id)
{
    Entry e = entry(id.toUuid());
    QObject* view = m_manager->findChild<QObject*>("linkedDisplayer");
    view->setProperty("entry", QVariant::fromValue(e));
}

QString ControllerFrequency::displayText() const
{
    return tr("Frequency");
}

QString ControllerFrequency::baseText() const
{
    return "FrequencyFeature";
}

QObject* ControllerFrequency::worker(QString name) const
{
    return m_workers[name];
}

void ControllerFrequency::updateFreqEndless(QVariant id, bool e)
{
    m_freqs[id.toUuid()].setEndless(e);
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::updateFreqSupport(QVariant id, int support)
{
    Entry ref = m_freqs[id.toUuid()].referenceEntry();
    ref.setSupport((Account::EntryTypeEnum)support);
    m_freqs[id.toUuid()].setReferenceEntry(ref);
    m_db->updateFrequency(m_freqs[id.toUuid()]);
}

void ControllerFrequency::checker()
{

    for(auto it: m_freqs)
    {
        while(it.endless() && it.end() < QDate::currentDate())
        {
            m_generate->setProperty("freqId", it.id());
            m_generate->setProperty("freqGroup", it.nbGroup()+1);
            auto date = it.end().addDays( Account::nbDay(it.end(), it.freq()));
            generate(date.toString("dd-MM-yyyy"), date.addYears(1).toString("dd-MM-yyyy"));
        }
    }
}
