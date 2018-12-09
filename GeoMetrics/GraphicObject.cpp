#include "GraphicObject.h"
#include "GraphicView.h"
#include "GraphicScene.h"

Rect::Rect(){
	bottomLeft.x = 0;
	bottomLeft.y = 0;
	topRight.x = 0;
	topRight.y = 0;
	center.x = 0;
	center.y = 0;
}
Rect::Rect(QPointF _bottomLeft, QPointF _topRight){

	if (_bottomLeft.x() > _topRight.x()){
		bottomLeft.x = _topRight.x();
		topRight.x = _bottomLeft.x();
	}
	else{
		bottomLeft.x = _bottomLeft.x();
		topRight.x = _topRight.x();
	}

	if (_bottomLeft.y() > _topRight.y()){
		bottomLeft.y = _topRight.y();
		topRight.y = _bottomLeft.y();
	}
	else{
		bottomLeft.y = _bottomLeft.y();
		topRight.y = _topRight.y();
	}

	calculateCenter();
}
Rect::Rect(Point2D _bottomLeft, Point2D _topRight){

	if (_bottomLeft.x > _topRight.x){
		bottomLeft.x = _topRight.x;
		topRight.x = _bottomLeft.x;
	}
	else{
		bottomLeft.x = _bottomLeft.x;
		topRight.x = _topRight.x;
	}

	if (_bottomLeft.y > _topRight.y){
		bottomLeft.y = _topRight.y;
		topRight.y = _bottomLeft.y;
	}
	else{
		bottomLeft.y = _bottomLeft.y;
		topRight.y = _topRight.y;
	}

	calculateCenter();
}
Rect::Rect(float x1, float y1, float x2, float y2){
	if (x1 > x2){
		bottomLeft.x =(x2);
		topRight.x = (x1);
	}
	else{
		bottomLeft.x = (x1);
		topRight.x = (x2);
	}

	if (y1 > y2){
		bottomLeft.y = (y2);
		topRight.y = (y1);
	}
	else{
		bottomLeft.y = (y1);
		topRight.y = (y2);
	}


	calculateCenter();
}
void Rect::calculateCenter(){
	center.x = bottomLeft.x + (topRight.x - bottomLeft.x)*0.5;
	center.y = bottomLeft.y + (topRight.y - bottomLeft.y)*0.5;
}


