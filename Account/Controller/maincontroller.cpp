#include "maincontroller.h"


MainController::MainController(): AbstractController()
{
    AbstractController::setCurrentAccount("test_account1");
    AbstractController::initTestEntry();

    //qmlRegisterType<Entry>("Account",1,0,"Entry");
    //    qmlRegisterType<Information>();

    //    qDebug()<<QMetaType::type("Entry");
    //    qDebug()<<qRegisterMetaType<Entry>();
    //    qDebug()<<qRegisterMetaType<Information>();
}

MainController::~MainController()
{}

int MainController::exec()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (m_engine.rootObjects().isEmpty())
        return -1;

    QObject* root = m_engine.rootObjects().first();

    connect(root, SIGNAL(adding()), this, SLOT(add()));
    connect(root, SIGNAL(remove(int)), this, SLOT(remove(int)));
    connect(root, SIGNAL(edit(int)), this, SLOT(edit(int)));
    QObject* calendar = root->findChild<QObject*>("cal");

    if(calendar)
        connect(calendar, SIGNAL(s_datesChanged()), this, SLOT(selection()));

    QObject* combo = root->findChild<QObject*>("accountSelect");

    if(combo)
    {
        QStringList t;
        t<<"test_account1"<<"test_account2";
        combo->setProperty("model", t);
        //        combo->setProperty("model", AbstractController::accountList());
        connect(combo, SIGNAL(s_currentTextChange(QString)), this, SLOT(accountChange(QString)));
        accountChange(t[0]);
    }
    return 0;
}

void MainController::add()
{
    QObject* item = m_engine.rootObjects().first()->findChild<QObject*>("table");
    QVariant ret;
    QMetaObject::invokeMethod(item, "openAdding", Q_RETURN_ARG(QVariant, ret));

    QObject* adding = item->findChild<QObject*>("addingid");

    connect(adding, SIGNAL(accept()), this, SLOT(adding()));
}

void MainController::adding()
{
    QObject* adding = m_engine.rootObjects().first()->findChild<QObject*>("addingid");
    static int ind = 0;
    Entry e;
    QVariant val, date, label, type;
//    QMetaObject::invokeMethod(adding, "value", Q_RETURN_ARG(QVariant, val));
//    QMetaObject::invokeMethod(adding, "label", Q_RETURN_ARG(QVariant, label));
//    QMetaObject::invokeMethod(adding, "date", Q_RETURN_ARG(QVariant, date));

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
    qDebug()<<ind<<e.date()<<e.value()<<e.info().title()<<e.type();
       ind++;
    AbstractController::addEntry(e);
}
void MainController::remove(int id)
{
    qDebug()<<"Remove"<<id;
}

void MainController::edit(int id)
{
    qDebug()<<"Edit"<< id;
    QObject* info = m_engine.rootObjects().first()->findChild<QObject*>("infoTest");

    if(info)
    {
        Entry e = AbstractController::entry(id);
        QObject* model =  m_engine.rootObjects().first()->findChild<QObject*>("entry");
        if(model)
        {
            qDebug()<<"value "<<model->setProperty("value", e.value());
            model->setProperty("id", e.id());
        }

        model =  m_engine.rootObjects().first()->findChild<QObject*>("infoModel");
        if(model)
        {
            model->setProperty("estimated", e.info().estimated());
            model->setProperty("title", e.info().title());
        }
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


    QObject* tab = m_engine.rootObjects().first()->findChild<QObject*>("entryView");
    if(tab){
        for(auto i = 0 ; i < ret.size(); i++)
        {
            QVariantMap map;
            map.insert("id", ret[i].id());
            map.insert("date", ret[i].date());
            map.insert("value", ret[i].value());
            map.insert("label", ret[i].label());
            map.insert("type", ret[i].type());

            QMetaObject::invokeMethod(tab, "fAdd", Q_ARG(QVariant, map));
        }
    }

    Total t;
    for(auto e: ret)
        t = t + e;

    QObject* tot = m_engine.rootObjects().first()->findChild<QObject*>("total");
    tot->setProperty("text", t.value());
}

void MainController::accountChange(QString acc)
{
    AbstractController::setCurrentAccount(acc);
    QObject* account = m_engine.rootObjects().first()->findChild<QObject*>("accountTitle");

    if(account)
        account->setProperty("text", tr("Account: ") + acc);

    selection();
}
