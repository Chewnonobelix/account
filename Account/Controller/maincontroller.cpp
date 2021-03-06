#include "maincontroller.h"


MainController::MainController(): AbstractController()
{
    //AbstractController::initTestEntry();

    connect(&m_info, ControllerInformation::s_addCategory, this, addCategory);
    connect(&m_graph, GraphController::s_sum, this, receiveSum);

}

MainController::~MainController()
{
    AbstractController::deleteDb();
}

int MainController::exec()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (m_engine.rootObjects().isEmpty())
        return -1;

    QObject* root = m_engine.rootObjects().first();

    connect(root, SIGNAL(adding(bool)), this, SLOT(add(bool)));
    connect(root, SIGNAL(remove(int)), this, SLOT(remove(int)));
    connect(root, SIGNAL(removeAccount(QString)), this, SLOT(deleteAccount(QString)));

    QObject* calendar = root->findChild<QObject*>("cal");

    if(calendar)
    {
        connect(calendar, SIGNAL(s_monthChanged()), this, SLOT(previewCalendar()));
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(selection()));
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

    connect(&m_info, SIGNAL(s_update(Entry)), this, SLOT(update(Entry)));

    QObject* graph = root->findChild<QObject*>("chart");
    if(graph)
    {
        m_graph.set(graph);
        m_graph.exec();
    }

    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    QObject* checker = popup->findChild<QObject*>("checkerModel");

    if(checker)
        connect(checker, SIGNAL(validate()), this, SLOT(validateCheckEstimated()));

    QObject* skipper = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");

    if(skipper)
        connect(skipper, SIGNAL(s_pageChange()), this, SLOT(selection()));

    return 0;
}

void MainController::update(Entry e)
{
    AbstractController::updateEntry(e);
    selection();

    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");

    if(tab)
        QMetaObject::invokeMethod(tab, "selectFromId", Q_ARG(QVariant, e.id()));

}

void MainController::add(bool account)
{
    QObject* item = m_engine.rootObjects().first()->findChild<QObject*>("table");
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
    QMetaObject::invokeMethod(item, "openAdding", Q_ARG(QVariant, pX), Q_ARG(QVariant, pY));
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
        e.setType("Income");

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
    accountChange(currentAccount());

    if(adding->property("newAccount").toBool())
        loadAccount();

    selection();
}
void MainController::remove(int id)
{
    Entry e = AbstractController::entry(id);
    AbstractController::removeEntry(e);
    selection();
}

