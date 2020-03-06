#include "maincontroller.h"


MainController::MainController(int storage): AbstractController()
{
    Q_UNUSED(storage)
    
    qDebug()<<"ControllerDB"<<qRegisterMetaType<ControllerDB>();
    qDebug()<<"ControllerXMLMulti"<<qRegisterMetaType<ControllerXMLMulti>();
    qDebug()<<"Account::FrequencyEnum QML"<<qmlRegisterUncreatableMetaObject(Account::staticMetaObject, "Account", 1,0, "Account", "This is Account's flags");
    qDebug()<<"Account::FrequencyEnum"<<qRegisterMetaType<Account::FrequencyEnum>();
    qDebug()<<"ControllerBudget"<<qRegisterMetaType<ControllerBudget>("BudgetFeature");
    qDebug()<<"ControllerFrequency"<<qRegisterMetaType<ControllerFrequency>("FrequencyFeature");
    qDebug()<<"ControllerCommon"<<qRegisterMetaType<ControllerCommon>("CommonExpanseFeature");
    
    try
    {        
        setDb(m_settings.database().isEmpty() ? "ControllerDB" : m_settings.database());
        m_db->init();
    }
    catch(QString except)
    {
        qDebug()<<except;
    }
    
    m_db->moveToThread(&m_dbThread);
    
    connect(&m_graph, GraphController::s_sum, this, receiveSum);
    connect(&m_dbThread, QThread::started, m_db, InterfaceDataSave::exec);
    
    m_dbThread.start();
    
    m_db->selectProfile();
}

MainController::~MainController()
{
    if(m_dbThread.isRunning())
    {
        m_dbThread.terminate();
        m_dbThread.wait();
    }
    AbstractController::deleteDb();
}

