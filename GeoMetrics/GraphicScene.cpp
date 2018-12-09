#include "GraphicScene.h"
#include "GraphicView.h"
#include "GeoMetrics.h"
#include "GraphicObject.h"

GraphicScene::GraphicScene(GraphicView* _parent){
	boundingRect = new Rect( 0, 0, 0, 0 );
	parent = _parent;
	textFont = QFont("Courier", 10, 10, 1);

	rectFilled = 0;
	sceneWidth = 0;
	sceneHeight = 0;

	selMode = POLYGON_MODE;
	selPoints.clear();
}
void GraphicScene::loadTexture()
{
	QImage img;

	if (!img.load("Resources/Map.jpg")) {
	}

	QImage t = QGLWidget::convertToGLFormat(img);

	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	mapDL = glGenLists(1);
	glNewList(mapDL, GL_COMPILE);
	{
		glColor3f(1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-200.0f, 100.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-200.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(200.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(200.0f, 100.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glEndList();

	objsDL = glGenLists(1);

	boundingRect = new Rect(-200, -100, 200, 100);
	sceneWidth = 400;
	sceneHeight = 200;
}
void GraphicScene::addItem(MapObject* item){
	//if (rectFilled){
	//	if (boundingRect->bottomLeft.x > item->getRect()->bottomLeft.x)
	//		boundingRect->bottomLeft.x = item->getRect()->bottomLeft.x;
	//	if (boundingRect->topRight  .x < item->getRect()->topRight  .x)
	//		boundingRect->topRight  .x = item->getRect()->topRight  .x;
	//	if (boundingRect->bottomLeft.y > item->getRect()->bottomLeft.y)
	//		boundingRect->bottomLeft.y = item->getRect()->bottomLeft.y;
	//	if (boundingRect->topRight  .y < item->getRect()->topRight  .y)
	//		boundingRect->topRight  .y = item->getRect()->topRight  .y;
	//}
	//else{
	//	boundingRect->bottomLeft.x = item->getRect()->bottomLeft.x;
	//	boundingRect->topRight  .x = item->getRect()->topRight  .x;
	//	boundingRect->bottomLeft.y = item->getRect()->bottomLeft.y;
	//	boundingRect->topRight  .y = item->getRect()->topRight  .y;
	//	rectFilled = 1;
	//}
	//sceneWidth  = boundingRect->topRight.x - boundingRect->bottomLeft.x;
	//sceneHeight = boundingRect->topRight.y - boundingRect->bottomLeft.y;

	

	//objects.push_back(item);

}
void GraphicScene::addItem(GraphicData* data) {
	bool match = 0;
	foreach(GraphicData* obj, objects) {
		if (obj->name == data->name) {
			obj->setCenter(data->centerX, data->centerY);
			match = 1;
			break;
		}
	}
	if (!match) {
		objects.push_back(data);
	}

	//QString str;
	//
	//glNewList(objsDL, GL_COMPILE);
	//{
	//	foreach(GraphicData data, objects) {
	//		glColor3f(1, 1, 0);
	//		glBegin(GL_QUADS);
	//		{
	//			glVertex3f(data.bottomLeftX, data.bottomLeftY, 0);
	//			glVertex3f(data.topRightX,   data.bottomLeftY, 0);
	//			glVertex3f(data.topRightX,   data.topRightY, 0);
	//			glVertex3f(data.bottomLeftX, data.topRightY, 0);
	//		}
	//		glEnd();
	//
	//		glColor3f(0, 0, 0);
	//		glBegin(GL_LINE_LOOP);
	//		{
	//			glVertex3f(data.bottomLeftX, data.bottomLeftY, 0);
	//			glVertex3f(data.topRightX,   data.bottomLeftY, 0);
	//			glVertex3f(data.topRightX,   data.topRightY, 0);
	//			glVertex3f(data.bottomLeftX, data.topRightY, 0);
	//		}
	//		glEnd();
	//
	//		str += QString("Name=%1 -- Point1: (%2 , %3) --- Point2: (%4 , %5)\n")
	//			.arg(data.name)
	//			.arg(data.bottomLeftX)
	//			.arg(data.bottomLeftY)
	//			.arg(data.topRightX)
	//			.arg(data.topRightY);
	//	}
	//}
	//glEndList();

	//QString str = QString("Name=%1 -- Point1: (%2 , %3)\n")
	//	.arg(objects[0]->name)
	//	.arg(objects[0]->centerX)
	//	.arg(objects[0]->centerY);
	//
	//QMessageBox::information(NULL, "", str);
}
void GraphicScene::addItemText(QGraphicsItem* item){
	//groupText.push_back(item);
	//addItem(item);
}
void GraphicScene::paintScene(){

	glCallList(mapDL);
	//glCallList(objsDL);
	foreach(GraphicData* obj, objects) {
		obj->draw();
	}
}
void GraphicScene::paintText(){
	//foreach(GraphicObject* co, groupLE) parent->renderString(co->getRect()->bottomLeft.x(), co->getRect()->topRight.y(), co->getCellData()->name);
}
void GraphicScene::paintSelectionFrame(){
	Point2D bottomLeft = {0, 0};
	Point2D topRight = {0, 0};
	
	glColor3f(0.7, 0.7, 0.7);
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(bottomLeft.x, bottomLeft.y, 0.0);
		glVertex3f(bottomLeft.x, topRight.y  , 0.0);
		glVertex3f(topRight.x  , topRight.y  , 0.0);
		glVertex3f(topRight.x  , bottomLeft.y, 0.0);
	}
	glEnd();
}
void GraphicScene::updateCallLists(){
	//if (!cellTextDisplayListIndex) cellTextDisplayListIndex = glGenLists(1);
	//if (!instTextDisplayListIndex) instTextDisplayListIndex = glGenLists(1);

	//genChipCallList();
	//genFilledCallList();
}
void GraphicScene::setLastSelectedObjects(GraphicObject* obj){
	//lastSelectedObject = obj;
}
Rect* GraphicScene::getBoundingRect(){
	return boundingRect;
}
void GraphicScene::startSelection(){
	selectionIsActive = 1;
}
//void GraphicScene::addSelectionPoint(QPoint p) {
//
//}
void GraphicScene::endSelection(){
	selectionIsActive = 0;

	
	
	selPoints.clear();
}
void GraphicScene::selectObjectsInsideFrameDinamicaly(){
	////parent->getGui()->setSelectionFromDock(0);
	//
	//selectedObjects->clear();
	//
	//Point2D bottomLeft;
	//Point2D topRight;
	//
	//if (startSelectionPoint.x < endSelectionPoint.x){
	//	bottomLeft.x = startSelectionPoint.x;
	//	topRight.x = endSelectionPoint.x;
	//}
	//else{
	//	bottomLeft.x = endSelectionPoint.x;
	//	topRight.x = startSelectionPoint.x;
	//}
	//if (startSelectionPoint.y < endSelectionPoint.y){
	//	bottomLeft.y = startSelectionPoint.y;
	//	topRight.y = endSelectionPoint.y;
	//}
	//else{
	//	bottomLeft.y = endSelectionPoint.y;
	//	topRight.y = startSelectionPoint.y;
	//}
	//
	//if (chipVisible){
	//	foreach(GraphicsGroup* group, graphicGroups){
	//		foreach(GraphicObject* co, group->list()){
	//			bool xOk = 0;
	//			bool yOk = 0;
	//			Rect* rect = co->getBoundingRect();
	//			if (rect->getBottomLeft().x >= bottomLeft.x && rect->getTopRight().x <= topRight.x) xOk = 1;
	//			if (rect->getBottomLeft().y >= bottomLeft.y && rect->getTopRight().y <= topRight.y) yOk = 1;
	//
	//			if (xOk && yOk)
	//				selectedObjects->push_back(co);
	//		}
	//	}
	//}
	//else{
	//	foreach(GraphicObject* co, filledObjects->list()){
	//		bool xOk = 0;
	//		bool yOk = 0;
	//		Rect* rect = co->getBoundingRect();
	//		if (rect->getBottomLeft().x >= bottomLeft.x && rect->getTopRight().x <= topRight.x) xOk = 1;
	//		if (rect->getBottomLeft().y >= bottomLeft.y && rect->getTopRight().y <= topRight.y) yOk = 1;
	//
	//		if (xOk && yOk)
	//			selectedObjects->push_back(co);
	//	}
	//}
	//
	//foreach(GraphicObject* co, selectedObjects->list()){
	//	co->setSelection(1);
	//}
	//
	////parent->getGui()->setSelectionFromDock(1);
}
void GraphicScene::selectObjectsInsideFrame(){
	//parent->getGui()->setSelectionFromDock(0);

	//selectedObjects->clear();
	//
	//Point2D bottomLeft;
	//Point2D topRight;
	//
	//if (startSelectionPoint.x < endSelectionPoint.x){
	//	bottomLeft.x = startSelectionPoint.x;
	//	topRight.x = endSelectionPoint.x;
	//}
	//else{
	//	bottomLeft.x = endSelectionPoint.x;
	//	topRight.x = startSelectionPoint.x;
	//}
	//if (startSelectionPoint.y < endSelectionPoint.y){
	//	bottomLeft.y = startSelectionPoint.y;
	//	topRight.y = endSelectionPoint.y;
	//}
	//else{
	//	bottomLeft.y = endSelectionPoint.y;
	//	topRight.y = startSelectionPoint.y;
	//}
	//
	//
	//foreach(GraphicObject* co, filledObjects->list()){
	//	bool xOk = 0;
	//	bool yOk = 0;
	//	Rect* rect = co->getBoundingRect();
	//	if (rect->getBottomLeft().x >= bottomLeft.x && rect->getTopRight().x <= topRight.x) xOk = 1;
	//	if (rect->getBottomLeft().y >= bottomLeft.y && rect->getTopRight().y <= topRight.y) yOk = 1;
	//
	//	if (xOk && yOk)
	//		selectedObjects->push_back(co);
	//}
	//
	//foreach(GraphicObject* co, selectedObjects->list()){
	//	co->setSelection(1);
	//}
	//
	////parent->getGui()->setSelectionFromDock(1);
}
void GraphicScene::selectOne(QPointF p){
	//parent->getGui()->setSelectionFromDock(0);

	clearSelectedObjectsList();
	//QList<GraphicObject*> list = getObjectsUnderPos(p);
	//if (list.size()){
	//	GraphicObject* obj = list.front();
	//	selectOne(obj);
	//}

	//parent->getGui()->setSelectionFromDock(1);
}
void GraphicScene::selectOneAndAddToList(QPointF p, bool fromWidget){
	//QList<GraphicObject*> list = getObjectsUnderPos(p);
	//if (list.size()){
	//	GraphicObject* obj = list.front();
	//	selectOneAndAddToList(obj, fromWidget);
	//}
}
void GraphicScene::selectOne(GraphicObject* obj, bool fromWidget){
	obj->setSelection(1, fromWidget);
}
void GraphicScene::selectOneAndAddToList(GraphicObject* obj, bool fromWidget){
}
void GraphicScene::selectOneOrUnselectOne(QPointF p){
	//QList<GraphicObject*> list = getObjectsUnderPos(p);
	//if (list.size()){
	//	GraphicObject* obj = list.front();
	//	selectOneOrUnselectOne(obj);
	//}
}
void GraphicScene::selectOneOrUnselectOne(GraphicObject* obj){
	if (obj->isSelected()){
		obj->setSelection(0);
	}
	else{
		obj->setSelection(1);
	}
}
void GraphicScene::clearSelectedObjectsList(bool fromWidget){
	//foreach(GraphicObject* co, selectedObjects->list()){
	//	co->setSelection(0, fromWidget);
	//}
	//selectedObjects->clear();
}
void GraphicScene::removeInstanceDataFromSelectedObjects(){
	bool regenCallLists = 0;
	//foreach(GraphicObject* co, selectedObjects->list()){
	//
	//}
	if (regenCallLists)
		updateCallLists();
}
void GraphicScene::dragMove(void* mimedata, QPointF p){
	//const WidgetMimeData* md = (const WidgetMimeData*) mimedata;
	//clearSelectedObjectsList();
	//QList<GraphicObject*> list = getObjectsUnderPos(p);
	//if (list.size()){
	//	GraphicObject* obj = list.front();
	//	if (md->instData()->libData){
	//		if (md->instData()->libData->destCellType == obj->getCellData()->type){
	//			obj->setSelection(1);
	//			selectedObjects->push_back(obj);
	//		}
	//	}
	//}
}
void GraphicScene::dropInstData(void* mimedata, QPointF p){
	//if (!parent->isChipLoaded()){
	//	QMessageBox::critical(NULL, "Chip wasn't load.", "Chip wasn't load. Please load chip first.", QMessageBox::Ok);
	//	return;
	//}
	//
	//GraphicObject* obj				= NULL;
	//const WidgetMimeData* md	= (const WidgetMimeData*) mimedata;
	//InstanceData* instData		= md->instData();
	//GraphicObject* lastObj			= md->GraphicObject();
	//QList<GraphicObject*>objList	= getObjectsUnderPos(p);
	//
	//clearSelectedObjectsList();
	//
	//if (objList.size() == 0) return;
	//
	//obj = objList.front();
	//
	//if (!obj || obj == lastObj) return;
	//
	//if (!instData->libData) {
	//	QMessageBox::information(NULL, programData->getLocale()->messages.Warning, programData->getLocale()->messages.Warning_CantMapInstanceThatDoesNotHaveLibraryData);
	//	return;
	//}
	//
	//if (!obj->isEditable()) {
	//	QMessageBox::information(NULL, programData->getLocale()->messages.Warning, programData->getLocale()->messages.Warning_CantMapInstanceCellIsNotEditable);
	//	return;
	//}
	//
	//if (obj->getCellData()->type != instData->libData->destCellType){
	//	QMessageBox::information(NULL, programData->getLocale()->messages.Warning, programData->getLocale()->messages.Warning_CantMapInstanceWithType1ToCellWithType2.arg(instData->libData->destCellType).arg(obj->getCellData()->type));
	//	return;
	//}
	//
	//if (obj->getInstData()){
	//	int ans = QMessageBox::question(NULL, programData->getLocale()->messages.Question, programData->getLocale()->messages.Question_Cell1AlreadyHasInstance2ReplaceItWith3.arg(obj->getCellData()->name).arg(obj->getInstData()->name).arg(instData->name), QMessageBox::Yes | QMessageBox::No);
	//	if (ans == QMessageBox::Yes){
	//		if (lastObj) 
	//			unmapInst(lastObj);
	//		unmapInst(obj);
	//		mapInst(obj, instData);
	//		updateCallLists();
	//		((Placement_GUI*)parent->parent())->showMessageOnStatusBar(programData->getLocale()->messages.Info_Instance1HasBeenMappedTo2.arg(obj->getInstData()->name).arg(obj->getCellData()->name));
	//	}
	//}
	//else{
	//	if (lastObj) 
	//		unmapInst(lastObj);
	//	mapInst(obj, instData);
	//	updateCallLists();
	//	((Placement_GUI*)parent->parent())->showMessageOnStatusBar(programData->getLocale()->messages.Info_Instance1HasBeenMappedTo2.arg(obj->getInstData()->name).arg(obj->getCellData()->name));
	//}
	//
	//obj->setSelection(1);
	//selectedObjects->push_back(obj);
}
void GraphicScene::clearFrameSelectionFromEmptyElements(){
	//foreach(GraphicObject* co, selectedObjects->list()){
	//	
	//}
}
void GraphicScene::selectAllFilledObjects(){
	//selectedObjects->clear();
	//foreach(GraphicObject* co, filledObjects->list()){
	//	selectedObjects->push_back(co);
	//	co->setSelection(1);
	//}
}
void GraphicScene::clearAllFilledObjects(){
	selectAllFilledObjects();
	removeInstanceDataFromSelectedObjects();
}
void GraphicScene::clear(){
	boundingRect = new Rect(0, 0, 0, 0);
	rectFilled = 0;
	sceneWidth = 0;
	sceneHeight = 0;

	//foreach(GraphicsGroup* group, graphicGroups){
	//	group->clearAll();
	//	delete group;
	//}
	//graphicGroups.clear();
	//
	//selectedObjects->clearAll();
	//filledObjects->clearAll();
	//
	//startSelectionPoint = Point2D();
	//endSelectionPoint = Point2D();
}
void GraphicScene::clearScene(){

}
