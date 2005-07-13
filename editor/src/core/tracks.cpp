//---------------------------------------------------------------------------
/*
    MaSzyna2 EU07 locomotive simulator scenery editor
    Copyright (C) 2004-2005  Marcin Wozniak

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "tracks.h"
#include "editor.h"
#include "TrackPiece.h"

#include <osg/PolygonMode>

edFlexTrack *edFlexTrack::clone()
{
	edFlexTrack *trk= new edFlexTrack();
	trk->setBallastShape(ballast);
	trk->setRailsShape(rails);
	trk->rot= rot;
	trk->pos= pos+osg::Vec3d(10,10,0);
	trk->setBezier(trackPieces[0]->segment.GetPt1(),trackPieces[0]->segment.GetCPt1(),trackPieces[0]->segment.GetCPt2(),trackPieces[0]->segment.GetPt2(),trackPieces[0]->segment.getRoll1(),trackPieces[0]->segment.getRoll2());
	trk->ballastTex= ballastTex;
	trk->railsTex= railsTex;
	return trk;

}

void edFlexTrack::load(std::istream &stream, int version, CollectNodes *cn)
{
	edTrack::load(stream, version, cn);
	if (version<=2)
		blend= false;
	else
		read(stream, blend);
	rails.resize(readUI(stream));
	for (unsigned int i=0; i<rails.size(); i++)
	{
		rails[i].resize(readUI(stream));
		for (unsigned int j=0; j<rails[i].size(); j++)
			rails[i][j]= readV3f(stream);
	}
	ballast.resize(readUI(stream));
	for (unsigned int i=0; i<ballast.size(); i++)
	{
		ballast[i].resize(readUI(stream));
		for (unsigned int j=0; j<ballast[i].size(); j++)
			ballast[i][j]= readV3f(stream);
	}
	read(stream,flags);
	osg::Vec3d pt1(readV3d(stream));
	osg::Vec3d cpt1(readV3d(stream));
	osg::Vec3d cpt2(readV3d(stream));
	osg::Vec3d pt2(readV3d(stream));
	float r1(readF(stream));
	float r2(readF(stream));
	setBezier(pt1,cpt1,cpt2,pt2,r1,r2);
	trackPieces[0]->loadSignals(stream,version,cn);
	trackPieces.front()->setFlags(flags);		//HACK
}

void edFlexTrack::save(std::ostream &stream)
{
	edTrack::save(stream);
	write(stream, blend);
	write(stream, rails.size());
	for (unsigned int i=0; i<rails.size(); i++)
	{
		write(stream, rails[i].size());
		for (unsigned int j=0; j<rails[i].size(); j++)
			write(stream, rails[i][j]);
	}
	write(stream, ballast.size());
	for (unsigned int i=0; i<ballast.size(); i++)
	{
		write(stream, ballast[i].size());
		for (unsigned int j=0; j<ballast[i].size(); j++)
			write(stream, ballast[i][j]);
	}
	write(stream,flags);
	write(stream,trackPieces[0]->segment.GetPt1());
	write(stream,trackPieces[0]->segment.GetCPt1());
	write(stream,trackPieces[0]->segment.GetCPt2());
	write(stream,trackPieces[0]->segment.GetPt2());
	write(stream,trackPieces[0]->segment.getRoll1());
	write(stream,trackPieces[0]->segment.getRoll2());
	trackPieces[0]->saveSignals(stream);
}

void edFlexTrack::export(std::ostream &stream) 
{
	edTrack::export(stream);
	/*
	write(stream,unsigned int(1));
	write(stream,segment.GetPt1());
	write(stream,segment.GetCPt1());
	write(stream,segment.GetCPt2());
	write(stream,segment.GetPt2());
	write(stream,segment.getRoll1());
	write(stream,segment.getRoll2());*/
}