GraphicObject::GraphicObject(){
	frameSelectedColor = QColor(0,0,0);
	frameUnselectedColor = QColor(0, 0, 0);
	bodyEmptyColor = QColor(0, 0, 0);
	bodyFilledColor = QColor(0, 0, 0);

	rect = NULL;
	boundingRect = NULL;

	selected = 0;

	data = NULL;

	frameWidth = 1;

	notEditable = 0;
}
GraphicObject::GraphicObject(Rect* _rect, int _frameWidth, GraphicData* _data) {
	rect = _rect;
	boundingRect = _rect;
	frameWidth = _frameWidth;
	frameUnselectedColor = QColor(200, 0, 0);
	frameSelectedColor = QColor(0, 255, 0);
	bodyEmptyColor = QColor(64, 64, 64);
	bodyFilledColor = QColor(200, 128, 0);
	data = _data;
	selected = 0;
	notEditable = 0;
}
GraphicObject::GraphicObject(GraphicObject* obj){
	rect = new Rect(obj->getRect()->getBottomLeft(), obj->getRect()->getTopRight());
	boundingRect = new Rect(obj->getBoundingRect()->getBottomLeft(), obj->getBoundingRect()->getTopRight());
	
	data = new GraphicData;
	data->type = obj->getGraphicData()->type;
	//data->x1 = obj->getGraphicData()->x1;
	//data->x2 = obj->getGraphicData()->x2;
	//data->y1 = obj->getGraphicData()->y1;
	//data->y2 = obj->getGraphicData()->y2;
	//data->x = obj->getGraphicData()->x;
	//data->y = obj->getGraphicData()->y;
	data->name = obj->getGraphicData()->name;

	selected = obj->selected;
	frameWidth = obj->frameWidth;

	frameUnselectedColor = obj->frameUnselectedColor;
	frameSelectedColor	 = obj->frameSelectedColor	;
	bodyEmptyColor		 = obj->bodyEmptyColor		;
	bodyFilledColor		 = obj->bodyFilledColor		;

	notEditable = 0;

}
void GraphicObject::setFrameUnselectedColor(QColor &col){
	frameUnselectedColor = col;
}
void GraphicObject::setBodyEmptyColor(QColor &col){
	bodyEmptyColor = col;
}
void GraphicObject::setFrameSelectedColor(QColor &col){
	frameSelectedColor = col;
}
void GraphicObject::setBodyFilledColor(QColor &col){
	bodyFilledColor = col;
}
void GraphicObject::setSelection(bool selected, bool fromWidget){
	this->selected = selected;
}
void GraphicObject::setNotEditable(bool val){
	notEditable = val;
}
GraphicData* GraphicObject::getGraphicData(){
	return data;
}
void GraphicObject::paint(PaintMode type){
	switch (type){
	case DEFAULT_PAINT:{
		setGlColor(bodyEmptyColor);
		paintBody();
		setGlColor(frameUnselectedColor);
		paintFrame();
		break;
	}
	case SELF_PAINT:{
		paintBody();
		selected ? setGlColor(frameSelectedColor) : setGlColor(frameUnselectedColor);
		paintFrame();
		break;
	}
	default: break;
	}
	
}
void GraphicObject::paintFrame(){
	glLineWidth(frameWidth);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(rect->getBottomLeft().x, rect->getBottomLeft().y, 0.0);
		glVertex3f(rect->getBottomLeft().x, rect->getTopRight().y  , 0.0);
		glVertex3f(rect->getTopRight().x  , rect->getTopRight().y  , 0.0);
		glVertex3f(rect->getTopRight().x  , rect->getBottomLeft().y, 0.0);
	}
	glEnd();
}
void GraphicObject::paintBody(){
	glBegin(GL_QUADS);
	{
		glVertex3f(rect->getBottomLeft().x, rect->getBottomLeft().y, 0.0);
		glVertex3f(rect->getBottomLeft().x, rect->getTopRight().y  , 0.0);
		glVertex3f(rect->getTopRight().x  , rect->getTopRight().y  , 0.0);
		glVertex3f(rect->getTopRight().x  , rect->getBottomLeft().y, 0.0);
	}
	glEnd();
}
Rect* GraphicObject::getBoundingRect(){
	return boundingRect;
}
Rect* GraphicObject::getRect(){
	return rect;
}
bool GraphicObject::isSelected(){
	return selected;
}
bool GraphicObject::isEditable(){
	return !notEditable;
}


