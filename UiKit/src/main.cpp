#include <QGuiApplication>
#include <QStringList>
#include <QUrl>
#include <qqml.h>

#include <liveqmlengine.h>

#include "demodatagenerator.h"

// UiKit is a dev-only component gallery: LiveQmlEngine (see
// lib/DesignLibrary/DesignPattern) hot-reloads Main.qml and everything it
// pulls in (UiKitSection.qml, the OB* components, the Style singletons)
// straight from disk whenever one of those .qml files changes.
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // UiKit loads .qml files from disk at runtime rather than through
    // qt_add_qml_module (see CMakeLists.txt), so DemoDataGenerator is
    // registered by hand instead of via QML_ELEMENT.
    qmlRegisterType<DemoDataGenerator>("OpenAccount.UiKit", 1, 0, "DemoDataGenerator");

    const QStringList qmlSourceDirs{
        QStringLiteral(UIKIT_QML_SOURCE_DIR "/"),
        QStringLiteral(VIEW_QML_SOURCE_DIR "/"),
    };

    LiveQmlEngine liveEngine(nullptr, qmlSourceDirs);
    liveEngine.createWindow(QUrl(QStringLiteral("Main.qml")));

    return app.exec();
}
