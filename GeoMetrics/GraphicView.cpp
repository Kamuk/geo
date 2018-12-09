#include "GraphicView.h"
#include "GeoMetrics.h"
#include "GraphicScene.h"
#include "GraphicObject.h"

GraphicView::GraphicView(QWidget* parent) : QGLWidget(parent){
	winWidth = 0;
	winHeight = 0;
	p_scene = NULL;
	scaleFactor = 1.15;
	dinamicSelectionByFrame = 0;

	clear();
	init();
	installEventFilter(this);
	setToolTipDuration(1000);
	setMouseTracking(1);
}
void GraphicView::init(){
	initScene();
	setAcceptDrops(true);
	viewportRect = new Rect(0.0, 0.0, 0.0, 0.0);

	QTime midnight(0, 0, 0);
	qsrand(midnight.secsTo(QTime::currentTime()));
	
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateAll()));
	timer->start(10);

	//timer->start(100);

	conn = mysql_init(NULL);
	if (conn == NULL) {
		QMessageBox::information(this, "", "Error: can'tcreate MySQL-descriptor\n");
	}
	if (!mysql_real_connect(conn, "localhost", "root", "password", "geo", NULL, NULL, 0)) {
		QMessageBox::information(this, "Error: can't connect to database %1\n", QString(mysql_error(conn)));
	}
	else {
		//QMessageBox::information(this, "", "Success!\n");
	}

}
void GraphicView::initScene(){
	p_scene = new GraphicScene(this);
}
GraphicScene* GraphicView::getScene(){
	return p_scene;
}
void GraphicView::updateView(){
	resizeGL(winWidth, winHeight);
	updateGL();
}
void GraphicView::clear(){
	scaleValue = 1.0;

	translateX = 0.0;
	translateY = 0.0;

	pixelStepX = 0.0;
	pixelStepY = 0.0;

	maxX = 0;
	minX = 0;
	maxY = 0;
	minY = 0;

	maxAndMinIsInitialized = 0;

	viewportRect = new Rect(0, 0, 0, 0);

	multiselectionActivated = 0;

	shiftKeyState = 0;
	ctrlKeyState = 0;

	if (p_scene)
		p_scene->clear();
}
void GraphicView::clearSceneFromCircuit(){
	p_scene->clearScene();
	updateGL();
}

