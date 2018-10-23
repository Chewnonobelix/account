#include "maincontroller.h"


MainController::MainController(): AbstractController()
{
    //AbstractController::initTestEntry();

    connect(&m_info, ControllerInformation::s_addCategory, this, addCategory);
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

    QObject* calendar = root->findChild<QObject*>("cal");

    if(calendar)
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(selection()));

    QObject* combo = root->findChild<QObject*>("accountSelect");

    if(combo)
    {
        QStringList t = AbstractController::accountList();
        if(t.isEmpty())
            t<<"";
        combo->setProperty("model", t);
        connect(combo, SIGNAL(s_currentTextChange(QString)), this, SLOT(accountChange(QString)));
        accountChange(t[0]);
    }

    QObject* adding = root->findChild<QObject*>("addingid");

    if(adding)
        connect(adding, SIGNAL(accept()), this, SLOT(adding()));

    QObject* view = root->findChild<QObject*>("entryView");

    if(view)
        connect(view, SIGNAL(s_view(int)), this, SLOT(edit(int)));

    QObject* xml = root->findChild<QObject*>("xmlMenu");
    if(xml)
        connect(xml, SIGNAL(s_xml(bool)), this, SLOT(toXml(bool)));

    connect(&m_info, SIGNAL(s_update(Entry)), this, SLOT(update(Entry)));

    QObject* graph = root->findChild<QObject*>("chart");
    if(graph)
    {
        m_graph.set(graph);
        m_graph.exec();
    }

    return 0;
}

void MainController::update(Entry e)
{
    AbstractController::updateEntry(e);
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

    if(adding->property("newAccount").toBool())
        loadAccount();

    selection();
}
void MainController::remove(int id)
{
    qDebug()<<"Remove"<<id;

    Entry e = AbstractController::entry(id);
    AbstractController::removeEntry(e);
    selection();
}

void MainController::edit(int id)
{
    QObject* info = m_engine.rootObjects().first()->findChild<QObject*>("infoView");
    qDebug()<<"Edit"<< id;

    if(info)
    {
        Entry e = AbstractController::entry(id);
        m_info.set(e, info);
    }
}

void MainController::selection()
{
    qDebug()<<"Selection";
    QObject* calendar = m_engine.rootObjects().first()->findChild<QObject*>("cal");
    QMetaProperty mp = calendar->metaObject()->property(calendar->metaObject()->indexOfProperty("selectedDates"));
    QJSValue array = mp.read(calendar).value<QJSValue>();
    QList<QDate> ld;
    for(int i = 0; i < array.property("length").toInt(); i++)
    {
        ld<<QDate::fromString(array.property(i).toString(), "dd-MM-yyyy");
    }

    QList<Entry> ret;

    if(ld.isEmpty())
        ret = AbstractController::entries();
    else
        for(auto it: ld)
            ret<<AbstractController::entries(it);


    auto cpm1 = [](QDate d1, QDate d2)
    {
        if(d1.isValid() && d2.isValid())
        {
            if(d1 < d2)
                return d1;
        }
        else if(d1.isValid())
            return d1;

        return d2;
    };

    auto cpm2 = [](QDate d1, QDate d2)
    {
        if(d1.isValid() && d2.isValid())
        {
            if(d1 > d2)
                return d1;
        }
        else if(d1.isValid())
            return d1;

        return d2;
    };

    Total m, e, t;
    QDate minD, maxD;
    double minV, maxV;

    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    if(tab){
        if(!ret.isEmpty())
        {
            minV = ret.first().value();
            maxV = ret.first().value();
        }
        QMetaObject::invokeMethod(tab, "reset");
        for(auto i = 0 ; i < ret.size(); i++)
        {
            QVariantMap map;
            minD = cpm1(minD, ret[i].date());
            maxD = cpm2(maxD, ret[i].date());
            t = t + ret[i];

            if(!ret[i].info().estimated())
                m = m + ret[i];

            if(ret[i].info().estimated())
                e = e + ret[i];

            minV = minV < t.value() ? minV : t.value();
            maxV = maxV > t.value() ? maxV : t.value();

            map.insert("id", ret[i].id());
            map.insert("date", ret[i].date());
            map.insert("value", ret[i].value());
            map.insert("label", ret[i].label());
            map.insert("type", ret[i].type().toLower());
            map.insert("total", t.value());
            map.insert("real", m.value());
            map.insert("estimated", e.value());
            QMetaObject::invokeMethod(tab, "fAdd", Q_ARG(QVariant, map));
//            QMetaObject::invokeMethod(chart, "addData", Q_ARG(QVariant, QDateTime(m.date())), Q_ARG(QVariant, m.value()),
//                                      Q_ARG(QVariant, QDateTime(e.date())), Q_ARG(QVariant,e.value()),
//                                      Q_ARG(QVariant, QDateTime(t.date())), Q_ARG(QVariant,t.value()));
        }

        minV -= 10;
        maxV += 10;
//        QMetaObject::invokeMethod(chart, "setMinMaxDate", Q_ARG(QVariant, minD), Q_ARG(QVariant, maxD));
//        QMetaObject::invokeMethod(chart, "setMinMaxValue", Q_ARG(QVariant, minV), Q_ARG(QVariant, maxV));
//        QMetaObject::invokeMethod(chart, "reset");
    }



    QObject* head = m_engine.rootObjects().first()->findChild<QObject*>("head");
    if(head)
        head->setProperty("total", t.value());
}

void MainController::accountChange(QString acc)
{
    AbstractController::setCurrentAccount(acc);

    QObject* head = m_engine.rootObjects().first()->findChild<QObject*>("head");

    if(head)
        head->setProperty("accountName", acc);

    selection();
}

void MainController::toXml(bool xml)
{
    qDebug()<<"To xml"<<xml;
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
        if(t.isEmpty())
            t<<"";
        combo->setProperty("model", t);
        connect(combo, SIGNAL(s_currentTextChange(QString)), this, SLOT(accountChange(QString)));
        accountChange(t[0]);
    }
}