edFlexTrack::FlexPair edFlexTrack::split(float maxLen)
{
//	osg::Vec3d ptA1
	osg::Vec3d pt1= trackPieces[0]->segment.GetPt1();
	osg::Vec3d cpt1= (trackPieces[0]->segment.GetPt1()+trackPieces[0]->segment.GetCPt1())*0.5;
//	osg::Vec3d pt2= (trackPieces[0]->segment.GetPt2()+trackPieces[0]->segment.GetCpt2())*0.5;
	osg::Vec3d mid= (trackPieces[0]->segment.GetCPt1()+trackPieces[0]->segment.GetCPt2())*0.5;
	osg::Vec3d cpt2= (mid+cpt1)*0.5;

	osg::Vec3d cpt4= (trackPieces[0]->segment.GetPt2()+trackPieces[0]->segment.GetCPt2())*0.5;
	osg::Vec3d cpt3= (mid+cpt4)*0.5;

	osg::Vec3d pt2= (cpt2+cpt3)*0.5;
	osg::Vec3d pt3(pt2); 
	osg::Vec3d pt4= trackPieces[0]->segment.GetPt2();

	edFlexTrack *trk1= clone();
	edFlexTrack *trk2= clone();

	Editor::instance()->deselectNode();
//	osg::ref_ptr<edFlexTrack> tmp= this;
	osg::Vec3d ps= pos;
	osg::Quat quat(rot);

	Connection *con1= getConnection(0)->getConnected();
	Connection *con2= getConnection(1)->getConnected();

	free(); // 'this' is NOT valid beyond this line !!!!

	osg::Vec3d cntr= (pt1+cpt1+cpt2+pt2)*0.25;
	trk1->setBezier(pt1-cntr,cpt1-cntr,cpt2-cntr,pt2-cntr,0,0);
	
	trk1->moveTo(ps+quat*cntr);


	cntr= (pt3+cpt3+cpt4+pt4)*0.25;
	trk2->setBezier(pt3-cntr,cpt3-cntr,cpt4-cntr,pt4-cntr,0,0);
	trk2->moveTo(ps+quat*cntr);

	FlexPair ret(trk1,trk2);

	ret.first->getConnection(0)->connectTo(con1);
	ret.second->getConnection(1)->connectTo(con2);
	ret.first->getConnection(1)->connectTo(ret.second->getConnection(0));


	if (trk1->trackPieces[0]->segment.GetLength()>maxLen || trk2->trackPieces[0]->segment.GetLength()>maxLen)
	{
//		tr1= trk1->split(maxLen);
//		ret.first= trk1->split(maxLen).first;
//	if ()
//		tr2= trk2->split(maxLen);
//		ret.second= trk2->split(maxLen).second;
		FlexPair &tr1(trk1->split(maxLen));
		FlexPair &tr2(trk2->split(maxLen));
//		tr1.second->getConnection(1)->connectTo(tr2.first->getConnection(0));
//		ret.first= trk1->split(maxLen).second;
//		ret.second= trk2->split(maxLen).first;
		ret.first= tr1.first;
		ret.second= tr2.second;
	}
//	else
//		ret.first->getConnection(1)->connectTo(ret.second->getConnection(0));

	return ret;
}

void edTrackConnection::onConnectionChange() 
{
	if (sw.valid() && !sw->getValue(1)) 
		sw->setSingleChildOn(getDeselectedVisualNum()); 
}

void edTrackConnection::updateVisual()
{
	if (!transform.valid())
	{
		transform= new osg::AutoTransform();
		transform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
		transform->setAutoScaleToScreen(true);
		sw= new osg::Switch();
		sw->addChild(new osg::Geode(*Editor::lastInstance()->deselectedConnectionVisual.get()),false);
		sw->addChild(new osg::Geode(*Editor::lastInstance()->selectedConnectionVisual.get()),false);
		sw->addChild(new osg::Geode(*Editor::lastInstance()->connectedConnectionVisual.get()),false);

		transform->addChild(sw.get());
		Editor::lastInstance()->getNodesRoot()->addChild(transform.get());
		setVisual(transform.get(),sw.get());	
		onDeselect();

	}

	transform->setPosition(pos);
	transform->setRotation(rot);

}

Connection::~Connection()
{
	disconnect();
	node->free();
}


void Connection::setPos(osg::Vec3f _pos) 
{
	pos= _pos;
	node->moveTo(getWorldPos());
}

void Connection::setDir(osg::Vec3f _dir) 
{
	dir= _dir; 
}

int edFlexTrack::totalFexTracks= 0;

