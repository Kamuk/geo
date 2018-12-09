#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include <QtSql>
#include <mysql.h>
//Flags
#define YES 1
#define NO 0

#define EXIT_OK 0
#define EXIT_ERROR 1

enum SelectionMode { NONE_SELECTION, RECTANGLE_MODE, CIRCLE_MODE, POLYGON_MODE };

struct Point2D {
	float x;
	float y;
};

class GeoMetrics;
class GraphicView;
class GraphicScene;
class Rect;
class GraphicObject;
class GraphicsGroup;
class MapObject;
class GraphicData;

static void setGlColor(QColor col) {
	glColor3f(col.redF(), col.greenF(), col.blueF());
}
static void setGlColor(float r, float g, float b) {
	glColor3f(r, g, b);
}
static void setGlColor(int r, int g, int b) {
	glColor3d(r, g, b);
}