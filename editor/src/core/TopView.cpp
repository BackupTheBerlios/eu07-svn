
#include <osg/Notify>

#include "TopView.h"

#include <windows.h>



TopView::TopView(GtkWidget* dr_area) : OSG_realizer(dr_area), eye(0.0,0.0,0.0), mouseAdapter(NULL)
{
	setHeight(300);
	char buf[256];
	GetPrivateProfileString("Settings","ViewX","0",buf,255,"editor.ini");
	eye.x()= atof(buf);
	GetPrivateProfileString("Settings","ViewY","0",buf,255,"editor.ini");
	eye.y()= atof(buf);
	GetPrivateProfileString("Settings","Scale","10",buf,255,"editor.ini");
	scale= atof(buf);

	sceneView->setCullingMode(osg::CullSettings::DEFAULT_CULLING);
}

TopView::~TopView()
{
	char buf[256];
	sprintf(buf,"%f",eye.x());
	WritePrivateProfileString("Settings","ViewX",buf,"editor.ini");
	sprintf(buf,"%f",eye.y());
	WritePrivateProfileString("Settings","ViewY",buf,"editor.ini");
	sprintf(buf,"%f",scale);
	WritePrivateProfileString("Settings","Scale",buf,"editor.ini");
}

void TopView::setByMatrix(const osg::Matrixd& matrix)
{
	eye = matrix.getTrans();
}

osg::Matrixd TopView::getMatrix() const
{
	return osg::Matrixd::inverse(getInverseMatrix());
}

osg::Matrixd TopView::getInverseMatrix() const
{
	return osg::Matrixd::lookAt(eye,eye+osg::Vec3d(0,0,-1),osg::Vec3d(0,1,0));
}

gboolean TopView::button(GdkEventButton *eb) 
{
		
	bool ret= false;
	if (mouseAdapter)
	{
		switch (eb->type) {
			case GDK_BUTTON_PRESS :
				ret= mouseAdapter->onPush(eb->button,eb->state);
			break;
			case GDK_2BUTTON_PRESS :
				ret= mouseAdapter->onDoubleClick(eb->button,eb->state);
			break;
			case GDK_BUTTON_RELEASE :
				ret= mouseAdapter->onRelease(eb->button,eb->state);
			break;
		}
	}
	if (ret)
		return true;
	else
		return OSG_realizer::button(eb); 
}

gboolean TopView::motion(GdkEventMotion *em) 
{
	bool ret= false;
	if (em->state & GDK_BUTTON2_MASK)
	{
		 eye-= osg::Vec3(getMouseWorldDX(),getMouseWorldDY(),0.0f);
		 exposed();
		 ret= true;
	}
	if (em->state & (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK))
		ret= mouseAdapter->onDrag(em->state);
	else
		ret= mouseAdapter->onMove(em->state);

	if (ret)
		return true;
	else
		return OSG_realizer::motion(em); 
}

gboolean TopView::scroll(GdkEvent *ev)
{
	switch (ev->scroll.direction) {
		case GDK_SCROLL_UP:
			setScale(scale/1.5);
		return true;
		case GDK_SCROLL_DOWN:
			setScale(scale*1.5);
		return true;
	}
	return OSG_realizer::scroll(ev); 
}

bool TopView::computeIntersections(double x, double y, osg::Node *node, osgUtil::IntersectVisitor::HitList& hits, osg::Node::NodeMask traversalMask)
{
	osg::LineSegment *ls= new osg::LineSegment(osg::Vec3(x,y,10000),osg::Vec3(x,y,-1000));
	iv.reset();
	iv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	iv.addLineSegment(ls);
	iv.setTraversalMask(traversalMask);
	iv.traverse(*node);
	hits= iv.getHitList(ls);
	bool ret= iv.hits();
	iv.reset();
//	iv.getin//traverse(*node);
	return ret;
}

