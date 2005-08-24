
#include <osg/Notify>

#include "PerspectiveView.h"



PerspectiveView::PerspectiveView(GtkWidget* dr_area) : OSG_realizer(dr_area), defaultTarget(0.0,0.0,0.0)
{
	sceneView->setClearColor(osg::Vec4(0,0.6,1,1));
	perspective= true;
	reset(true);
}

PerspectiveView::~PerspectiveView()
{
}

void PerspectiveView::setByMatrix(const osg::Matrixd& matrix)
{
	target= matrix.getTrans();
	reset(false);
}

osg::Matrixd PerspectiveView::getMatrix() const
{
	return osg::Matrixd::inverse(getInverseMatrix());
}

osg::Matrixd PerspectiveView::getInverseMatrix() const
{
	return osg::Matrixd::translate(-target)*osg::Matrixd::rotate(-yaw,osg::Vec3d(0,0,1),-pitch,osg::Vec3d(1,0,0),0,osg::Vec3d(0,1,0))*osg::Matrixd::translate(0,0,-radius);
}

gboolean PerspectiveView::button(GdkEventButton *eb) 
{
	return OSG_realizer::button(eb); 
}

gboolean PerspectiveView::motion(GdkEventMotion *em) 
{
	if (em->state & GDK_BUTTON1_MASK)
	{
		pitch+= getMouseNormDY()*5;
		yaw-= getMouseNormDX()*5;
		exposed();
		return true;
	}
	if (em->state & GDK_BUTTON2_MASK)
	{
		 target-= osg::Vec3d(getMouseNormDX(),getMouseNormDY(),0.0)*osg::Matrixd::rotate(yaw,0,0,1)*radius;
		 exposed();
		 return true;
	}
	if (em->state & GDK_BUTTON3_MASK)
	{
		radius-= getMouseNormDY()*radius*10;
		if (radius<10)
			radius= 10;
		exposed();
		return true;
	}
	return OSG_realizer::motion(em); ; 
}

gboolean PerspectiveView::scroll(GdkEvent *ev)
{
	switch (ev->scroll.direction) {
		case GDK_SCROLL_UP:
			radius/= 1.5;
			exposed();
		return true;
		case GDK_SCROLL_DOWN:
			radius*= 1.5;
			exposed();
		return true;
	}
	return OSG_realizer::scroll(ev); 
}