void MainController::edit(int id)
{
    QObject* info = m_engine.rootObjects().first()->findChild<QObject*>("infoView");

    if(info)
    {
        Entry e = AbstractController::entry(id);
        m_info.set(e, info);
    }
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

void MainController::selection(int)
{
    m_graph.exec();
    QObject* calendar = m_engine.rootObjects().first()->findChild<QObject*>("cal");
    QMetaProperty mp = calendar->metaObject()->property(calendar->metaObject()->indexOfProperty("selectedDates"));
    QJSValue array = mp.read(calendar).value<QJSValue>();
    QList<QDate> ld;
    for(int i = 0; i < array.property("length").toInt(); i++)
        ld<<QDate::fromString(array.property(i).toString(), "dd-MM-yyyy");

    for(int i = 0; i < ld.size(); i++)
        for(int j = i; j < ld.size(); j++)
            if(ld[j] < ld[i])
                ld.swap(i,j);

    QList<Entry> ret;

    if(ld.isEmpty())
        ret = AbstractController::entries();
    else
        for(auto it: ld)
            ret<<AbstractController::entries(it);

    int maxPage = ret.size() < 100 ? 1 : (ret.size() / 100);
    QObject* pageSkip = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");

    if(pageSkip)
    {
        int cMaxPage = pageSkip->property("maxPage").toInt();

        if(maxPage != cMaxPage)
        {
            pageSkip->setProperty("pageIndex", 1);
            pageSkip->setProperty("maxPage", maxPage);
        }
    }

    Total t;

    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    QObject* skipper = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");
    if(tab && skipper)
    {
        QMetaObject::invokeMethod(tab, "unselectAll");
        QMetaObject::invokeMethod(tab, "reset");
        int first = ((skipper->property("pageIndex").toInt()));

        first -= 1;
        first *= 100;
        for(auto i = 0 ; i < ret.size(); i++)
        {
            t = t + ret[i];

            if(i >= first && i < qMin(ret.size(), first+100))
            {
                QVariantMap map;

                map.insert("id", ret[i].id());
                map.insert("date", ret[i].date());
                map.insert("value", ret[i].value());
                map.insert("label", ret[i].label());
                map.insert("type", ret[i].type().toLower());
                map.insert("total", t.value());
                map.insert("estimated", ret[i].info().estimated());
                QMetaObject::invokeMethod(tab, "fAdd", Q_ARG(QVariant, map));
            }
        }
    }

    QObject* head = m_engine.rootObjects().first()->findChild<QObject*>("head");
    if(head)
        head->setProperty("selectionTotal", t.value());

}

void MainController::accountChange(QString acc)
{
    AbstractController::setCurrentAccount(acc);

    QObject* head = m_engine.rootObjects().first()->findChild<QObject*>("head");

    if(head)
    {
        head->setProperty("accountName", acc);
        head->setProperty("total", accountTotal().value());
    }

    int maxPage = entries().size() / 100;
    QObject* pageSkip = m_engine.rootObjects().first()->findChild<QObject*>("pageSkip");

    if(pageSkip)
        pageSkip->setProperty("maxPage", maxPage);

    selection();
    checkEstimated();
}

void MainController::toXml()
{
    qDebug()<<"To Sql";
    XmltoSql xts(*dynamic_cast<ControllerXML*>(m_db));

    bool ret = xts.exec();
    qDebug()<<ret;
}

void MainController::addCategory(QString name, QString type)
{
    AbstractController::addCategory(name, type);
}

void MainController::loadAccount()
{
    QObject* combo = m_engine.rootObjects().first()->findChild<QObject*>("accountSelect");

    if(combo)
    {
        QStringList t = AbstractController::accountList();
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

    for(auto it: entries())
    {
        if(it.info().estimated() && it.date() <= QDate::currentDate())
            list<<it;
    }
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    QObject* checker = popup->findChild<QObject*>("checkerModel");

    if(checker)
    {
        QMetaObject::invokeMethod(checker, "clear");
        for(auto it: list)
        {
            QVariantMap map;
            map.insert("id", it.id());
            map.insert("edate", it.date().toString("dd-MM-yyyy"));
            map.insert("label", it.label());
            map.insert("isChecked", false);
            map.insert("value", it.value());
            QMetaObject::invokeMethod(checker, "fAdd", Q_ARG(QVariant, map));
        }

        int count = checker->property("count").toInt();

        if( count > 0)
            QMetaObject::invokeMethod(popup, "open");
    }
}

void MainController::validateCheckEstimated()
{
    QObject* popup = m_engine.rootObjects().first()->findChild<QObject*>("cEstimated");
    QObject* checker = popup->findChild<QObject*>("checkerModel");

    int count = checker->property("count").toInt();

    for(int i = 0; i < count; i++)
    {
        QVariant id ,isChecked;
        QMetaObject::invokeMethod(checker, "idE", Q_RETURN_ARG(QVariant, id), Q_ARG(QVariant, i));
        QMetaObject::invokeMethod(checker, "isChecked", Q_RETURN_ARG(QVariant, isChecked), Q_ARG(QVariant, i));

        Entry e = entry(id.toInt());

        if(isChecked.toBool())
        {
            Information inf = e.info();
            inf.setEstimated(false);
            e.setInfo(inf);
            updateEntry(e);
        }
        else
        {
            removeEntry(e);
        }
    }

    QMetaObject::invokeMethod(popup, "close");
    selection();
}

void MainController::deleteAccount(QString account)
{
    AbstractController::deletAccount(account);
    loadAccount();
}

void MainController::receiveSum()
{
    previewCalendar();
}