int MainController::exec()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/Core/Main.qml")));
    
    if (m_engine.rootObjects().isEmpty())
        return -1;
    
    QObject* root = m_engine.rootObjects().first();
    
    connect(root, SIGNAL(adding(bool)), this, SLOT(add(bool)));
    connect(root, SIGNAL(remove(int)), this, SLOT(remove(int)));
    connect(root, SIGNAL(removeAccount(QString)), this, SLOT(deleteAccount(QString)));
    connect(root, SIGNAL(s_closing()), this, SLOT(close()));
    
    
    QObject* calendar = root->findChild<QObject*>("cal");
    
    if(calendar)
    {
        connect(calendar, SIGNAL(s_monthChanged()), this, SLOT(previewCalendar()));
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(updateQuickView()));
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(pageChange()));
    }
    
    QObject* combo = root->findChild<QObject*>("accountSelect");
    
    if(combo)
    {
        connect(combo, SIGNAL(s_currentTextChange(QString)), this, SLOT(accountChange(QString)));
        loadAccount();
    }
    
    QObject* adding = root->findChild<QObject*>("addingid");
    
    if(adding)
        connect(adding, SIGNAL(accept()), this, SLOT(adding()));
    
    QObject* view = root->findChild<QObject*>("entryView");
    
    if(view)
        connect(view, SIGNAL(s_view(int)), this, SLOT(edit(int)));
    
    QObject* xml = root->findChild<QObject*>("xmlMenu");
    if(xml)
        connect(xml, SIGNAL(s_xml()), this, SLOT(toXml()));
    
    QObject* graph = root->findChild<QObject*>("chart");
    if(graph)
    {
        m_graph.set(graph);
        m_graph.exec();
    }
    
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    
    if(popup)
        connect(popup, SIGNAL(validate()), this, SLOT(validateCheckEstimated()));
    
    QObject* skipper = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");
    
    if(skipper)
        connect(skipper, SIGNAL(s_pageChange()), this, SLOT(pageChange()));
    
    QObject* transfert = root->findChild<QObject*>("transfert");
    
    if(transfert)
    {
        m_transfert.set(transfert);
        connect(root, SIGNAL(openTransfert()), this, SLOT(openTransfert()));
        connect(&m_transfert, ControllerTransfert::s_finish, this, MainController::buildModel);
    }
    
    QObject* info = root->findChild<QObject*>("infoView");
    
    if(info)
    {
        m_info.configure(info);
        
        QObject* f = info->findChild<QObject*>("frequency");
        if(f)
        {
            QObject* past = f->findChild<QObject*>("frequencyPast");
            if(past)
                connect(past, SIGNAL(s_showFromFrequency(int)), this, SLOT(pageChange(int)));
        }
    }
    
    
    QList<AbstractController*> baseAbstract;
    
    baseAbstract<<&m_info;
    
    for(auto it: m_settings.featuresList())
    {
        
        
        if(QMetaType::type(it.toLatin1()) == 0)
        {
            qDebug()<<"Unknow Feature"<<it;
            continue;
        }
        
        QMetaType mt(QMetaType::type(it.toLatin1()));
        AbstractController* p = (AbstractController*)mt.create();
        auto sp = dynamic_cast<FeatureBuilder*>(p)->build(&m_engine, root, baseAbstract);
        ControllerSettings::registerFeature(sp);
        mt.destroy(p);
    }
    
    loadFeatures();
    qDebug()<<ControllerSettings::registredFeature();
    
    QObject* quick = root->findChild<QObject*>("quick");
    if(quick)
    {
        connect(quick, SIGNAL(s_opening()), this, SLOT(quickOpen()));
        
        QObject* finish = quick->findChild<QObject*>("finish");
        connect(finish, SIGNAL(s_clicked()), this, SLOT(quickAdding()));
        
        QObject* cat = quick->findChild<QObject*>("cat");
        connect(cat, SIGNAL(s_addCategory(QString)), this, SLOT(quickAddCategory(QString)));
    }
    
    QObject* profile = root->findChild<QObject*>("profileMenu");
    
    if(profile)
        connect(profile, SIGNAL(s_profile(QString)), this, SLOT(changeProfile(QString)));
    
    QObject* profiles = m_engine.rootObjects().first()->findChild<QObject*>("popProfile");
    
    if(profiles)
    {
        QObject* okProfile = profiles->findChild<QObject*>("okProfile");
        connect(okProfile, SIGNAL(clicked()), this, SLOT(addProfile()));
    }
    
    QObject* deleteProfile = profile->findChild<QObject*>("deleteProfile");
    if(deleteProfile)
        connect(deleteProfile, SIGNAL(s_deleteProfile(QString)), this, SLOT(deleteProfile(QString)));
    
    loadProfiles();
    
    connect(m_db, InterfaceDataSave::s_updateEntry, this, MainController::buildModel);
    connect(m_db, InterfaceDataSave::s_updateEntry, this, MainController::pageChange);
    
    m_settings.init(m_engine);
    connect(root, SIGNAL(s_openSetting()), &m_settings, SLOT(open()));
    
    connect(&m_settings, ControllerSettings::s_language, this, MainController::languageChange);
    connect(&m_settings, ControllerSettings::s_finish, this, MainController::loadFeatures);
    
    languageChange();
    buildModel();
    
    updateQuickView();
    
    return 0;
}

void MainController::close()
{
    QApplication::closeAllWindows();
}

void MainController::loadFeatures()
{
    QStringList features;
    features<<QObject::tr("List")<<QObject::tr("Graph");
    QList<QObject*> featureItem;
    for(auto it: m_settings.featuresList())
    {
        if(m_settings.featureEnable(it))
        {
            m_features<<ControllerSettings::features(it);
            features<<ControllerSettings::features(it)->displayText();
            featureItem<<ControllerSettings::features(it)->view;
        }
    }
    
    
    QObject* swipe = m_engine.rootObjects().first()->findChild<QObject*>("swipe");
    if(swipe)
    {
        while(swipe->property("count").toInt() > 2)
            QMetaObject::invokeMethod(swipe, "takeItem", Q_ARG(int, swipe->property("count").toInt() - 1));
        
        for(auto it: featureItem)
            QMetaObject::invokeMethod(swipe, "addItem", Q_ARG(QQuickItem*, dynamic_cast<QQuickItem*>(it)));
    }
    
    QObject* featuresRepetear = m_engine.rootObjects().first()->findChild<QObject*>("features");
    if(featuresRepetear)
        featuresRepetear->setProperty("model", features);
}

void MainController::changeProfile(QString name)
{
    m_db->setProfile(name);
    loadAccount();
}

void MainController::update(Entry e)
{
    AbstractController::updateEntry(e);
}

