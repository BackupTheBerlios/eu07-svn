#ifndef TopView_HPP
#define TopView_HPP 1

#include "OSG_realizer.h"

class TopView : public OSG_realizer
{
    public:

		class MouseAdapter
		{
		public:
			virtual bool onPush(unsigned int button, unsigned int state) = 0;
			virtual bool onRelease(unsigned int button, unsigned int state) = 0;
			virtual bool onDoubleClick(unsigned int button, unsigned int state) = 0;
			virtual bool onDrag(unsigned int state) = 0;
			virtual bool onMove(unsigned int state) = 0;
		};

		TopView(GtkWidget* dr_area);
		virtual ~TopView();

		inline void setHeight(const double height) {eye.z()= height; };

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

		bool computeIntersections(double x, double y, osg::Node *node, osgUtil::IntersectVisitor::HitList& hits, osg::Node::NodeMask traversalMask = 0xffffffff);




	inline double getMouseWorldX() 
	{ return ((mc0.x/get_area_alloc()->width)*2-1)*scale*aspect+eye.x(); };
	inline double getMouseWorldY() 
	{ return -((mc0.y/get_area_alloc()->height)*2-1)*scale+eye.y(); };

	inline void setMouseAdapter(MouseAdapter *ma) { mouseAdapter= ma; };

	protected:
       
		osg::Vec3d  eye;

		MouseAdapter *mouseAdapter;

	osgUtil::IntersectVisitor iv;
};

#endif