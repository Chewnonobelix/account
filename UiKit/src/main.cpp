#include <QGuiApplication>
#include <QStringList>
#include <QUrl>

#include <liveqmlengine.h>

// UiKit is a dev-only component gallery: LiveQmlEngine (see
// lib/DesignLibrary/DesignPattern) hot-reloads Main.qml and everything it
// pulls in (UiKitSection.qml, the OB* components, the Style singletons)
// straight from disk whenever one of those .qml files changes.
int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    const QStringList qmlSourceDirs{
        QStringLiteral(UIKIT_QML_SOURCE_DIR "/"),
        QStringLiteral(VIEW_QML_SOURCE_DIR "/"),
    };

    LiveQmlEngine liveEngine(nullptr, qmlSourceDirs);
    liveEngine.createWindow(QUrl(QStringLiteral("Main.qml")));

    return app.exec();
}
