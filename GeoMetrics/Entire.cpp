#include "Entire.h"
#include "GraphicObject.h"

Point2D calculateCenter(QList<GraphicObject*> list){
	Point2D center;
	Point2D rectCenter;
	bool first = 1;
	center.x = 0.0;
	center.y = 0.0;

	foreach(GraphicObject* co, list){
		rectCenter = co->getRect()->getCenter();
		if (first) { center = rectCenter; first = 0; continue; }

		center.x += co->getRect()->getCenter().x;
		center.y += co->getRect()->getCenter().y;

	}
	center.x = center.x / list.size();
	center.y = center.y / list.size();

	return center;
}
Point2D calculateCenter(GraphicObject* object1, GraphicObject* object2){
	Point2D center;

	center.x = (object1->getRect()->getCenter().x + object2->getRect()->getCenter().x)/2;
	center.y = (object1->getRect()->getCenter().y + object2->getRect()->getCenter().y)/2;

	return center;
}
