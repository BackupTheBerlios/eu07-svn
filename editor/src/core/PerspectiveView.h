#ifndef PERSPECTIVEVIEW_HPP
#define PERSPECTIVEVIEW_HPP 1

#include "OSG_realizer.h"

class PerspectiveView : public OSG_realizer
{
    public:

		PerspectiveView(GtkWidget* dr_area);
		virtual ~PerspectiveView();

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByMatrix(const osg::Matrixd& matrix);

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

        /** get the position of the manipulator as 4x4 Matrix.*/
        virtual osg::Matrixd getMatrix() const;

        /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
        virtual osg::Matrixd getInverseMatrix() const;

		virtual gboolean button(GdkEventButton *eb);
		virtual gboolean motion(GdkEventMotion *em);
		virtual gboolean scroll ( GdkEvent *ev );

		virtual void reset(bool position) { pitch= osg::DegreesToRadians(45.0); yaw= 0; radius= 20; if (position) target= defaultTarget; };

		void setTarget(osg::Vec3d _target) { target=defaultTarget= _target; };

	protected:

       
		osg::Vec3d defaultTarget, target;
		double pitch,yaw;
		double radius;

};

#endif