edFlexTrack::edFlexTrack() : edTrack() 
{
	maxDist= 2000;
	totalFexTracks++;
	flags= 0;
	connections.reserve(2);
	connections.push_back(new Connection(this));
	connections.push_back(new Connection(this));
	blend= false;
}

void edFlexTrack::setBezier(osg::Vec3f pt1, osg::Vec3f cpt1, osg::Vec3f cpt2, osg::Vec3f pt2, float roll1, float roll2)
{
	/*
	if (connections.size()<2)
	{
		connections.reserve(2);
		connections.push_back(new Connection(this));
		connections.push_back(new Connection(this));
	}*/
	if (trackPieces.size()<1)
	{
		trackPieces.reserve(1);
		trackPieces.push_back(new TrackPiece(connections[0].get(),connections[1].get(),this,name.c_str()));
	}
	connections[0]->setPos(pt1);
	connections[0]->setDir(pt1-cpt1);
	connections[1]->setPos(pt2);
	connections[1]->setDir(pt2-cpt2);
	trackPieces[0]->setBezier(pt1,cpt1,cpt2,pt2,roll1,roll2);
	/*
	segment.Init(pt1,cpt1,cpt2,pt2,roll1,roll2);
	segment.PointsChange();*/
}
//#include "editor.h"
void edFlexTrack::updateVisual()
{
	if (!geomTransform.valid())//(visuals.empty())
	{
		osg::Texture2D *tex2d= NULL;
		osg::Material *mater= NULL;
		osg::StateSet *ballast_dstate= NULL;
		osg::StateSet *rails_dstate= NULL;
		osg::Geode *geode= NULL;
		osg::Geometry *geom= NULL;
		osg::TexEnvCombine *envComb= NULL;
		double minDist= 0;
		double maxDist= 0;
		double minDist2= 0;
		double maxDist2= 0;


		edFlexTrack *prev= ( getConnection(0)->getConnected() ? dynamic_cast<edFlexTrack*>(getConnection(0)->getConnected()->getOwner()) : NULL );
		edFlexTrack *next= ( getConnection(1)->getConnected() ? dynamic_cast<edFlexTrack*>(getConnection(1)->getConnected()->getOwner()) : NULL );

		bool ballastBlend=	false;
		bool railsBlend=	false;
		if ( blend && prev && next && !prev->blend && !next->blend )
		{
			next->updateVisual();
			prev->updateVisual();

			ballastBlend=	prev->ballastTex2D.valid() && next->ballastTex2D.valid() && prev->ballastTex!=next->ballastTex;
			railsBlend=		prev->railsTex2D.valid() && next->railsTex2D.valid() && prev->railsTex!=next->railsTex;
		}




		bool curve= true;
		osg::ref_ptr<osg::LineSegment> ls= new osg::LineSegment(trackPieces[0]->segment.GetPt1(),trackPieces[0]->segment.GetPt2());
		if (ls->intersect(osg::BoundingSphere(trackPieces[0]->segment.GetCPt1(),0.1)) && 
			ls->intersect(osg::BoundingSphere(trackPieces[0]->segment.GetCPt2(),0.1)))
			curve= false;

//		curve= false;
		double step= ( curve ? 5 : trackPieces[0]->segment.GetLength() );


		osg::LOD *lod= ( curve && trackPieces[0]->segment.GetLength()>20 ? new osg::LOD : NULL );

		geomTransform= new osg::MatrixTransform;
		geomTransform->setDataVariance(osg::Node::STATIC);
		geomTransform->setNodeMask(Editor::nm_Tracks);
//		geomTransform->addChild(Editor::lastInstance()->selectedPointVisual.get());
		edGeomNode::updateVisual();
		geomTransform->setUserData(this);
		if (lod) 
			geomTransform->addChild(lod);

//		return;
		int nv= 0;
		if (ballast.size()>0 && ballast[0].size()>1)
			nv++;
		if (rails.size()>0 && rails[0].size()>1)
			nv++;

		setNumVisuals(nv);

		bool lastLod= lod==NULL;
		double lastDist= 3000;

		ballast_dstate= new osg::StateSet();
//		ballast_dstate->setMode(GL_LIGHTING,osg::StateAttribute::ON);

		if (ballastBlend)
		{
			ballast_dstate->setTextureAttributeAndModes(0, prev->ballastTex2D.get(), osg::StateAttribute::ON );
			ballast_dstate->setTextureAttributeAndModes(1, next->ballastTex2D.get(), osg::StateAttribute::ON );

			envComb= new osg::TexEnvCombine;
			envComb->setSource0_RGB(osg::TexEnvCombine::TEXTURE1);
			envComb->setSource1_RGB(osg::TexEnvCombine::TEXTURE0);
			envComb->setSource2_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
			envComb->setCombine_RGB(osg::TexEnvCombine::INTERPOLATE); 
			ballast_dstate->setTextureAttribute(0, envComb);

			envComb= new osg::TexEnvCombine;
			envComb->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
			envComb->setSource1_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
			envComb->setCombine_RGB(osg::TexEnvCombine::MODULATE); 
			ballast_dstate->setTextureAttribute(1, envComb);	

			ballast_dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::OFF ); 
		}
		else
		{
			tex2d= new osg::Texture2D();
			tex2d->setDataVariance(osg::Node::STATIC);
			ballastTex2D= tex2d;
			
			ballast_dstate->setDataVariance(osg::Node::STATIC);
	//		tex2d->setUseHardwareMipMapGeneration(false);
			tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			tex2d->setImage(osgDB::readImageFile(ballastTex.c_str()));
	//		tex2d->setImage(osgDB::readImageFile("TpD1.dds",osgDB::Registry::CACHE_ALL));

			ballast_dstate->setTextureAttributeAndModes(0, tex2d, osg::StateAttribute::ON );
//			dstate->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
		}

		rails_dstate= new osg::StateSet();
//		rails_dstate->setMode(GL_LIGHTING,osg::StateAttribute::ON);

		if (railsBlend)
		{
			rails_dstate->setTextureAttributeAndModes(0, prev->railsTex2D.get(), osg::StateAttribute::ON );
			rails_dstate->setTextureAttributeAndModes(1, next->railsTex2D.get(), osg::StateAttribute::ON );

			envComb= new osg::TexEnvCombine;
			envComb->setSource0_RGB(osg::TexEnvCombine::TEXTURE1);
			envComb->setSource1_RGB(osg::TexEnvCombine::TEXTURE0);
			envComb->setSource2_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
			envComb->setCombine_RGB(osg::TexEnvCombine::INTERPOLATE); 
			rails_dstate->setTextureAttribute(0, envComb);

			envComb= new osg::TexEnvCombine;
			envComb->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
			envComb->setSource1_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
			envComb->setCombine_RGB(osg::TexEnvCombine::MODULATE); 
			rails_dstate->setTextureAttribute(1, envComb);	

			rails_dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::OFF ); 
		}
		else
		{
			tex2d= new osg::Texture2D();
			tex2d->setDataVariance(osg::Node::STATIC);
			railsTex2D= tex2d;
			rails_dstate->setDataVariance(osg::Node::STATIC);
	//		tex2d->setUseHardwareMipMapGeneration(false);
			tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			tex2d->setImage(osgDB::readImageFile(railsTex.c_str()));
	//		tex2d->setImage(osgDB::readImageFile("Rail_screw_used1.dds",osgDB::Registry::CACHE_ALL));
			rails_dstate->setTextureAttributeAndModes(0, tex2d, osg::StateAttribute::ON );
//			dstate->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
		}



		do 
		{
			if (step>trackPieces[0]->segment.GetLength()*0.9)
				lastLod= true;

		if (ballast.size()>0 && ballast[0].size()>1)
		{
		geom= trackPieces[0]->segment.CreateLoft(ballast, 5, step, osg::Vec3d(0,0,0), ballastBlend);
		geom->setUserData(this);
		geom->setDataVariance(osg::Node::STATIC);

		geode= new osg::Geode();
		geode->setDataVariance(osg::Node::STATIC);
		geode->setStateSet(ballast_dstate);
		geode->addDrawable(geom);
		geode->setUserData(this);
//		addVisual(geode);
		geom->setUseDisplayList(true);
		geom->setDataVariance(osg::Object::DataVariance::STATIC);
		geode->setDataVariance(osg::Object::DataVariance::STATIC);
		minDist= maxDist;
		maxDist= ( lastLod ? lastDist : geode->getBound().radius()+step*20 );

		if (lod)
			lod->addChild(geode,minDist,maxDist);
		else
			geomTransform->addChild(geode);

//*
		if (minDist==0)
		{
		geode = new osg::Geode;
		geode->setDataVariance(osg::Node::STATIC);
		geode->addDrawable( geom );
		geode->setUserData(this);
//		dstate= new osg::StateSet();
//		dstate->setRenderBinDetails(10,"RenderBin");
//		dstate->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS));
//		osg::PolygonMode *pm= new osg::PolygonMode();
//		pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
//		dstate->setAttributeAndModes(pm);

//		dstate->setDataVariance(osg::Node::STATIC);
//		mater= new osg::Material();
//		mater->setDataVariance(osg::Node::STATIC);
//		mater->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//		mater->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//		mater->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,1.0,1.0));
//		dstate->setAttributeAndModes(mater);
//		geode->setStateSet( dstate );
		geode->setStateSet( Editor::instance()->selectedNodesState.get() );
		geomTransform->addChild(geode);
		addSelectedVisual(geode);
		}
		}
//*/
		
		if ((minDist==0 || maxDist<500) && rails.size()>0 && rails[0].size()>1)
		{
		geom= trackPieces[0]->segment.CreateLoft(rails, 5, step, osg::Vec3d(0,0,0), railsBlend);
		geom->setDataVariance(osg::Node::STATIC);
		geom->setUserData(this);

		geode= new osg::Geode();
		geode->setDataVariance(osg::Node::STATIC);
		geode->setStateSet(rails_dstate);
		geode->addDrawable(geom);
//		geode->setUserData(this);
//		addVisual(geode);
		geom->setUseDisplayList(true);
		geom->setDataVariance(osg::Object::DataVariance::STATIC);
		geode->setDataVariance(osg::Object::DataVariance::STATIC);
//		minDist2= maxDist2;
//		maxDist2= ( geode->getBound().radius()+5*10+step*10 );
//		maxDist2= ( lastLod ? lastDist : geode->getBound().radius()+5*10+step*10 );
		if (lod)
			lod->addChild(geode,minDist,maxDist);
		else
			geomTransform->addChild(geode);
/*
		geode = new osg::Geode;
		geode->addDrawable( geom );
		geode->setUserData(this);
		dstate= new osg::StateSet();
		mater= new osg::Material();
		mater->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		mater->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//		mater->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,1.0,1.0));
		dstate->setAttributeAndModes(mater);
		geode->setStateSet( dstate );
		addSelectedVisual(geode);*/
		}

		step*= 2;
		} while (!lastLod);

	}
	//else
		edTrack::updateVisual();
		for (unsigned int i=0; i<visualsTransforms.size(); i++)
			visualsTransforms[i]->setMatrix(geomTransform->getMatrix());
}

