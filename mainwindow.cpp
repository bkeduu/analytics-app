#include "mainwindow.h"
#include "statustab.h"
#include "consumerstab.h"
#include "generationtab.h"
#include "forecasttab.h"
#include "settingstab.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow{parent}, ui{new Ui::MainWindow}, tabWidget{nullptr}, tabDialog{nullptr}, layout{nullptr} {
	ui->setupUi(this);

	QFont appFont{this->font()};
	appFont.setPixelSize(18);
	appFont.setHintingPreference(QFont::PreferFullHinting);
	this->setFont(appFont);

	setWindowTitle(tr("Analytics system"));
	setWindowIcon(QIcon{":/images/window_icon.png"});

	QString consumers_JSON;
	QFile consumers_JSON_file{"C:\\Users\\leakt\\Documents\\analytics-app\\configs\\consumers.json"};
	consumers_JSON_file.open(QIODevice::ReadOnly | QIODevice::Text);
	consumers_JSON = consumers_JSON_file.readAll();

	QJsonDocument consumers = QJsonDocument::fromJson(consumers_JSON.toUtf8());

	layout = new QHBoxLayout{this};
	tabWidget = new QTabWidget{this};

	tabs[Tab::Status] = StatusTab::getWidget(tr("Status"), tabWidget);

	ConsumersTab* consumersTab = ConsumersTab::getWidget(tr("Consumers"), tabWidget);
	consumersTab->setJSONDocument(consumers);

	tabs[Tab::Consumers] = consumersTab;
	tabs[Tab::Generation] = GenerationTab::getWidget(tr("Generation"), tabWidget);
	tabs[Tab::Forecast] = ForecastTab::getWidget(tr("Forecast"), tabWidget);
	tabs[Tab::Settings] = SettingsTab::getWidget(tr("Settings"), tabWidget);


	foreach (Tab tab, tabs.keys()) {
		tabWidget->addTab(tabs[tab], tabs[tab]->getName());
		tabWidget->setTabIcon(tabWidget->indexOf(tabs[tab]),
							  QIcon{QString{":/images/page_"} + QString::number((int)tab) + ".png"});
	}

	tabWidget->tabBar()->setIconSize(QSize(30, 30));

	layout->addWidget(tabWidget);
	tabWidget->setCurrentWidget(tabs[Tab::Status]);
	tabWidget->setSizePolicy(QSizePolicy{QSizePolicy::Maximum, QSizePolicy::Maximum});

	this->setLayout(layout);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	tabWidget->resize(event->size());
	event->accept();
}

MainWindow::~MainWindow() {
	delete ui;
}
