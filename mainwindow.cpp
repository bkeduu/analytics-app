#include "mainwindow.h"
#include "statustab.h"
#include "consumerstab.h"
#include "generationtab.h"
#include "forecasttab.h"
#include "settingstab.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), tabWidget(nullptr), tabDialog(nullptr), layout(nullptr) {

	ui->setupUi(this);

	setWindowTitle(tr("Analytics system"));
	setWindowIcon(QIcon(":/images/window_icon.png"));

	layout = new QGridLayout(this);
	tabWidget = new QTabWidget(this);

	tabs[Tab::Status] = StatusTab::getWidget(tr("Status"), tabWidget);
	tabs[Tab::Forecast] = ConsumersTab::getWidget(tr("Consumers"), tabWidget);
	tabs[Tab::Generation] = GenerationTab::getWidget(tr("Generation"), tabWidget);
	tabs[Tab::Consumers] = ForecastTab::getWidget(tr("Forecast"), tabWidget);
	tabs[Tab::Settings] = SettingsTab::getWidget(tr("Settings"), tabWidget);

	foreach (Tab tab, tabs.keys()) {
		tabWidget->addTab(tabs[tab], tabs[tab]->getName());
		tabWidget->setTabIcon(tabWidget->indexOf(tabs[tab]),
							  QIcon(QString(":/images/page_") + QString::number((int)tab) + ".png"));
	}

	tabWidget->tabBar()->setMinimumHeight(100);
	tabWidget->setMinimumHeight(100);
	tabWidget->tabBar()->setDocumentMode(true);
	tabWidget->tabBar()->setExpanding(true);
	tabWidget->tabBar()->setIconSize(QSize(30, 30));

	layout->addWidget(tabWidget);
	tabWidget->setCurrentWidget(tabs[Tab::Status]);
	tabWidget->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	this->setLayout(layout);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	tabWidget->resize(event->size());
	event->accept();
}

MainWindow::~MainWindow() {
	delete ui;
}
