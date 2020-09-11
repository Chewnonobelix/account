#include "maincontroller.h"

MainController::MainController(int storage)
    : AbstractController(), m_engine(this, QStringLiteral(QML_SOURCE) + "/View")
{
    Q_UNUSED(storage)
    QString message = "Generate from backend";
    qDebug()<<"ControllerDB"<<qRegisterMetaType<ControllerDB>();
    qDebug()<<"ControllerXMLMulti"<<qRegisterMetaType<ControllerXMLMulti>();
    qDebug()<<"Account::FrequencyEnum QML"<<qmlRegisterUncreatableMetaObject(Account::staticMetaObject, "Account", 1,0, "Account", "This is Account's flags");
    qDebug() << "Account::FrequencyEnum" << qRegisterMetaType<Account::FrequencyEnum>();

    qDebug() << "Worker Qml"
             << qmlRegisterUncreatableType<Worker>("Account.Frequency", 1, 0, "Worker", message);
    
    qmlRegisterModule("Account.Style", 1, 0);
    m_dbThread = new QThread;;
    connect(m_dbThread, &QThread::started, this, &MainController::reload);
    try
    {
        setDb(m_settings.database().isEmpty() ? "ControllerDB" : m_settings.database());
    }
    catch(QString except)
    {
        qDebug()<<except;
    }

    auto *context = engine().rootContext();
    context->setContextProperty("_settings", &m_settings);
    context->setContextProperty("_db", db());
    context->setContextProperty("_sync", &m_synchro);
    context->setContextProperty("_main", this);
    context->setContextProperty("_info", &m_info);
    context->setContextProperty("_transfert", &m_transfert);
    context->setContextProperty("_graph", &m_graph);

    m_engine.createWindow(QUrl("/Core/Main.qml"));

#ifdef ENABLE_HOTRELOADING
//    temp.setQmlSourceDir(QML_SOURCE);
#endif
}

MainController::~MainController()
{
    if(m_dbThread->isRunning())
    {
        m_dbThread->terminate();
        m_dbThread->wait();
        delete m_dbThread;
    }
    AbstractController::deleteDb();
}

void MainController::reload()
{
    loadAccount();
    buildModel();
}

void MainController::bind(QVariant id)
{
    pageChange(id.toUuid());
}

int MainController::exec()
{
    //    engine().load(QUrl(QStringLiteral("qrc:/Core/Main.qml")));

    if (engine().rootObjects().size() < 1)
        return -1;

    QObject *root = engine().rootObjects().last();

    QObject *info = root->findChild<QObject *>("infoView");

    if (info) {
        m_info.configure(info);
    }

    QObject* combo = root->findChild<QObject*>("accountSelect");
    
    if(combo)
    {
        loadAccount();
    }

    QObject *transfert = root->findChild<QObject *>("transfert");

    if (transfert) {
        m_transfert.set(transfert);
        connect(&m_transfert, &ControllerTransfert::s_finish, this, &MainController::buildModel);
    }

    for (auto it : m_settings.featuresList()) {
        if (QMetaType::type(it.toLatin1()) == 0) {
            qDebug() << "Unknow Feature" << it;
            continue;
        }

        QMetaType mt(QMetaType::type(it.toLatin1()));
        AbstractController *p = (AbstractController *) mt.create();
        auto sp = dynamic_cast<FeatureBuilder *>(p)->build(&engine(), root);
        if (sp.dynamicCast<QObject>())
            sp.dynamicCast<QObject>()->setParent(this);

        ControllerSettings::registerFeature(sp);
        mt.destroy(p);
    }

    loadFeatures();
    qDebug() << ControllerSettings::registredFeature();

    connect(m_db, &InterfaceDataSave::s_updateEntry, this, &MainController::buildModel);
    connect(m_db, &InterfaceDataSave::s_updateEntry, this, &MainController::pageChange);

    m_settings.init(engine());

    connect(&m_settings, &ControllerSettings::s_language, this, &MainController::languageChange);
    connect(&m_settings, &ControllerSettings::s_finish, this, &MainController::loadFeatures);
    
    QObject* licence = root->findChild<QObject*>("licence");
    if(licence)
        connect(licence, SIGNAL(opened()), this, SLOT(licence()));
    
    QObject* howto = root->findChild<QObject*>("howto");
    if(howto)
        connect(howto, SIGNAL(opened()), this, SLOT(readme()));
    
    QObject* about = root->findChild<QObject*>("about");
    if(about)
        connect(about, SIGNAL(opened()), this, SLOT(about()));
    
    m_graph.set(engine());
    connect(m_db, &InterfaceDataSave::s_updateEntry, &m_graph, &AbstractGraphController::exec);
    m_graph.exec();

    connect(this, SIGNAL(s_totalChanged()), this, SLOT(totalChanged()));

    connect(m_db, &InterfaceDataSave::s_updateEntry, this, &AbstractController::calculTotal);
    connect(this, &AbstractController::s_totalChanged, this, &MainController::previewCalendar);
    languageChange();
    m_synchro.exec();

    changeProfile(m_settings.currentProfile());
    loadProfiles();

    return 0;
}

