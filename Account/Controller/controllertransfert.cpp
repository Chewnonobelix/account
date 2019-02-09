#include "controllertransfert.h"

ControllerTransfert::ControllerTransfert(): AbstractController()
{

}

ControllerTransfert::~ControllerTransfert()
{}

int ControllerTransfert::exec()
{
    if(m_view)
    {
        auto accounts = accountList();
        QMetaObject::invokeMethod(m_view, "addAccount", Q_ARG(QVariant, accounts));
        QMetaObject::invokeMethod(m_view, "open");
    }
    return 0;
}

void ControllerTransfert::set(QObject * view)
{
    m_view = view;

    if(m_view)
        connect(m_view, SIGNAL(s_accept()), this, SLOT(accept()));
}


void ControllerTransfert::accept()
{
    QObject *to, *from, *val, *info;
    QString accountOut, accountIn, title;
    double transfertVal;
    to = m_view->findChild<QObject*>("toCombo");
    from = m_view->findChild<QObject*>("fromCombo");
    val = m_view->findChild<QObject*>("spinVal");
    info = m_view->findChild<QObject*>("fieldInfo");

    accountOut = from->property("currentText").toString();
    accountIn = to->property("currentText").toString();
    title = info->property("text").toString();
    transfertVal = val->property("value").toInt() / 100.0;

    qDebug()<<accountOut<<accountIn<<title<<transfertVal;
    Entry in, out;

    out.setAccount(accountOut);
    out.setValue(transfertVal);
    out.setType("outcome");
    Information inf;
    inf.setTitle(title);
    inf.setEstimated(out.date() > QDate::currentDate());
    out.setInfo(inf);

    in = out;
    in.setType("income");

    addEntry(in); addEntry(out);

}
