#include "GeoMetrics.h"
#include "GraphicView.h"
#include "GraphicObject.h"

GeoMetrics::GeoMetrics(QWidget *parent)
	: QMainWindow(parent)
{
	init();
}

void GeoMetrics::init() {
	initCentralWidget();

	initMySql();
}

void GeoMetrics::initCentralWidget() {
	glWid = new GraphicView();
	setCentralWidget(glWid);
}

void GeoMetrics::initMySql() {
	/*QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	bool a = QSqlDatabase::isDriverAvailable("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("geo");
	db.setUserName("root");
	db.setPassword("password");
	
	bool ok = db.open();

	if (!ok)
		QMessageBox::critical(this, "Error", db.lastError().text());

	QSqlQuery query;
	query.exec("SELECT id, current_point_x, current_point_y FROM trackers");
	QString str;
	while (query.next()) {
		str += QString("%1 --- %2 --- %3\n")
			.arg(query.value(0).toInt())
			.arg(query.value(1).toFloat())
			.arg(query.value(1).toFloat());
	}

	QMessageBox::information(this, "", str);

	db.close();
	*/
	

	

	//QMessageBox::information(this, "", str);



	//########################################//

	//mysql_close(conn);
}