void edFlexTrack::setBlendI(int val) 
{
	if (blend!= (val==1))
	{
		Editor::instance()->deselectNode();
		blend= (val==1); 
		edGeomNode::freeGeometry();
		updateVisual();
		redrawAll(); 
	}
};


void edTrack::free()
{

	edGeomNode::free();
}

void edTrack::load(std::istream &stream, int version, CollectNodes *cn)
{
	edGeomNode::load(stream, version, cn);
	read(stream, name); onSetName();		//HACK
	read(stream, railsTex);
	read(stream, ballastTex);
	unsigned int n= readUI(stream);
//	if (connections.empty())
//		connections.reserve(n);
	for (unsigned int i=0; i<n; i++)
	{
//		connections.push_back(new Connection(this));
		unsigned int uid= readUI(stream);
		if (uid!=0xFFFFFFFF)
		{
			unsigned int cid= readUI(stream);
			if (uid<cn->trackNodesList.size())
				connections[i]->connectTo(cn->trackNodesList[uid]->getConnection(cid));
		}
	}
}

void edTrack::save(std::ostream &stream)
{
	edGeomNode::save(stream);
	write(stream, name);
	write(stream, railsTex);
	write(stream, ballastTex);
	write(stream, connections.size());
	for (unsigned int i=0; i<connections.size(); i++)
	{
		if (connections[i]->getConnected())
		{
			edTrack *him= connections[i]->getConnected()->getOwner();
			write(stream, him->getUID());
			write(stream, connections[i]->getConnected()->getID());
//			write(stream, him->getConnectionID(connections[i]->getConnected()));
		}
		else 
			write(stream, 0xFFFFFFFF);
	}
}

