#include <QApplication>
#include "MainWindow.h"
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("chloerei");
    QCoreApplication::setOrganizationDomain("chloerei.com");
    QCoreApplication::setApplicationName("qtalk");

    QSettings setting;
    QString language = setting.value("general/language").toString();
    QTranslator translator;
    QString tsPath = QCoreApplication::applicationDirPath() + "/translations";
    qDebug() << tsPath + "/qtalk_" + language;
    translator.load(tsPath + "/qtalk_" + language);
    qApp->installTranslator(&translator);


    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
