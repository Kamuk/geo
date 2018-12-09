#pragma once

#include "Entire.h"

enum PaintMode { NO_MODE = -1, DEFAULT_PAINT, SELF_PAINT, SELECT_PAINT };

class Rect{

public:

	Point2D bottomLeft;
	Point2D topRight;
	Point2D center;

public:
	Rect();
	Rect(QPointF _bottomLeft, QPointF _topRight);
	Rect(Point2D _bottomLeft, Point2D _topRight);
	Rect(float x1, float y1, float x2, float y2);

	void setBottomLeft(QPointF point) { bottomLeft.x = point.x(); bottomLeft.y = point.y(); calculateCenter(); };
	void setTopRight(QPointF point) { topRight.x = point.x(); topRight.y = point.y(); calculateCenter(); };
	void setBottomLeft(Point2D point) { bottomLeft.x = point.x; bottomLeft.y = point.y; calculateCenter(); };
	void setTopRight(Point2D point) { topRight.x = point.x; topRight.y = point.y; calculateCenter(); };

	void setBottomLeft(float x, float y) { bottomLeft.x = x; bottomLeft.y = y; calculateCenter(); };
	void setTopRight(float x, float y) { topRight.x = x; topRight.y = y; calculateCenter(); };

	Point2D getBottomLeft() { return bottomLeft; };
	Point2D getTopRight() { return topRight; };
	Point2D getCenter() { return center; }

	void calculateCenter();

};

class GraphicData {
public:
	QString type;
	QString name;

	float bottomLeftX;
	float bottomLeftY;
	float topRightX;
	float topRightY;

	float centerX;
	float centerY;

	float width;
	float heigh;

public:
	GraphicData() {
		type = "";
		bottomLeftX = 0;
		bottomLeftY = 0;
		topRightX = 0;
		topRightY = 0;
		centerX = 0;
		centerY = 0;

		width = 0;
		heigh = 0;

		name = "";
	}

	GraphicData(QString Oname, QString Otype, float cX, float cY, float w, float h) {
		type = Otype;
		name = Oname;

		bottomLeftX = cX - w / 2;
		bottomLeftY = cY - h / 2;

		topRightX = cX + w / 2;
		topRightY = cY + h / 2;

		centerX = cX;
		centerY = cY;

		width = w;
		heigh = h;
	}

	void draw() {
		glColor3f(1, 0, 0);
		glLineWidth(2);
		glTranslatef(centerX, centerY, 0);

		glBegin(GL_QUADS);
		{
			glVertex3f(-width / 2, -heigh / 2, 0);
			glVertex3f( width / 2, -heigh / 2, 0);
			glVertex3f( width / 2,  heigh / 2, 0);
			glVertex3f(-width / 2,  heigh / 2, 0);
		}
		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(-width / 2, -heigh / 2, 0);
			glVertex3f( width / 2, -heigh / 2, 0);
			glVertex3f( width / 2,  heigh / 2, 0);
			glVertex3f(-width / 2,  heigh / 2, 0);
		}
		glEnd();

		glTranslatef(-centerX, -centerY, 0);
	}

	void move(float x, float y) {
		centerX += x;
		centerY += y;
	}

	void setCenter(float x, float y) {
		centerX = x;
		centerY = y;
	}

};

class GraphicObject
{
public:
	QColor frameSelectedColor;
	QColor frameUnselectedColor;
	QColor bodyEmptyColor;
	QColor bodyFilledColor;

private:
	GraphicScene* parent;

	Rect* rect;
	Rect* boundingRect;

	bool selected;

	GraphicData* data;

	int frameWidth;

	bool notEditable;

public:
	GraphicObject();
	GraphicObject(Rect* rect, int frameWidth = 0, GraphicData* data = NULL);
	GraphicObject(GraphicObject*);
	~GraphicObject() {};

	void setFrameUnselectedColor(QColor&);
	void setBodyEmptyColor(QColor &col);
	void setFrameSelectedColor(QColor &col);
	void setBodyFilledColor(QColor &col);

	void setSelection(bool selected, bool fromWidget = 0);
	void setNotEditable(bool);
	GraphicData* getGraphicData();

	void paint(PaintMode mode);
	void paintFrame();
	void paintBody();
	Rect* getBoundingRect();
	Rect* getRect();
	bool isSelected();
	bool isEditable();
};

class GraphicsGroup {
private: //Data:
	QString _type;
	int _displayListIndex;
	QList<GraphicObject*> _list;
	PaintMode _paintMode;

public: //Constructors:
	GraphicsGroup();
	GraphicsGroup(GraphicsGroup*);
	GraphicsGroup(QList<GraphicObject*> list, PaintMode pType = DEFAULT_PAINT);

public: //Public interface:
	void clear(); //clear list
	void clearAll(); //clear all data

	void genDisplayList(); //generate OpenGL display list for group

	void removeOne(GraphicObject*); //remove one GraphicObject* from list
	void removeLast(); //remove last GraphicObject* from list
	void removeFirst(); //remove first GraphicObject* from list
	void removeAt(int id); //remove GraphicObject* at index from list

	int size();

	GraphicObject* front();
	GraphicObject* back();
	GraphicObject* at(int id);

	void push_back(GraphicObject*); //push GraphicObject* in end of list;
	void push_front(GraphicObject*); //push GraphicObject* in begining of list;
	GraphicObject* pop_back(); //pop GraphicObject* from end;
	GraphicObject* pop_front(); //pop GraphicObject* from begining;

	QString type();
	void setType(QString);
	QList<GraphicObject*> list();

	void setPaintMode(int mode);

	void paint();
	void directPaint(int paintMode = NO_MODE);
	void directPaint(QColor bodyColor, QColor frameColor);
};

class MapObject : GraphicObject {

	MapObject(Rect* rect, GraphicData* data = NULL) : GraphicObject(rect, 3, data) {

	};

};
