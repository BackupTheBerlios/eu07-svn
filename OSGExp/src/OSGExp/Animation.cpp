/**********************************************************************
 *
 *	FILE:			Animation.cpp
 *
 *	DESCRIPTION:	Methods for exporting MAX animations
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 22.01.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/

#include "OSGExp.h"


/** 
 * This method will add an animation path to the given node.
 */
void OSGExp::addAnimation(INode* node, TimeValue t, osg::Node* osgNode){
	if (!hasAnimation(node))
		return;
	osg::AnimationPath* animationPath = createAnimationPath(node,t);
	if(animationPath){
		osgNode->setUpdateCallback(new osg::AnimationPathCallback(animationPath));
		osgNode->setDataVariance(osg::Object::DYNAMIC);
//		TSTR string;
//		if (node->GetUserPropString("animation",string))
//			osgNode->setName(string.data());
	}
}

/**
 * This method will create and return an animation path.
 */
osg::AnimationPath* OSGExp::createAnimationPath(INode *node, TimeValue t){

	// Get the controllers from the node.
	Control* pC = node->GetTMController()->GetPositionController();
	Control* rC = node->GetTMController()->GetRotationController();
	Control* sC = node->GetTMController()->GetScaleController();

	// Are we using TCB, Linear, and Bezier keyframe controllers. 
	if (isExportable(pC) && isExportable(rC) && isExportable(sC)) {
		if (pC->NumKeys() || rC->NumKeys() || sC->NumKeys()) {
		    // Create the animation path.
			osg::AnimationPath* animationPath = new osg::AnimationPath();
			animationPath->setLoopMode(osg::AnimationPath::LOOP);
			exportPosKeys(animationPath, pC);
			return animationPath;
		}
	}

	// Or are we using other kinds of animations which can be sampled lineary.
	if(hasAnimation(node)){
		return sampleNode(node);
	}
	return NULL;
}

/**
 * This method will sample the node at each frame to get any possible change
 * of position, rotation and scaling. The posible changes is inserted into an
 * OSG AnimationPath and returned.
 */
osg::AnimationPath* OSGExp::sampleNode(INode* node){

    // Create the animation path.
    osg::AnimationPath* animationPath = new osg::AnimationPath();
    animationPath->setLoopMode(osg::AnimationPath::LOOP);

	TimeValue start = _ip->GetAnimRange().Start();
	TimeValue end = _ip->GetAnimRange().End();
	int val;
	if (node->GetUserPropInt("startFrame",val))
		start= val;
	if (node->GetUserPropInt("endFrame",val))
		end= val;
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;

	for (t=start; t<=end; t+=delta) {
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);
		Point3 pos = ap.t;
		Point3 sca = ap.k;

		// Note: OSG wants absolute rotations
		Quat rot = ap.q;

		// Convert from Max's left-handed rotation to right-handed
		float ang;
		Point3 axis;
		AngAxisFromQ(rot, &ang, axis);
		ang = -ang;
		rot = QFromAngAxis(ang, axis);
 
		// Insert the sample point into animation path
		addControlPoint(animationPath, (t/(float)TIME_TICKSPERSEC), pos, rot, sca);
	}
	return animationPath;
}

/** 
 * This method will insert a control point into the animation path
 * given a MAX position, rotation, scale, and time.
 */
void OSGExp::addControlPoint(osg::AnimationPath* animationPath , float time,
											Point3 pos, Quat rot, Point3 sca){
	osg::Vec3 position(pos.x, pos.y, pos.z);
	osg::Quat rotation(rot.x, rot.y, rot.z, rot.w);
	osg::Vec3 scale   (sca.x, sca.y, sca.z);
      
	animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation, scale));
}

/** 
 * This method will insert a MAX position value into the animation path.
 */
void OSGExp::addControlPos(osg::AnimationPath* animationPath , float time, Point3 pos){
	osg::Vec3 position(pos.x, pos.y, pos.z);
	animationPath->insert(time, osg::AnimationPath::ControlPoint(position));
}


/** 
 * This method will check if there is any animation connected to the node.
 * It will run from start to end of animation range and if the position,
 * rotation or scale has been changed during this, it will return true.
 */
BOOL OSGExp::hasAnimation(INode* node){
	TimeValue start = _ip->GetAnimRange().Start();
	TimeValue end = _ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3 firstPos;
	float rotAngle, firstRotAngle;
	Point3 rotAxis, firstRotAxis;
	Point3 firstScaleFactor;
	//return TRUE;

	for (t=start; t<=end; t+=delta) {

		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		AngAxisFromQ(ap.q, &rotAngle, rotAxis);

		// If any point, rotation or scale in the t'th frame has
		// changes from the start frame then we have an animation.
		if (t != start){
			if(!Util::isPoint3Equal(ap.t, firstPos) ||
				!Util::isPoint3Equal(rotAxis, firstRotAxis) ||
				fabs(rotAngle - firstRotAngle) > ALMOST_ZERO ||
				!Util::isPoint3Equal(ap.k, firstScaleFactor)){
				return TRUE;
			}
		}
		else {
			firstPos = ap.t;
			firstRotAngle = rotAngle;
			firstRotAxis = rotAxis;
			firstScaleFactor = ap.k;
		}
	}
	return FALSE;
}

/**
 * This method will add all the position keys found in the controller to
 * the animation path.
 */
void OSGExp::exportPosKeys(osg::AnimationPath* animationPath, Control* cont){
	if (!cont)
		return;

	int i;
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	// TCB position
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0)) {
		int numKeys;
		if (numKeys = ikc->GetNumKeys()) {
			for (i=0; i<numKeys; i++) {
				ITCBPoint3Key key;
				ikc->GetKey(i, &key);
				; // NOT SUPPORTED YET
			}
		}
	}
	// Bezier position
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0)){
		int numKeys;
		if(numKeys = ikc->GetNumKeys()){
			for (i=0; i<numKeys; i++) {
				IBezPoint3Key key;
				ikc->GetKey(i, &key);
				; // NOT SUPPORTED YET
			}
		}
	}
	// Linear position
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0)) {
		int numKeys;
		if(numKeys = ikc->GetNumKeys()){
			for (i=0; i<numKeys; i++) {
				ILinPoint3Key key;
				ikc->GetKey(i, &key);
				addControlPos(animationPath, (key.time/(float)TIME_TICKSPERSEC),key.val);
			}
		}
	}
}

/** 
 * This method will return true if the given TM controller can
 * be simulated in OSG.
 */
BOOL OSGExp::isExportable(Control* cont){
	ulong partA, partB;

	if (!cont)
		return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)
		return FALSE;

	switch (partA) {
//		case TCBINTERP_POSITION_CLASS_ID:
//		case TCBINTERP_ROTATION_CLASS_ID:
//		case TCBINTERP_SCALE_CLASS_ID:
//		case HYBRIDINTERP_POSITION_CLASS_ID:
//		case HYBRIDINTERP_ROTATION_CLASS_ID:
//		case HYBRIDINTERP_SCALE_CLASS_ID:
		case LININTERP_POSITION_CLASS_ID:
//		case LININTERP_ROTATION_CLASS_ID:
//		case LININTERP_SCALE_CLASS_ID:
			return TRUE;
	}

	return FALSE;
}