void MainController::add(bool account)
{
    QObject* m = m_engine.rootObjects().first();
    QObject* h = m->findChild<QObject*>("head");
    QObject* mb = m->findChild<QObject*>("menuBar");
    QPoint p = QCursor::pos();
    double pX, pY;
    pX = p.x() - m->property("x").toDouble();
    pX /= m->property("width").toDouble();
    
    pY = p.y() - m->property("y").toDouble() - h->property("height").toDouble() - mb->property("height").toDouble();
    pY /= m->property("height").toDouble();
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("addingid");
    popup->setProperty("newAccount", account);
    popup->setProperty("pY", pY); popup->setProperty("pX", pX);
    QMetaObject::invokeMethod(popup, "open");
}

void MainController::adding()
{
    QObject* adding = m_engine.rootObjects().first()->findChild<QObject*>("addingid");
    Entry e;
    
    if(adding->property("newAccount").toBool())
    {
        QVariant val, date, account;
        
        val = adding->property("v_val");
        date = adding->property("v_date");
        account = adding->property("v_title");
        e.setAccount(account.toString());
        e.setDate(QDate::fromString(date.toString(), "dd-MM-yyyy"));
        e.setValue(val.toDouble());
        e.setType("income");
        
        Information i;
        i.setTitle("Initial");
        e.setInfo(i);
    }
    else
    {
        QVariant val, date, label, type;
        
        val = adding->property("v_val");
        date = adding->property("v_date");
        label = adding->property("v_title");
        type = adding->property("v_type");
        
        e.setDate(QDate::fromString(date.toString(), "dd-MM-yyyy"));
        e.setValue(val.toDouble());
        e.setType(type.toString());
        Information i;
        i.setTitle(label.toString());
        e.setInfo(i);
        if(date > QDate::currentDate())
            i.setEstimated(true);
        e.setAccount(currentAccount());
    }
    AbstractController::addEntry(e);
    
    if(adding->property("newAccount").toBool())
        loadAccount();
    
    int id = 0;
    for(auto it: m_db->selectEntry(currentAccount()))
        id = std::max(id, it.id());
    
    pageChange(id);
}

void MainController::addEntryMain(Entry  e)
{
    Information i = e.info();
    
    if(e.date() > QDate::currentDate())
        i.setEstimated(true);
    e.setAccount(currentAccount());
    
    e.setInfo(i);
    
    AbstractController::addEntry(e);
}

void MainController::quickOpen()
{
    QObject* quick = m_engine.rootObjects().first()->findChild<QObject*>("quick");
    auto cats = m_db->selectCategory();
    QStringList modi = cats.values("income"), modo = cats.values("outcome");
    modi<<""; modo<<"";
    quick->setProperty("outcomeCats", modo);
    quick->setProperty("incomeCats", modi);
}

void MainController::quickAdding()
{
    QObject* quick = m_engine.rootObjects().first()->findChild<QObject*>("quick");
    if(quick)
    {
        auto et = quick->property("entry").value<QJSValue>();
        Entry e;
        Information i;
        i.setCategory(et.property("category").toString());
        e.setDate(QDate::fromString(et.property("date").toString(), "dd-MM-yyyy"));
        e.setValue(et.property("value").toNumber());
        e.setType(et.property("type").toString());
        i.setEstimated(e.date() > QDate::currentDate());
        i.setTitle(et.property("title").toString());
        e.setInfo(i);
        e.setAccount(currentAccount());
        
        m_db->addEntry(e);
    }
}

void MainController::quickAddCategory(QString cat)
{
    QObject* quick = m_engine.rootObjects().first()->findChild<QObject*>("quick");
    QObject* typecombo = quick->findChild<QObject*>("type");
    QObject* combo = quick->findChild<QObject*>("cat");
    if(typecombo)
    {
        QString type = typecombo->property("currentValue").toString();
        m_db->addCategory(cat, type);
        QStringList list = m_db->selectCategory().values(type);
        list<<"";
        quick->setProperty(type == "income" ? "incomeCats" : "outcomeCats", list);
        QMetaObject::invokeMethod(combo, "setting", Q_ARG(QVariant, cat));
    }
}

void MainController::remove(int id)
{
    Entry e = AbstractController::entry(id);
    m_db->removeEntry(e);
//    accountChange(currentAccount());
}

void MainController::edit(int id)
{
    m_info.view(id);
}

