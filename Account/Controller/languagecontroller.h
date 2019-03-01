#ifndef LANGUAGECONTROLLER_H
#define LANGUAGECONTROLLER_H


#include <QObject>
#include <QApplication>
#include <QTranslator>

class LanguageController: public QObject
{
    Q_OBJECT

private:
    QTranslator m_translator;

public:
    explicit LanguageController();
    ~LanguageController();
};

#endif // LANGUAGECONTROLLER_H
