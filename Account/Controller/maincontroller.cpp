#include "maincontroller.h"

MainController::MainController(): AbstractController()
{
    AbstractController::setCurrentAccount("test_account1");
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

    Entry e;
    QVariant val, date, label;
    QMetaObject::invokeMethod(adding, "value", Q_RETURN_ARG(QVariant, val));
    QMetaObject::invokeMethod(adding, "label", Q_RETURN_ARG(QVariant, label));
    QMetaObject::invokeMethod(adding, "date", Q_RETURN_ARG(QVariant, date));

    e.setDate(date.toDate());
    e.setValue(val.toDouble());
    Information i;
    i.setTitle(label.toString());
    e.setInfo(i);
    qDebug()<<e.date()<<e.value()<<e.info().title();

    AbstractController::addEntry(e);
}
void MainController::remove(int id)
{
    qDebug()<<"Remove"<<id;
}

void MainController::edit(int id)
{
    qDebug()<<"Edit"<< id;
}

void MainController::selection()
{
    qDebug()<<"Selection";
    QObject* calendar = m_engine.rootObjects().first()->findChild<QObject*>("cal");
    QMetaProperty mp = calendar->metaObject()->property(calendar->metaObject()->indexOfProperty("selectedDates"));
    QJSValue array = mp.read(calendar).value<QJSValue>();
    QList<QDate> ld;

    for(int i = 0; i < array.property("length").toInt(); i++)
        ld<<array.property(i).toDateTime().date();


    qDebug()<<ld;

    QList<Entry> ret;

    if(ld.isEmpty())
        ret = AbstractController::entries();
    else
        for(auto it: ld)
            ret<<AbstractController::entries(it);

    qDebug()<<"List ret"<<ret.size();
}
