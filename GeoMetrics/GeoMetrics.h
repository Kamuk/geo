#pragma once

#include "Entire.h"

class GeoMetrics : public QMainWindow
{
	Q_OBJECT

	GraphicView* glWid = Q_NULLPTR;

public:
	GeoMetrics(QWidget *parent = Q_NULLPTR);

	void init();

	void initCentralWidget();

	void initMySql();
};
