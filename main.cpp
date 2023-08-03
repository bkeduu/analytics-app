#include "client.h"

#include <QApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "analytics-app_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			a.installTranslator(&translator);
			break;
		}
	}

	Client c;
	return a.exec();
}
