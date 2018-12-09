#ifndef GRAPHIC_VIEW_H
#define GRAPHIC_VIEW_H

#include "Entire.h"

class GraphicView : public QGLWidget
{
	Q_OBJECT

private:

	MYSQL *conn;
	MYSQL_RES *res;

	float viewScale;
	float winWidth;
	float winHeight;

	float scaleValue;

	float translateX;
	float translateY;

	float pixelStepX;
	float pixelStepY;

	GraphicScene* p_scene;

	float maxX, minX;
	float maxY, minY;

	bool maxAndMinIsInitialized;

	float scaleFactor;

	Rect* viewportRect;

	QPoint dragPoint;
	QPoint translatePoint;

	bool multiselectionActivated;

	bool shiftKeyState;
	bool ctrlKeyState;

	bool dinamicSelectionByFrame;

public:
	GraphicView(QWidget* parent = NULL);
	~GraphicView(){};

public:
	GraphicScene* getScene();

	void updateView();
	void clear();
	void clearSceneFromCircuit();

	void addGraphicObject(GraphicData* obj);
	void addGraphicObjectList(QList<GraphicData*> objects);

	void focusOnPoint(Point2D);
	
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);

	bool eventFilter(QObject*, QEvent*);
	void wheelEvent(QWheelEvent*);
	void dropEvent(QDropEvent*);
	void dragEnterEvent(QDragEnterEvent*);
	void dragMoveEvent(QDragMoveEvent*);

private:
	void init();
	void initScene();

	void startDrag();

	void translate(float x, float y, bool withoutCheck = 0);
	void scale(float factor);
	QPointF fromWindowToSceneCoordinates(QPoint);
	void scaleWithMappingToMouse(QPoint, float);

public slots:
	void updateAll();

};

#endif //GRAPHIC_VIEW_H