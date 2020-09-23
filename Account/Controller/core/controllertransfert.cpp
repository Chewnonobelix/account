#include "controllertransfert.h"

ControllerTransfert::ControllerTransfert(): AbstractController()
{

}

ControllerTransfert::~ControllerTransfert()
{}

int ControllerTransfert::exec()
{
    emit accountListChanged(m_db->selectAccount());
    emit openChanged();

    return 0;
}

void ControllerTransfert::set(QObject * view)
{
    m_view = view;
}


void ControllerTransfert::accept()
{
    Entry in(m_entry), out(m_entry);

    out.setAccount(m_outcomeAccount);
    out.setType("outcome");
    setCurrentAccount(out.account());
    addEntry(out);

    in.setType("income");
    in.setAccount(m_incomeAccount);
    setCurrentAccount(in.account());
    addEntry(in);

    emit s_finish();
}

void ControllerTransfert::onDateChanged(QDate d)
{
    m_entry.setDate(d);

    m_entry.setEstimated(d > QDate::currentDate());
}

void ControllerTransfert::onTitleChanged(QString t)
{
    m_entry.setTitle(t);
}

void ControllerTransfert::onValueChanged(double v)
{
    m_entry.setValue(v);
}
