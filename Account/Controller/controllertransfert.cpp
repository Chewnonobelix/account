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
    QObject *to, *from, *val, *info, *dateField;
    QString accountOut, accountIn, title, date;
    double transfertVal;
    to = m_view->findChild<QObject*>("toCombo");
    from = m_view->findChild<QObject*>("fromCombo");
    val = m_view->findChild<QObject*>("spinVal");
    info = m_view->findChild<QObject*>("fieldInfo");
    dateField = m_view->findChild<QObject*>("dateField");

    accountOut = from->property("currentText").toString();
    accountIn = to->property("currentText").toString();
    title = info->property("text").toString();
    transfertVal = val->property("value").toInt() / 100.0;
    date = dateField->property("text").toString();

    qDebug()<<accountOut<<accountIn<<title<<transfertVal<<QDate::fromString(date, "dd-MM-yyyy");
    Entry in, out;

    out.setAccount(accountOut);
    out.setDate(QDate::fromString(date, "dd-MM-yyyy"));
    out.setValue(transfertVal);
    out.setType("outcome");
    Information inf;
    inf.setTitle(title);
    inf.setEstimated(out.date() > QDate::currentDate());
    out.setInfo(inf);

    in = out;
    in.setType("income");
    in.setAccount(accountIn);
    addEntry(in); addEntry(out);

    qDebug()<<in.date()<<out.date();
    emit s_finish();
}