void GraphicView::initializeGL(){
	qglClearColor(Qt::black);
	p_scene->loadTexture();
}
void GraphicView::resizeGL(int w, int h){
	winWidth = w;
	winHeight = h;

	if (h == 0) {
		h = 1;
	}

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

	if (w <= h) {
		float right = (0.5 / scaleValue);
		float left = -right;
		float top = 0.5 / scaleValue / aspectRatio;
		float bottom = -top;
		pixelStepX = (right - left) / w;
		pixelStepY = (top - bottom) / h;
		viewportRect->setBottomLeft(left + translateX, bottom + translateY);
		viewportRect->setTopRight(right + translateX, top + translateY);
		glOrtho(
			viewportRect->bottomLeft.x, viewportRect->topRight.x, 
			viewportRect->bottomLeft.y, viewportRect->topRight.y,
			1.0								 , -1.0
		);
	}
	else {
		float right = 0.5 / scaleValue * aspectRatio;
		float left = -right;
		float top = 0.5 / scaleValue;
		float bottom = -top;
		pixelStepX = (right - left) / w;
		pixelStepY = (top - bottom) / h;
		viewportRect->setBottomLeft(left + translateX, bottom + translateY);
		viewportRect->setTopRight(right + translateX, top + translateY);
		glOrtho(
			viewportRect->bottomLeft.x, viewportRect->topRight.x, 
			viewportRect->bottomLeft.y, viewportRect->topRight.y,
			1.0								 , -1.0
		);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void GraphicView::paintGL(){
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	

	p_scene->paintScene();

}

void GraphicView::translate(float x, float y, bool withoutCheck){
	if (!withoutCheck){
		float cx = viewportRect->center.x;
		float cy = viewportRect->center.y;

		cx += x;
		cy += y;
		if (cx >= p_scene->getBoundingRect()->bottomLeft.x
			&&
			cx <= p_scene->getBoundingRect()->topRight.x
			){
			translateX += x;
		}
		if (cy >= p_scene->getBoundingRect()->bottomLeft.y
			&&
			cy <= p_scene->getBoundingRect()->topRight.y
			){
			translateY += y;
		}
	}
	else{
		translateX += x;
		translateY += y;
	}
	updateView();
}
void GraphicView::scale(float factor){
	scaleValue *= factor;
	updateView();
}
void GraphicView::scaleWithMappingToMouse(QPoint p, float factor){
	Rect lastViewportect(viewportRect->bottomLeft, viewportRect->topRight);
	scaleValue *= factor;
	resizeGL(winWidth, winHeight);
	float x = 0.0;
	float y = 0.0;
	if (p.x() > winWidth * 0.5){
		x = ((lastViewportect.topRight.x - lastViewportect.bottomLeft.x)
			- (viewportRect->topRight.x - viewportRect->bottomLeft.x))
			* 0.5;
		x *= (p.x() - (winWidth*0.5)) / (winWidth*0.5);
	}
	if (p.x() < winWidth * 0.5){
		x = ((lastViewportect.topRight.x - lastViewportect.bottomLeft.x)
			- (viewportRect->topRight.x - viewportRect->bottomLeft.x))
			* 0.5;
		x *= -((winWidth*0.5) - p.x()) / (winWidth*0.5);
	}
	if (p.y() > winHeight * 0.5){
		y = ((lastViewportect.topRight.y - lastViewportect.bottomLeft.y)
			- (viewportRect->topRight.y - viewportRect->bottomLeft.y))
			* 0.5;
		y *= -(p.y() - (winHeight*0.5)) / (winHeight*0.5);
	}
	if (p.y() < winHeight * 0.5){
		y = ((lastViewportect.topRight.y - lastViewportect.bottomLeft.y)
			- (viewportRect->topRight.y - viewportRect->bottomLeft.y))
			* 0.5;
		y *= ((winHeight*0.5) - p.y()) / (winHeight*0.5);
	}
	translate(x, y);
}
QPointF GraphicView::fromWindowToSceneCoordinates(QPoint p){
	QPointF out;
	float x = p.x();
	float y = winHeight - p.y();

	x *= pixelStepX;
	y *= pixelStepY;

	x += viewportRect->bottomLeft.x;
	y += viewportRect->bottomLeft.y;

	out.setX(x);
	out.setY(y);

	return out;
}

void GraphicView::startDrag(){
}
bool GraphicView::eventFilter(QObject* obj, QEvent* ev){
	switch (ev->type()){
	case QEvent::MouseMove:{
		QMouseEvent* me = (QMouseEvent*)ev;
		//p_scene->getObjectsUnderPos(fromWindowToSceneCoordinates(me->pos()));
		
		//QToolTip::showText(me->globalPos(), "AAAAAA");
		
		break;
	}
	case QEvent::KeyPress:{
		QKeyEvent* ke = (QKeyEvent*)ev;
		if (ke->key() == Qt::Key_Delete){
			p_scene->removeInstanceDataFromSelectedObjects();
			p_scene->clearSelectedObjectsList();
		}
		if (ke->key() == Qt::Key_Shift){
			shiftKeyState = 1;
		}
		if (ke->key() == Qt::Key_Control){
			ctrlKeyState = 1;
		}
		break;
	}
	case QEvent::KeyRelease:{
		QKeyEvent* ke = (QKeyEvent*)ev;
		if (ke->key() == Qt::Key_Shift){
			shiftKeyState = 0;
		}
		if (ke->key() == Qt::Key_Control){
			ctrlKeyState = 0;
		}
		break;
	}
	case QEvent::DragEnter:{
		QDragEnterEvent* de = (QDragEnterEvent*)ev;

		break;
	}
	case QEvent::Drop:{
		QDropEvent* de = (QDropEvent*)ev;
		
		break;
	}
	case QEvent::ToolTip:{
		QHelpEvent* he = (QHelpEvent*) ev;
		break;
	}
	default: break;
	}
	updateGL();
	return QGLWidget::eventFilter(obj, ev);
}
void GraphicView::wheelEvent(QWheelEvent* we){
	if (we->delta() < 0){
		scaleWithMappingToMouse(we->pos(), 1.0 / scaleFactor);
	}
	if (we->delta() > 0){
		scaleWithMappingToMouse(we->pos(), scaleFactor);
	}
}
void GraphicView::mousePressEvent(QMouseEvent* me){
	if (me->button() == Qt::RightButton){

	}
	if (me->buttons() & Qt::LeftButton){
		//dragPoint = me->pos();
		//if (!ctrlKeyState) p_scene->selectOne(fromWindowToSceneCoordinates(me->pos()));
		//else p_scene->selectOneOrUnselectOne(fromWindowToSceneCoordinates(me->pos()));
		//if (p_scene->getLastSelectedObject()){
		//
		//}
		//else{
		//	multiselectionActivated = 1;
		//	p_scene->setMultiselection(multiselectionActivated);
		//	p_scene->setStartSelectionPoint(fromWindowToSceneCoordinates(me->pos()));
		//	p_scene->setEndSelectionPoint(fromWindowToSceneCoordinates(me->pos()));
		//}
	}
	if (me->buttons() & Qt::MiddleButton){
		translatePoint = me->pos();
	}
	setFocus();
	update();
}
void GraphicView::mouseDoubleClickEvent(QMouseEvent* me){
	if (me->button() == Qt::RightButton){

	}
	if (me->button() == Qt::LeftButton){
		//if (p_scene->getLastSelectedObject()){
		//}
	}
}
void GraphicView::mouseReleaseEvent(QMouseEvent* me){
	if (me->button() == Qt::RightButton){

	}
	if (me->button() == Qt::LeftButton){
		//if (multiselectionActivated){
		//	if (!dinamicSelectionByFrame) p_scene->selectObjectsInsideFrame();
		//	multiselectionActivated = 0;
		//	p_scene->setMultiselection(multiselectionActivated);
		//	if (dinamicSelectionByFrame) p_scene->clearFrameSelectionFromEmptyElements();
		//}
	}
	updateGL();
}
void GraphicView::mouseMoveEvent(QMouseEvent* me){
	if (me->buttons() & Qt::RightButton){
	
	}
	if (me->buttons() & Qt::LeftButton){
		//if (multiselectionActivated){
		//	p_scene->setEndSelectionPoint(fromWindowToSceneCoordinates(me->pos()));
		//	//if(dinamicSelectionByFrame) p_scene->selectObjectsInsideFrameDinamicaly();
		//}
		//else{
		//	if (p_scene->getLastSelectedObject()){
		//		
		//	}
		//}
	}
	if (me->buttons() & Qt::MiddleButton){
		float x = (translatePoint - me->pos()).x() * pixelStepX;
		float y = (translatePoint - me->pos()).y() * pixelStepY;
		translate(x, -y);
		translatePoint = me->pos();
	}

	updateGL();
}
void GraphicView::dropEvent(QDropEvent* de){
	updateGL();
}
void GraphicView::dragEnterEvent(QDragEnterEvent* de){
}
void GraphicView::dragMoveEvent(QDragMoveEvent* de){
	updateGL();
}

void GraphicView::focusOnPoint(Point2D p){
	translate(p.x - viewportRect->center.x, p.y - viewportRect->center.y);
}

void GraphicView::addGraphicObject(GraphicData* obj) {
	p_scene->addItem(obj);
}
void GraphicView::addGraphicObjectList(QList<GraphicData*> objects) {
	foreach(GraphicData* obj, objects) {
		addGraphicObject(obj);
	}
}

void GraphicView::updateAll(){
	if(mysql_query(conn, "SELECT id, current_point_x, current_point_y FROM trackers")) {
		MYSQL_ROW row;
		if (res = mysql_store_result(conn)) {
			while (row = mysql_fetch_row(res)) {

				GraphicData* obj = new GraphicData(QString(row[0]), "Obj", QString(row[1]).toFloat(), QString(row[2]).toFloat(), 2, 2);

				addGraphicObject(obj);
			}
		}
	}

	updateGL();
}