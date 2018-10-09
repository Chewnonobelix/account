#include "maincontroller.h"


MainController::MainController(): AbstractController()
{
    AbstractController::setCurrentAccount("test_account1");
    AbstractController::initTestEntry();
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

    connect(root, SIGNAL(adding()), this, SLOT(add()));
    connect(root, SIGNAL(remove(int)), this, SLOT(remove(int)));

    QObject* calendar = root->findChild<QObject*>("cal");

    if(calendar)
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(selection()));

    QObject* combo = root->findChild<QObject*>("accountSelect");

    if(combo)
    {
        QStringList t;
        t<<"test_account1"<<"test_account2";
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

    return 0;
}

void MainController::add()
{
    QObject* item = m_engine.rootObjects().first()->findChild<QObject*>("table");
    QVariant ret;
    QMetaObject::invokeMethod(item, "openAdding", Q_RETURN_ARG(QVariant, ret));
}

void MainController::adding()
{
    QObject* adding = m_engine.rootObjects().first()->findChild<QObject*>("addingid");
    Entry e;
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
    e.setAccount(currentAccount());

    AbstractController::addEntry(e);
    selection();
}
void MainController::remove(int id)
{
    qDebug()<<"Remove"<<id;
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

    Total t;

    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    if(tab){
        QMetaObject::invokeMethod(tab, "reset");
        for(auto i = 0 ; i < ret.size(); i++)
        {
            QVariantMap map;
            t = t + ret[i];
            map.insert("id", ret[i].id());
            map.insert("date", ret[i].date());
            map.insert("value", ret[i].value());
            map.insert("label", ret[i].label());
            map.insert("type", ret[i].type().toLower());
            map.insert("total", t.value());
            QMetaObject::invokeMethod(tab, "fAdd", Q_ARG(QVariant, map));
        }
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