void MainController::previewCalendar()
{
    QMap<QDate, Total> all = m_graph.sum();
    QObject* cal = m_engine.rootObjects().first()->findChild<QObject*>("cal");
    int month;
    int year;
    if(cal)
    {
        year = cal->property("currentYear").toInt();
        month = cal->property("currentMonth").toInt() + 1;
    }
    
    QVector<Total> monthPreview, dayPreview;
    QDate itDate;
    itDate.setDate(year, month, 1);
    QDate last = itDate.addMonths(1);
    while(itDate < last)
    {
        
        if(all.contains(itDate))
            monthPreview<<all[itDate];
        
        if(all.contains(itDate))
        {
            auto it = all.find(itDate);
            
            if((it) == all.begin())
                dayPreview<<all[itDate];
            else
                dayPreview<<(*it - *(it-1));
        }
        
        itDate = itDate.addDays(1);
    }
    
    QObject* model = cal->findChild<QObject*>("calendarPreview");
    QMetaObject::invokeMethod(model, "clear");
    QDate first;
    first.setDate(year, month, 1);
    for(auto i = 0; i < dayPreview.size(); i++)
    {
        QVariantMap map;
        map.insert("day", dayPreview[i].date().day());
        map.insert("value", dayPreview[i].value());
        
        QMetaObject::invokeMethod(model, "add", Q_ARG(QVariant, map));
    }
    
    model = cal->findChild<QObject*>("totalPreview");
    QMetaObject::invokeMethod(model, "clear");
    
    for(auto i = 0; i < monthPreview.size() ; i++)
    {
        QVariantMap map;
        map.insert("day", monthPreview[i].date().day());
        map.insert("value", monthPreview[i].value());
        
        if(monthPreview[i].date() >= first && monthPreview[i].date().isValid())
        {
            QMetaObject::invokeMethod(model, "add", Q_ARG(QVariant, map));
        }
    }
}

void MainController::buildModel(int id)
{
    m_model.clear();
    if(id < 0) id = -1;
    
    QList<Entry> ret;
    
    ret = m_db->selectEntry(currentAccount()).values();

    Total t;
    
    for(auto i = 0; i < ret.count(); i++)
    {
        t = t + ret[i];
        QVariantMap map = ret[i];
        
        map.insert("total", QVariant::fromValue(t));
        m_model<<QVariant::fromValue(map);
    }
}

void MainController::pageChange(int id)
{
    QObject* skipper = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");
    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    
    if(tab && skipper)
    {
        auto ld = dateList();
        int index = -1;        
        int first = 0;
        
        QMetaObject::invokeMethod(tab, "unselectAll");
        
        QList<QVariant> currentModel;
        
        std::for_each(m_model.begin(), m_model.end(), [ld, id, &currentModel, &index](const QVariant& e){
            if(ld.contains(e.toMap()["date"].toDate()))
                currentModel<<e;
            
            if(id == e.toMap()["id"].toInt())
                index = currentModel.count() % 100;
        });
        currentModel = currentModel.isEmpty() ? m_model : currentModel;
        int maxPage = currentModel.size() < 100 ? 1 : (currentModel.size() / 100 + 1);
        skipper->setProperty("maxPage", maxPage);


        first = index != -1 ? (index / 100)+1 : ((skipper->property("pageIndex").toInt()));
        
        first -= 1;
        first *= 100;
        
        QVariantList modelList;
        
        for(auto i = first ; i < qMin(currentModel.size(), first+100); i++)
            modelList<<currentModel[i];
        
        tab->setProperty("model", modelList);
        tab->setProperty("currentRow", index%100);
    }
}

void MainController::updateQuickView()
{
    auto ld = dateList();
    QObject* quickView = m_engine.rootObjects().first()->findChild<QObject*>("quickViewDate");
    
    if(quickView)
        quickView->setProperty("currentDate", ld.isEmpty() ? QDate::currentDate(): ld.first());
}

QList<QDate> MainController::dateList() const
{
    QObject* calendar = m_engine.rootObjects().first()->findChild<QObject*>("cal");
    QMetaProperty mp = calendar->metaObject()->property(calendar->metaObject()->indexOfProperty("selectedDates"));
    QJSValue array = mp.read(calendar).value<QJSValue>();
    QList<QDate> ld;
    for(int i = 0; i < array.property("length").toInt(); i++)
        ld<<QDate::fromString(array.property(i).toString(), "dd-MM-yyyy");
    
    for(int i = 0; i < ld.size(); i++)
        for(int j = i; j < ld.size(); j++)
            if(ld[j] < ld[i])
                ld.swapItemsAt(i,j);
    
    return ld;
}

