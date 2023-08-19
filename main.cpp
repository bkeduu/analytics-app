#include "client.h"

#include <QApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>
#include <QMessageBox>

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
	int resultCode;

	try {
		resultCode = a.exec();
	}
	catch (std::exception& e) {
		QMessageBox::critical(&c.getMainWindow(), QObject::tr("Critical error"), e.what());
	}

	return resultCode;
}