GraphicsGroup::GraphicsGroup(){
	_type = "";
	_displayListIndex = 0;
	_list.clear();
	_paintMode = DEFAULT_PAINT;
}
GraphicsGroup::GraphicsGroup(GraphicsGroup* group){
	_type = group->_type;
	_paintMode = group->_paintMode;
	QList<GraphicObject*> l = group->_list;
	foreach(GraphicObject* obj, l){
		_list.push_back(new GraphicObject(obj));
	}
	_displayListIndex = 0;
	genDisplayList();
}
GraphicsGroup::GraphicsGroup(QList<GraphicObject*> list, PaintMode pType){
	_type = list.front()->getGraphicData()->type;
	_paintMode = pType;
	foreach(GraphicObject* obj, list){
		_list.push_back(new GraphicObject(obj));
	}
	_displayListIndex = 0;
	genDisplayList();
}
void GraphicsGroup::clear(){
	_list.clear();
	glDeleteLists(_displayListIndex, 1);
	_displayListIndex = 0;
}
void GraphicsGroup::clearAll(){
	_type = "";
	//foreach(GraphicObject* co, _list){
	//	co->clearInstData();
	//}
	_list.clear();
	glDeleteLists(_displayListIndex, 1);
	_displayListIndex = 0;
}
void GraphicsGroup::genDisplayList(){
	if (!_displayListIndex) _displayListIndex = glGenLists(1);
	if (_displayListIndex){
		glNewList(_displayListIndex, GL_COMPILE);
		{
			if (_paintMode == DEFAULT_PAINT){
				QColor c_body(_list.front()->bodyEmptyColor);
				QColor c_frame(_list.front()->frameUnselectedColor);
				setGlColor(c_body);
				foreach(GraphicObject* obj, _list) obj->paintBody();
				setGlColor(c_frame);
				foreach(GraphicObject* obj, _list) obj->paintFrame();
			}
			if (_paintMode == SELF_PAINT){
				foreach(GraphicObject* obj, _list) obj->paint(_paintMode);
			}
		}
		glEndList();
	}
}
void GraphicsGroup::removeOne(GraphicObject* obj){
	_list.removeOne(obj);
}
void GraphicsGroup::removeLast(){
	_list.removeLast();
}
void GraphicsGroup::removeFirst(){
	_list.removeFirst();
}
void GraphicsGroup::removeAt(int id){
	_list.removeAt(id);
}
int GraphicsGroup::size(){
	return _list.size();
}
GraphicObject* GraphicsGroup::front(){
	return _list.front();
}
GraphicObject* GraphicsGroup::back(){
	return _list.back();
}
GraphicObject* GraphicsGroup::at(int id){
	return _list.at(id);
}
void GraphicsGroup::push_back(GraphicObject* obj){
	_list.push_back(obj);
}
void GraphicsGroup::push_front(GraphicObject* obj){
	_list.push_front(obj);
}
GraphicObject* GraphicsGroup::pop_back(){
	GraphicObject* obj = _list.back();
	_list.pop_back();
	return obj;
}
GraphicObject* GraphicsGroup::pop_front(){
	GraphicObject* obj = _list.front();
	_list.pop_front();
	return obj;
}
QString GraphicsGroup::type(){
	return _type;
}
void GraphicsGroup::paint(){
	glCallList(_displayListIndex);
}
void GraphicsGroup::directPaint(int paintMode){
	if (paintMode == NO_MODE){
		foreach(GraphicObject* obj, _list) obj->paint(_paintMode);
	}
	else{
		switch (_paintMode){
		case DEFAULT_PAINT:{
			QColor c_body(_list.front()->bodyEmptyColor);
			QColor c_frame(_list.front()->frameUnselectedColor);
			setGlColor(c_body);
			foreach(GraphicObject* obj, _list) obj->paintBody();
			setGlColor(c_frame);
			foreach(GraphicObject* obj, _list) obj->paintFrame();
			break;
		}
		case SELF_PAINT:{
			foreach(GraphicObject* obj, _list) obj->paint(SELF_PAINT);
			break;
		}
		case SELECT_PAINT:{
			QColor c_frame(_list.front()->frameSelectedColor);
			setGlColor(c_frame);
			foreach(GraphicObject* obj, _list) obj->paintFrame();
			break;
		}
		default: break;
		}
	}
}
void GraphicsGroup::directPaint(QColor bodyColor, QColor frameColor){
	setGlColor(bodyColor);
	foreach(GraphicObject* obj, _list) obj->paintBody();
	setGlColor(frameColor);
	foreach(GraphicObject* obj, _list) obj->paintFrame();
}
void GraphicsGroup::setType(QString type){
	_type = type;
}
void GraphicsGroup::setPaintMode(int mode){
	_paintMode = (PaintMode) mode;
}
QList<GraphicObject*> GraphicsGroup::list(){
	return _list;
}