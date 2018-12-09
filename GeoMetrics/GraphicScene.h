#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include "Entire.h"

class GraphicScene{

private:
	GraphicView* parent;

	Rect* boundingRect;
	bool rectFilled;

	qreal sceneWidth;
	qreal sceneHeight;

	QFont textFont;

	SelectionMode selMode;

	QList<Point2D> selPoints;
	//GraphicsGroup areas;

	QList<GraphicData*> objects;

	GLuint textures[1];

	GLuint mapDL;
	GLuint objsDL;

	bool selectionIsActive;

public:
	GraphicScene(GraphicView* parent = NULL);

	void loadTexture();
	void addItem(MapObject*);
	void addItem(GraphicData* data);
	void addItemText(QGraphicsItem*);

	//QList<GraphicObject*> getObjectsUnderPos(QPointF);
	//GraphicObject* getLastSelectedObject();

	void startSelection();
	void addSelectionPoint();
	void endSelection();

	void setLastSelectedObjects(GraphicObject*);

	Rect* getBoundingRect();

	void paintScene();
	void paintText();
	void paintSelectionFrame();

	void updateCallLists();

	void selectObjectsInsideFrameDinamicaly();
	void selectObjectsInsideFrame();
	void selectOne(QPointF);
	void selectOne(GraphicObject*, bool fromWidget = 0);
	void selectOneAndAddToList(QPointF, bool fromWidget = 0);
	void selectOneAndAddToList(GraphicObject*, bool fromWidget = 0);
	void selectOneOrUnselectOne(QPointF);
	void selectOneOrUnselectOne(GraphicObject* obj);
	void clearSelectedObjectsList(bool fromWidget = 0);
	void removeInstanceDataFromSelectedObjects();
	void dragMove(void* mimedata, QPointF point);
	void dropInstData(void* mimeData, QPointF point);
	void clearFrameSelectionFromEmptyElements();
	void selectAllFilledObjects();
	void clearAllFilledObjects();
	void clear();
	void clearScene();

};

#endif //GRAPHICSCENE_H