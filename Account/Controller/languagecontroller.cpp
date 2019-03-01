#include "languagecontroller.h"

LanguageController::LanguageController(): QObject(nullptr)
{
    QString language = QLocale::system().name().section('-', 0, 0);
    m_translator.load("account_"+language+".qm");
    QApplication::installTranslator(&m_translator);
}

LanguageController::~LanguageController()
{
}