void edTrack::setupTrackIDs(unsigned int &id)
{
	for (unsigned int i=0; i<trackPieces.size(); i++)
		trackPieces[i]->setUID(id++);
}

void edTrack::export(std::ostream &stream) 
{
	edGeomNode::export(stream);
	for (unsigned int i=0; i<trackPieces.size(); i++)
	{
		edNode::write(stream,trackPieces[i]->getType());
		trackPieces[i]->export(stream);
	}
	/*
	write(stream, connections.size());
	for (unsigned int i=0; i<connections.size(); i++)
	{
		if (connections[i]->getConnected())
		{
			edTrack *him= connections[i]->getConnected()->getOwner();
			write(stream, him->getUID());
			write(stream, connections[i]->getConnected()->getID());
//			write(stream, him->getConnectionID(connections[i]->getConnected()));
		}
		else 
			write(stream, 0xFFFFFFFF);
	}
*/
}


inline osg::Matrixd basisChange(const osg::Vec3d& u, const osg::Vec3d& v, const osg::Vec3d& n) {
	return osg::Matrixd(u.x(),v.x(),n.x(),0,u.y(),v.y(),n.y(),0,u.z(),v.z(),n.z(),0,0,0,0,1);
}

void edTrack::updateVisual()
{
	for (unsigned int i=0; i<connections.size(); i++)
		connections[i]->update();
	edGeomNode::updateVisual();
}

