#ifndef SKYDOME_H
#define SKYDOME_H 1

#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>

#include <osg/NodeCallback>

/*!
 *
 * \class SkyDome
 * \brief Skydome covering scene
 *
 * \date 20-06-2006
 * \author Zbyszek "ShaXbee" Mandziejewicz
 *
 * \todo osgEmpheresis integration
 *
 */

namespace spt
{

class SkyDome
{

public:
	SkyDome() : m_root(NULL) { init(); };
	SkyDome(osg::Group* root) : m_root(root) { init(); };

	~SkyDome();

	void setRoot(osg::Group* root);

private:
	void init();

	class Transform : public osg::Transform
	{
	public:
		/** Get the transformation matrix which moves from local coords to world coords.*/
		virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;
		virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor* nv) const;

	};

	class SkydomeCullCallback: public osg::NodeCallback
	{
	public:
		SkydomeCullCallback() { };
		void operator () (osg::Node *node, osg::NodeVisitor *nv);
	};

	osg::Group* m_root;
	osg::Transform* m_transform;
	osg::Node* m_node;

	SkydomeCullCallback* m_callback;

};

};

#endif