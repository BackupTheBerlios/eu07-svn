/********************************************************************** 
 *
 *	FILE:			Util.cpp
 *
 *	DESCRIPTION:	Differents kinds of utility methods.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 04.03.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/

#include "Util.h"
#include <sstream>

/**
 * Returns true if the to Point3's are almost equal.
 */
BOOL Util::isPoint3Equal(Point3 p1, Point3 p2){
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.z - p2.z) > ALMOST_ZERO)
		return FALSE;

	return TRUE;
}

/**
 * This method will return true if the two vec3's are almost equal.
 */
BOOL Util::isVec3Equal(osg::Vec3 p1, osg::Vec3 p2){
	if (fabs(p1.x() - p2.x()) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.y() - p2.y()) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.z() - p2.z()) > ALMOST_ZERO)
		return FALSE;

	return TRUE;
}



/**
 * This method will return true if the two vec2's are almost equal.
 */
BOOL Util::isVec2Equal(osg::Vec2 p1, osg::Vec2 p2){
	if (fabs(p1.x() - p2.x()) > ALMOST_ZERO)
		return FALSE;
	if (fabs(p1.y() - p2.y()) > ALMOST_ZERO)
		return FALSE;

	return TRUE;
}

/**
 * This method will return true if the given matrix is almost 
 * an identity matrix.
 */
BOOL Util::isIdentity(osg::Matrix mat){

	double * _mat = mat.ptr();
	if(fabs(_mat[0] - 1.0f) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[1]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[2]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[3]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[4]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[5] - 1.0f) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[6]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[7]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[8]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[9]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[10] - 1.0f) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[11]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[12]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[13]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[14]) > ALMOST_ZERO)
		return FALSE;
	if(fabs(_mat[15] - 1.0f) > ALMOST_ZERO)
		return FALSE;
	return TRUE;
}

/**
 * This method will check wheter or not two OSG geodes are the 
 * same with respect too the vertices and texcoords.
 */
BOOL Util::isGeodeEqual(osg::Geode* geode1, osg::Geode* geode2){
	if(!geode1)
		return FALSE;
	if(!geode2)
		return FALSE;

	// Check for same number of drawables.
	if(geode1->getNumDrawables() != geode2->getNumDrawables())
		return FALSE;

	// For every drawable
	for(unsigned int i=0; i<geode1->getNumDrawables(); i++){
		osg::Geometry* geom1 = geode1->getDrawable(i)->asGeometry();
		osg::Geometry* geom2 = geode2->getDrawable(i)->asGeometry();

		// Check vertices.
		osg::Vec3Array* coords1 = static_cast<osg::Vec3Array*>(geom1->getVertexArray());
		osg::Vec3Array* coords2 = static_cast<osg::Vec3Array*>(geom2->getVertexArray());
		if(coords1 && coords2){
			// Check for same number of vertices.
			int size1 = coords1->size();
			int size2 = coords2->size();
			if(coords1->size() != coords2->size())
				return FALSE;
			// Check equality among every vertex.
			for(unsigned int j=0; j<coords1->size();j++){
				if(!isVec3Equal((*coords1)[j], (*coords2)[j]))
					return FALSE;
			}
		}
		
		// Check for same number of texture units.
		if(geom1->getNumTexCoordArrays() != geom1->getNumTexCoordArrays())
			return FALSE;
		for(unsigned int units = 0; units < geom1->getNumTexCoordArrays(); units++){
			// Check texture coords.
			osg::Vec2Array* tcoords1 = (osg::Vec2Array*)geom1->getTexCoordArray(units);
			osg::Vec2Array* tcoords2 = (osg::Vec2Array*)geom2->getTexCoordArray(units);
			if(tcoords1 && tcoords2){
				// Check for same number of texture coords.
				if(tcoords1->size() != tcoords2->size())
					return FALSE;
				// Check for equality amoung every texture coords.
				for(unsigned int j=0; j<tcoords1->size();j++){
					if(!isVec2Equal((*tcoords1)[j], (*tcoords2)[j]))
						return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/**
 * This method will return true if the given node is referenced 
 * by the given OSG helper object class ID.
 */
 BOOL Util::isReferencedByHelperObject(INode* node, Class_ID helperID){
	IParamBlock2* pblock2;
	RefList refList;
	refList = node->GetRefList();
	RefListItem* ptr = refList.first;
	while (ptr){
		if(ptr->maker){
			SClass_ID sid = ptr->maker->SuperClassID();
			if(sid==PARAMETER_BLOCK2_CLASS_ID){
				pblock2 = (IParamBlock2*) ptr->maker;
				ClassDesc2 * desc = pblock2->GetDesc()->cd;
				if(desc){
					Class_ID id  = desc->ClassID();
					if(id == helperID)
						return TRUE;
				}
			}
		}
		ptr = ptr->next;
	}
	return FALSE;
 }

/**
 * This method will take an OSG group node and return
 * the first encountered OSG geode found in the group.
 */
osg::Geode* Util::getGeode(osg::Group* group){
	if(!group)
		return NULL;
	for(unsigned int i =0; i < group->getNumChildren(); i++){
		osg::Node* node = group->getChild(i);
		if(node->isSameKindAs(new osg::Geode()))
			return dynamic_cast<osg::Geode*>(node);
		else if(node->isSameKindAs(new osg::Group()))
			return getGeode(dynamic_cast<osg::Group*>(node));
	}
	return NULL;
}

/**
 * Converts an unsigned integer to a hex string.
 */
std::string Util::getHexString(unsigned int i){
	char buf[100];
	sprintf(buf, "0x%08X\0",i);
	std::string s(buf);
	return s;
}

/**
 * Converts hex string to unsigned integer.
 */
unsigned int Util::getUInt(std::string s){
	char* stopstring="\0";
	return strtoul(s.c_str(), &stopstring, 0);
}
