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
}
