#include "Entire.h"
#include "GeoMetrics.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GeoMetrics w;
	w.show();
	return a.exec();
}