void MainController::close()
{
    connect(&m_settings, &ControllerSettings::s_finishBackup, &QApplication::closeAllWindows);
    connect(&m_settings, &ControllerSettings::s_finishBackup, &QCoreApplication::quit);
    if(m_settings.autoBackup())
    {
        m_settings.backup();
    }
    else
    {
        QApplication::closeAllWindows();
        QCoreApplication::quit();
    }
}

void MainController::about()
{
    QObject* licence = engine().rootObjects().first()->findChild<QObject*>("about");
    if(licence)
    {
        QFile f("ABOUT.md");
        if(!f.open(QIODevice::ReadOnly))
            return;
        QString l = f.readAll();
        f.close();
        licence->setProperty("text", l);
    }
}

void MainController::readme()
{
    QString language = m_settings.languageCode();
    
    QFile f ("README_"+language+".md");
    if(!f.open(QIODevice::ReadOnly))
        return;
    
    QObject* readme = engine().rootObjects().first()->findChild<QObject*>("howto");
    
    readme->setProperty("text", f.readAll());
    f.close();
}

void MainController::licence()
{
    QObject* licence = engine().rootObjects().first()->findChild<QObject*>("licence");
    if(licence)
    {
        QFile f("LICENSE.md");
        if(!f.open(QIODevice::ReadOnly))
            return;
        QString l = f.readAll();
        f.close();
        licence->setProperty("text", l);
    }
}

void MainController::loadFeatures()
{
    QStringList features;
    features<<QObject::tr("List")<<QObject::tr("Graph");
    QList<QVariant> featureItem;
    for(auto it: m_settings.featuresList())
    {
        if(m_settings.featureEnable(it))
        {
            m_features<<ControllerSettings::features(it);
            features<<ControllerSettings::features(it)->displayText();
            featureItem << QVariant::fromValue(ControllerSettings::features(it)->view);
        }
    }

    emit enableQuickView(true);
    emit featuresChanged(featureItem);
    emit featuresListChanged(features);
}

void MainController::changeProfile(QString name)
{
    m_db->setProfile(name);
    m_settings.setCurrentProfile(name);
    loadAccount();
}

void MainController::update(Entry e)
{
    AbstractController::updateEntry(e);
}

void MainController::add(bool account)
{
    QObject* m = engine().rootObjects().first();
    QObject* h = m->findChild<QObject*>("head");
    QPoint p = QCursor::pos();
    double pX, pY;

    pX = p.x() - m->property("x").toDouble();
    pX /= m->property("width").toDouble();
    
    pY = p.y() - m->property("y").toDouble() - h->property("height").toDouble();
    pY /= m->property("height").toDouble();
    QObject* popup = engine().rootObjects().first()->findChild<QObject*>("addingid");
    popup->setProperty("newAccount", account);
    popup->setProperty("pY", pY); popup->setProperty("pX", pX);
    QMetaObject::invokeMethod(popup, "open");
}