void edTrack::onSelect(osg::Geode *geom)
{
	edGeomNode::onSelect(geom);
	for (unsigned int i=0; i<trackPieces.size(); i++)
		trackPieces[i]->markSignals(true);
}

void edTrack::onDeselect()
{
	edGeomNode::onDeselect();
	for (unsigned int i=0; i<trackPieces.size(); i++)
		trackPieces[i]->markSignals(false);
}

TrackPiece* edTrack::findAdjacentTrack(osg::Vec3d pos, osg::Vec3d dir, int &side)
{
	for (unsigned int i=0; i<trackPieces.size(); i++)
	{
		int s= trackPieces[i]->isAdjacent(pos,dir);
		if (s>=0)
		{
			side= s;
			return trackPieces[i].get();
		}
	}
	return NULL;
}

Connection::Connection(edTrack *_owner, ConnectionType _type) : 
	owner(_owner), 
	pos(0,0,0), 
	dir(0,1,0), 
	type(_type), 
	connected(NULL),
	trackPieceOwner(NULL)
{
	node= new edTrackConnection();
	node->owner= this;
}

bool Connection::connectTo(Connection *connection)
{
	if (connected==NULL && connection && connection->connected==NULL)
	{
		connection->connected= this;
		connected= connection;
		connected->node->onConnectionChange();
		node->onConnectionChange();
		return true;
	}
	return false;
}

bool Connection::connectAndAllign(Connection *connection)
{
	if (connectTo(connection))
	{
		osg::Quat q;
		osg::Vec3d d1= getDir(); d1.z()= 0; d1.normalize();
		osg::Vec3d d2= -connection->getWorldDir(); d2.z()= 0; d2.normalize();
		
		if ( fabs(d1*d2 + 1.0) < 0.0000001 )
			q.makeRotate(osg::DegreesToRadians(180.0),0,0,1);
		else
			q.makeRotate(d1,d2);

		owner->setRotation(q);
		owner->moveTo(connection->getWorldPos()+(owner->getPosition()-getWorldPos()));

		return true;
	}
	else
		return false;
}

bool Connection::disconnect()
{
	if (connected)
	{
		connected->connected= NULL;
		connected->node->onConnectionChange();
		connected= NULL;
		node->onConnectionChange();
		return true;
	}
	return false;
}

void Connection::update()
{
	osg::Quat q;
	q.makeRotate(osg::Vec3f(1,0,0),getWorldDir());
	node->setPositionAndRotation(getWorldPos(),q);
}

