#include "controllerinformation.h"

ControllerInformation::ControllerInformation(): AbstractController()
{
}

ControllerInformation::~ControllerInformation()
{
}

int ControllerInformation::exec()
{
    QObject* freqItem = m_view->findChild<QObject*>("frequency");
    
    if(freqItem)
    {
        freqItem->setProperty("visible", m_entry.hasMetadata("frequency"));
        if(m_entry.hasMetadata("frequency"))
        {
            int f = m_entry.metaData<int>("frequency");
            int g = m_entry.metaData<int>("freqGroup");
            auto freqs = m_db->selectFrequency();
            QObject* skipper = m_view->findChild<QObject*>("pageSwipper");
            int page = skipper->property("pageIndex").toInt();
            page--;
            for(auto it: freqs)
            {
                if(it.id() == f)
                {
                    auto le = it.listEntries(g);
                    QVariantList model;

                    skipper->setProperty("maxPage", le.size() / 100 + 1);
                    
                    for(int i = (page*100); i < le.size() && i < (page+1)*100; i++)
                        model<<le[i];
                    
                    QObject* lf = freqItem->findChild<QObject*>("frequencyPast");
                    if(lf)
                        lf->setProperty("model", QVariant::fromValue(model));
                }
            }
        }
    }
    
    return 0;
}

void ControllerInformation::configure(QObject * view)
{
    m_view = view;

    if(m_view)
    {
        QObject* child = m_view->findChild<QObject*>("entryEdit");
        
        connect(child, SIGNAL(s_titleChanged(QString)), this, SLOT(titleChange(QString)));
        connect(child, SIGNAL(s_valueChanged(double)), this, SLOT(valueChange(double)));
        
        QObject* skip = m_view->findChild<QObject*>("pageSwipper");
        
        connect(skip, SIGNAL(s_pageChange()), this , SLOT(pageChange()));    
        
        QObject  *catItem;
    
        catItem = child->findChild<QObject*>("category");
        if(catItem)
        {
            connect(catItem, SIGNAL(s_currentTextChanged(QString)), this, SLOT(categoryChange(QString)));
            connect(catItem, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
        }

        QObject* support = child->findChild<QObject*>("support");

        if(support)
            connect(support, SIGNAL(s_supportChanged(int)), this, SLOT(supportChange(int)));
    }
}

void ControllerInformation::view(int id)
{
    m_entry = AbstractController::entry(id);

    auto child = m_view->findChild<QObject*>("entryEdit");

    QStringList catList = m_db->selectCategory().values(m_entry.type());
    catList<<"";

    QObject  *catItem;

    catItem = child->findChild<QObject*>("category");
    
    if(catItem)
    {
        catItem->setProperty("blocked", true);
        child->setProperty("catModel", catList);
        catItem->setProperty("blocked", false);
    }

    child->setProperty("entry", QVariant::fromValue(m_entry));

    m_view->setProperty("visible", id != -1 && !m_entry.isBlocked());
    exec();
}

void ControllerInformation::titleChange(QString title)
{
    Information info = m_entry.info();
    info.setTitle(title);
    m_entry.setInfo(info);

    updateEntry(m_entry);
}

void ControllerInformation::valueChange(double value)
{
    m_entry.setValue(value);

    updateEntry(m_entry);
    emit s_update(m_entry.id());
}

void ControllerInformation::categoryChange(QString cat)
{
    Information info = m_entry.info();
    QString old = info.category();   
    info.setCategory(cat);
    m_entry.setInfo(info);

    updateEntry(m_entry);
    emit s_changeCat(old, m_entry.id());
}

void ControllerInformation::addNewCategory(QString cat)
{
    m_db->addCategory(cat, m_entry.type());
    categoryChange(cat);
    view(m_entry.id());
    emit s_exec();
    
}

void ControllerInformation::pageChange()
{
    exec();
}

void ControllerInformation::supportChange(int support)
{
    m_entry.setSupport((Account::EntryTypeEnum)support);
    m_db->updateEntry(m_entry);
    emit s_exec();
}