void MainController::adding()
{
    QObject* adding = engine().rootObjects().first()->findChild<QObject*>("addingid");
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
        e.setBlocked(true);
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
        if(date.toDate() > QDate::currentDate())
            i.setEstimated(true);
        e.setAccount(currentAccount());
    }
    AbstractController::addEntry(e);
    
    if(adding->property("newAccount").toBool())
        loadAccount();
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
    QObject* quick = engine().rootObjects().first()->findChild<QObject*>("quick");
    auto cats = m_db->selectCategory();
    QStringList modi = cats.values("income"), modo = cats.values("outcome");
    modi<<""; modo<<"";
    quick->setProperty("outcomeCats", modo);
    quick->setProperty("incomeCats", modi);
}

void MainController::quickAdding()
{
    QObject* quick = engine().rootObjects().first()->findChild<QObject*>("quick");
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
    QObject* quick = engine().rootObjects().first()->findChild<QObject*>("quick");
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

void MainController::remove(QVariant id)
{
    Entry e = AbstractController::entry(id.toUuid());
    m_db->removeEntry(e);
}

void MainController::edit(QVariant id)
{
    if (!id.isNull())
        m_info.view(id.toUuid());
}

void MainController::previewCalendar()
{
    QMap<QDate, Total> all = allTotal();
    QObject* cal = engine().rootObjects().first()->findChild<QObject*>("cal");
    int month = 0;
    int year = 0;
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
        else if(!monthPreview.isEmpty())
        {
            monthPreview<<monthPreview.last();
            monthPreview.last().setDate(itDate);
        }
        
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

void Builder::run()
{
    QVariantList temp;
    for(auto i = 100; i < init.count(); i++)
    {
        t = t + init[i];
        QVariantMap map = init[i];
        
        map.insert("total", QVariant::fromValue(t));
        temp.append(QVariant::fromValue(map));
    }
    model->append(temp);
}

void MainController::buildModel(QUuid)
{
    if(m_modelBuilder && m_modelBuilder->isRunning())
        return;
    
    m_model.clear();
    QList<Entry> ret;
    
    ret = m_db->selectEntry(currentAccount()).values();
    
    Total t;
    for(auto i = 0; i < qMin(ret.count(), 100); i++)
    {
        t = t + ret[i];
        QVariantMap map = ret[i];
        
        map.insert("total", QVariant::fromValue(t));
        m_model.push_back(QVariant::fromValue(map));
    }
    
    if(!m_modelBuilder)
    {
        m_modelBuilder.reset(new Builder);
        m_modelBuilder->setObjectName("Builder Thread");
    }
    
    m_modelBuilder->init = ret;
    
    m_modelBuilder->t = t;
    m_modelBuilder->model = &m_model;
    
    m_modelBuilder->start();
}

void MainController::pageChange(QUuid id)
{
    QObject* skipper = engine().rootObjects().first()->findChild<QObject*>("pageSkip");
    QObject* tab = engine().rootObjects().first()->findChild<QObject*>("entryView");
    
    if(tab && skipper)
    {
        auto ld = dateList();
        int index = -1;
        int first = 0;
        
        QMetaObject::invokeMethod(tab, "unselectAll");
        
        QList<QVariant> currentModel;
        
        std::for_each(m_model.begin(), m_model.end(), [ld, &currentModel](const QVariant& e){
            if(ld.isEmpty() || ld.contains(e.toMap()["date"].toDate()))
                currentModel<<e;
            
        });
        
        int maxPage = currentModel.size() < 100 ? 1 : (currentModel.size() / 100 + 1);
        skipper->setProperty("maxPage", maxPage);
        
        
        first = index != -1 ? (index / 100)+1 : ((skipper->property("pageIndex").toInt()));
        
        first -= 1;
        first *= 100;
        
        QVariantList modelList;
        
        for(auto i = first ; i < qMin(currentModel.size(), first+100); i++)
            modelList<<currentModel[i];
        Qt::SortOrder order = m_settings.sortOrder();
        QString role = m_settings.sortingRole();
        
        std::sort(modelList.begin(), modelList.end(), [order, role](QVariant a, QVariant b) {
            if(order == Qt::AscendingOrder)
                return (a.toMap()[role] < b.toMap()[role]);
            else
                return (a.toMap()[role] > b.toMap()[role]);
        });
        
        for(auto i = 0; i < modelList.size(); i++)
            if(modelList[i].toMap()["id"] == id)
                index = i;
        
        tab->setProperty("model", modelList);
        tab->setProperty("currentRow", index);
    }
}

void MainController::updateQuickView()
{
    auto ld = dateList();
    QObject* quickView = engine().rootObjects().first()->findChild<QObject*>("quickViewDate");
    
    if(quickView)
        quickView->setProperty("currentDate", ld.isEmpty() ? QDate::currentDate(): ld.first());
}

QList<QDate> MainController::dateList() const
{
    QObject* calendar = engine().rootObjects().first()->findChild<QObject*>("cal");
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
    if(acc.isEmpty())
        return;

    AbstractController::setCurrentAccount(acc);
    m_settings.setCurrentAccount(acc);
    calculTotal();

    emit totaleChanged(QVariant::fromValue(accountTotal()));

    buildModel();
    pageChange();
    checkEstimated();
}

void MainController::loadAccount()
{
    QObject* combo = engine().rootObjects().first()->findChild<QObject*>("accountSelect");
    
    if(combo)
    {
        QStringList t = m_db->selectAccount();
        combo->setProperty("model", t);
        
        if(t.isEmpty())
        {
            accountChange("");
            add(true);
        }
        else
        {
            accountChange(!t.contains(m_settings.currentAccount())? t[0] : m_settings.currentAccount());
            combo->setProperty("currentIndex", std::max(0, t.indexOf(m_settings.currentAccount())));
        }
    }
}

void MainController::checkEstimated()
{
    for(auto it: m_features)
        it->checker();

    QList<Entry> list;
    
    for(auto it: m_db->selectEntry(currentAccount()))
    {
        if(it.info().estimated() && it.date() <= QDate::currentDate())
            list<<it;
        else if(it.date() > QDate::currentDate())
            break;
    }
    
    QVariantList vl;
    
    for(auto it: list)
        vl << QVariant::fromValue(it);

    emit checkListChanged(vl);
}

void MainController::validateCheckEstimated(QVariantList tab)
{
    for (int i = 0; i < tab.size(); i++) {
        if (!tab[i].isValid())
            continue;

        Entry e = entry(tab[i].toUuid());

        if (tab[i].toBool()) {
            Information inf = e.info();
            inf.setEstimated(false);
            e.setInfo(inf);
            updateEntry(e);
        } else {
            m_db->removeEntry(e);
        }
    }

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

void MainController::totalChanged()
{
    emit totaleChanged(QVariant::fromValue(accountTotal()));
}

void MainController::addProfile(QString nProfile)
{
    if(m_db->addProfile(nProfile, ""))
    {
        changeProfile(nProfile);
        loadProfiles();
    }
}

void MainController::loadProfiles()
{
    QStringList profiles = m_db->selectProfile();
    if (!profiles.contains(m_db->currentProfile()))
        profiles << m_db->currentProfile();

    emit profilesListChanged(profiles);
    emit currentProfileChanged(m_db->currentProfile());
}

void MainController::deleteProfile(QString name)
{
    if(m_db->deleteProfile(name))
        changeProfile("Default");
    loadProfiles();
}

void MainController::languageChange()
{
    engine().retranslate();
}

void MainController::sortRole(QString role)
{
    m_settings.setSortingRole(role);
    QUuid id = engine().rootObjects().first()->findChild<QObject*>("table")->property("currentId").toUuid();
    pageChange(id);
}

void MainController::sortOrder(int order)
{
    m_settings.setSortOrdre((Qt::SortOrder)order);
    QUuid id = engine().rootObjects().first()->findChild<QObject*>("table")->property("currentId").toUuid();
    pageChange(id);
}