void MainController::accountChange(QString acc)
{
    AbstractController::setCurrentAccount(acc);
    
    QObject* head = m_engine.rootObjects().first()->findChild<QObject*>("head");
    
    if(head)
    {
        head->setProperty("accountName", acc);
        head->setProperty("total", QVariant::fromValue(accountTotal()));
    }
    
    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    
    if(tab)
        tab->setProperty("model", QVariantList());
    
    int maxPage = m_db->selectEntry(currentAccount()).size() / 100;
    QObject* pageSkip = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");
    
    if(pageSkip)
        pageSkip->setProperty("maxPage", maxPage);
    
    buildModel();
    pageChange();
    checkEstimated();
}

void MainController::toXml()
{
    qDebug()<<"To Sql";
    XmltoSql xts(*dynamic_cast<ControllerXMLMulti*>(m_db));
    
    bool ret = xts.exec();
    qDebug()<<ret;
}

void MainController::loadAccount()
{
    QObject* combo = m_engine.rootObjects().first()->findChild<QObject*>("accountSelect");
    
    if(combo)
    {
        QStringList t = m_db->selectAccount();
        if(t.size() == 0)
        {
            add(true);
        }
        combo->setProperty("model", t);
        connect(combo, SIGNAL(s_currentTextChange(QString)), this, SLOT(accountChange(QString)));
        
        if(t.isEmpty())
        {
            accountChange("");
            add(true);
        }
        else
            accountChange(t[0]);
    }
}

void MainController::checkEstimated()
{
    QList<Entry> list;
    
    for(auto it: m_db->selectEntry(currentAccount()))
    {
        if(it.info().estimated() && it.date() <= QDate::currentDate())
            list<<it;
    }
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    QVariantList vl;
    
    for(auto it: list)
        vl<<QVariant::fromValue(it);
    
    QObject* v = popup->findChild<QObject*>("repeater");
    
    if(v)
        v->setProperty("model", vl);
    
    if(!vl.isEmpty())
        QMetaObject::invokeMethod(popup, "open");
}

void MainController::validateCheckEstimated()
{
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    
    for(int i = 0; i < popup->property("tab").toList().size(); i++)
    {
        if(!popup->property("tab").toList()[i].isValid())
            continue;
        
        Entry e = entry(i);
        
        if(popup->property("tab").toList()[i].toBool())
        {
            Information inf = e.info();
            inf.setEstimated(false);
            e.setInfo(inf);
            updateEntry(e);
        }
        else
        {
            m_db->removeEntry(e);
        }
    }
    
    QMetaObject::invokeMethod(popup, "close");
    buildModel();
}

void MainController::deleteAccount(QString account)
{
    m_db->removeAccount(account);
    loadAccount();
}

void MainController::receiveSum()
{
    previewCalendar();
}

void MainController::openTransfert()
{
    m_transfert.exec();
}

void MainController::addProfile()
{
    QObject* profiles = m_engine.rootObjects().first()->findChild<QObject*>("popProfile");
    
    QString nProfile = profiles->findChild<QObject*>("profileName")->property("text").toString();
    //    QString password = profiles->findChild<QObject*>("password")->property("text").toString();
    
    QMetaObject::invokeMethod(profiles, "close");
    if(m_db->addProfile(nProfile, ""))
    {
        changeProfile(nProfile);
        loadProfiles();
    }
}

void MainController::loadProfiles()
{
    QObject* profile = m_engine.rootObjects().first()->findChild<QObject*>("profileRepeater");
    
    if(profile)
    {
        QStringList profiles = m_db->selectProfile();
        if(!profiles.contains(m_db->currentProfile()))
            profiles<<m_db->currentProfile();
        
        profile->setProperty("current", m_db->currentProfile());
        profile->setProperty("model", profiles);
    }
    
}

void MainController::deleteProfile(QString name)
{
    if(m_db->deleteProfile(name))
        changeProfile("Default");
    loadProfiles();
}

void MainController::languageChange()
{
    m_engine.retranslate();
}
