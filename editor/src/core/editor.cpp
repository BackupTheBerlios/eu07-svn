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

#include "editor.h"

#include <osgDB/WriteFile>
#include <osgUtil/IntersectVisitor>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Material>
#include <osg/Depth>
#include <osg/PolygonMode>
#include <osg/AutoTransform>
#include <osg/LineWidth>
#include <osg/StateAttribute>
#include <osg/Billboard>
//#include <osg/Billboard360.h>

extern "C" {
#include "triangle.h"
}

#include "../publisher.h"

#include <fstream>

#include <windows.h>

/*
top=1
bottom=-4
right=5
left=-5
*/
//const double realDimX =  68000;
//const double realDimY = 111000;

#include <osg/Projection>
#include <osg/MatrixTransform>

#include "terrainNode.h"
#include "railroad.h"
#include "tracks.h"
#include "MilePost.h"
#include "signals.h"
#include "templateTracks.h"
#include "scnImport.h"

osg::ref_ptr<osgDB::ReaderWriter> Editor::iveReaderWriter;
const double Editor::cellDim= 1000.0;
Editor *Editor::self= NULL;
Editor::ObjectsCache Editor::objectsCache;
//bool Editor::releaseGeometry= true;

const unsigned int	dwMagic= 'BSCN';
const int			version= 2;

Editor *Editor::lastInstance()
{
	return self;
}

Editor *Editor::instance()
{
	return self;
}

#define registerClass(CLAZZ) addKit(CLAZZ::getClassID(),CLAZZ::getClassProps());

Editor::Editor(TopView *_mainView) : MouseAdapter(), mainView(_mainView), mode(em_Select), allowRedraws(false)
{
	material= NULL;
	self= this;

//	defaultOptions= new osgDB::ReaderWriter::Options();
//	defaultOptions->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_ALL);

	root= new osg::Group;

	mainView->setMouseAdapter(this);
	mainView->setSceneGraph(root.get());

	defaultOptions= new osgDB::ReaderWriter::Options("noTexturesInIVEFile");
	defaultOptions->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_ALL);
	osgDB::Registry::instance()->setOptions(defaultOptions.get());
//	osgDB::Registry::instance()->setUseObjectCacheHint(osgDB::Registry::CACHE_ALL);
//	osgDB::PushAndPopDataPath tex_path("texture");
//	osgDB::PushAndPopDataPath mdl_path("models");
//	osgDB::PushAndPopDataPath srtm_path("srtm");
//	osgDB::PushAndPopDataPath map_path("maps");
//	osgDB::PushAndPopDataPath mat_path("materials");
	osgDB::Registry::instance()->getDataFilePathList().push_back("texture");
	osgDB::Registry::instance()->getDataFilePathList().push_back("models");
	osgDB::Registry::instance()->getDataFilePathList().push_back("srtm");
	osgDB::Registry::instance()->getDataFilePathList().push_back("maps");
	osgDB::Registry::instance()->getDataFilePathList().push_back("materials");
//	osgDB::Registry::instance()->getDataFilePathList().push_back("models/pkp");		//HACK
	osgDB::Registry::instance()->getDataFilePathList().push_back("models/signals");
	osgDB::Registry::instance()->getDataFilePathList().push_back("models/tracks");

	osg::StateSet *stateset= NULL;

	//zoom(1);

	nodesRoot= new osg::Group();
	mapsRoot= new osg::Switch();
	terrainRoot= new osg::Group();
	dynamicModelsRoot= new osg::Group();

	root->addChild(nodesRoot.get());
	root->addChild(mapsRoot.get());
	root->addChild(terrainRoot.get());
	root->addChild(dynamicModelsRoot.get());

	nodesRoot->setNodeMask(nm_Nodes);
	mapsRoot->setNodeMask(nm_Maps);
//	nodesRoot->setNodeMask(0x000000FF);
//	mapsRoot->setNodeMask( 0x0000FF00);

	edTerrainNode::terrainRoot= terrainRoot.get();

//	root->addChild(createStatusBar(4));

	stateset= new osg::StateSet();
//	osg::PolygonMode *pm= new osg::PolygonMode();
//	pm->setMode(osg::PolygonMode::Face::FRONT_AND_BACK,osg::PolygonMode::Mode::LINE);
//	stateset->setAttributeAndModes(pm);
	stateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS));
//	stateset->setMode(osg::StateAttribute::DEPTH,osg::StateAttribute::OFF);
	nodesRoot->setStateSet(stateset);

/*	
	stateset= new osg::StateSet();
	osg::PolygonMode *pm= new osg::PolygonMode();
	pm->setMode(osg::PolygonMode::Face::FRONT_AND_BACK,osg::PolygonMode::Mode::LINE);
	stateset->setAttributeAndModes(pm);
//	stateset->setAttributeAndModes(new osg::Depth(osg::Depth::NEVER));
	srtmRoot->setStateSet(stateset);
*/
	mapsRoot->addChild(osgDB::readNodeFile("maps0.osg"));
//	mapsRoot->addChild(osgDB::readNodeFile("maps0.ive"));
	stateset= new osg::StateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	mapsRoot->setStateSet(stateset);

	textureMatrix.makeIdentity();
	

	osg::ref_ptr<TerrainMaterial> tm;
	std::ifstream file("default.mat", std::ios::in);
	char buf[256];
	file.getline(buf,255);
	file.getline(buf,255);
	unsigned int n=0;
	file >> n;
	materials.resize(n);
//	for (unsigned int i=0; i<n; i++)
//		materials[i]= NULL;
	
	while (!file.eof())
	{
		unsigned int UID= 0;
		file >> UID;

//		if (tm->UID<n && !materials[tm->UID].valid())
		if (UID>0 && UID<n && !materials[UID].valid())
		{
			tm= new TerrainMaterial();
			tm->UID= UID;
			tm->load(file);
			//delete tm;
//			SafeDelete(tm);
			materials[UID]= tm.get();
		}
		else MessageBox(0,"!","!",MB_OK);
	}
	
	file.close();
	selectMaterial( materials.size()>1 ? 1 : -1 );
	
	selectModel(0);

//	changeMode(em_Select);
//	changeMode(em_Move);
//	changeMode(em_CreateTracks);
//	changeMode(em_CreatePoints);

	iveReaderWriter= osgDB::Registry::instance()->getReaderWriterForExtension("ive");

	pickVisitor= new PickLinesAndPoints();

	CreateVisuals();

	registerClass(PropertySet);
	registerClass(edPointFeature);
	registerClass(edTemplateTrack);
	registerClass(edFlexTrack);
	registerClass(edFenceFeature);
	registerClass(edWireFeature);
	registerClass(edRailLine);
	registerClass(edTerrainNode);
	registerClass(edLine);
	registerClass(edPoint);
	registerClass(edMilePost);
	registerClass(edSignal);
	registerClass(edSCNImport);
	setKitOwnerWithWrite(edSCNImport::instance());
//	registerClass(edRailLine);
//	addKit('PTFT',edPointFeature::getClassProps());
//	addKit('FTRK',edFlexTrack::getClassProps());
//	addKit('FNFT',edFenceFeature::getClassProps());
//	addKit('RLLN',edRailLine::getClassProps());
//	addKit('TERR',edTerrainNode::getClassProps());

	selectedNodesState= new osg::StateSet;
	osg::PolygonMode *pm= new osg::PolygonMode();
	pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	selectedNodesState->setAttributeAndModes(pm);
	selectedNodesState->setAttributeAndModes(new osg::LineWidth(3));
    selectedNodesState->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    selectedNodesState->setRenderBinDetails(20,"RenderBin");
	osg::Material *mater= new osg::Material();
	mater->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
	mater->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//					mater->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,1.0,1.0));
	selectedNodesState->setAttributeAndModes(mater);

	selectedNode= NULL;

}

#undef registerClass

Editor::~Editor()
{
}

void Editor::freeNodes()
{
	deselectNode();
	CollectNodes cn(0xFFFFFFFF);
		nodesRoot->accept(cn);
		terrainRoot->accept(cn);
		//	edNode *node;
			for (unsigned int i=0; i<cn.terrainNodesList.size(); i++)
			{
				cn.terrainNodesList[i]->free();
				cn.terrainNodesList[i]= NULL;
			}
			cn.terrainNodesList.clear();
			for (unsigned int i=0; i<cn.lnsList.size(); i++)
			{
				cn.lnsList[i]->free();
				cn.lnsList[i]= NULL;
			}
			cn.lnsList.clear();
			for (unsigned int i=0; i<cn.ptsList.size(); i++)
				if (!cn.ptsList[i]->locked())
				{
					cn.ptsList[i]->free();
					cn.ptsList[i]= NULL;
				}
			cn.ptsList.clear();
			for (unsigned int i=0; i<cn.trackNodesList.size(); i++)
			{
				cn.trackNodesList[i]->free();
				cn.trackNodesList[i]= NULL;
			}
			cn.trackNodesList.clear();
			cn.ttrackNodesList.clear();

	redrawAll();
}

osg::Node* Editor::createStatusBar(unsigned int n)
{
/*
    osg::Geode* geode = new osg::Geode();
    

    std::string timesFont("arial.ttf");

    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    stateset->setRenderBinDetails(11,"RenderBin");

    osg::Vec3 position(10.0f,1000.0f,0.0f);
    osg::Vec3 delta(150.0f,0.0f,0.0f);


	statusBar.reserve(n+1);
	for (unsigned int i=0; i<n; i++)
    {
        osgText::Text* text = new  osgText::Text;
		statusBar.push_back(text);
        geode->addDrawable( text );

        text->setFont(timesFont);
		text->setCharacterSize(20);
		text->setColor(osg::Vec4(1,0,0,1));
        text->setPosition(position);
        text->setText("test");
        position += delta;
    }    

	position.set(10.0f,975.0f,0.0f);
        osgText::Text* text = new  osgText::Text;
		statusBar.push_back(text);
        geode->addDrawable( text );

        text->setFont(timesFont);
		text->setCharacterSize(20);
		text->setColor(osg::Vec4(1,0,0,1));
        text->setPosition(position);
//        text->setText("test");

    // create the hud.
    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
	modelview_abs->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    modelview_abs->setMatrix(osg::Matrix::identity());
    modelview_abs->addChild(geode);

    osg::Projection* projection = new osg::Projection;
    projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024));
    projection->addChild(modelview_abs);
	projection->setNodeMask(0x0000FF00);

    return projection;
	*/
	return NULL;

}

void Editor::CreateVisuals()
{
	osg::Geode *geode= NULL;
	osg::StateSet *state_set= NULL;
	osg::Material *material= NULL;
	osg::Cylinder *cyl= NULL;
	if (!selectedPointVisual.valid() && !deselectedPointVisual.valid() && !markedPointVisual.valid())
	{

		osg::Geometry *geom= new osg::Geometry();
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,4));

		osg::Vec3Array *v= new osg::Vec3Array();
		osg::Vec3Array *norm= new osg::Vec3Array();
		v->reserve(4);
		v->push_back(osg::Vec3( 6, 0, 0));
		v->push_back(osg::Vec3(-7, 0, 0));
		v->push_back(osg::Vec3( 0, 6, 0));
		v->push_back(osg::Vec3( 0,-7, 0));

		norm->reserve(1);
		norm->push_back(osg::Vec3(0,0,1));
		geom->setVertexArray( v );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
		geom->setUseDisplayList(true);


		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		selectedPointVisual= geode;

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		deselectedPointVisual= geode;

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		markedPointVisual= geode;
	}
	/*
	if (!deselectedPointVisual.valid())
	{
		geode= new osg::Geode();
		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		deselectedPointVisual= geode;
	}*/
	if (!selectedLineVisual.valid() && !deselectedLineVisual.valid())
	{
		/*
		cyl= new osg::Cylinder(osg::Vec3(0,0,0),5.0,1.0);
//		cyl= new osg::Cylinder(osg::Vec3(0,0,0),1.0,1.0);
		osg::Quat quat;
		quat.makeRotate(osg::Vec3d(0,0,1),osg::Vec3d(1,0,0));
		cyl->setRotation(quat);*/

		osg::Geometry *geom= new osg::Geometry();
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,2));
//		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,2,4));

		osg::Vec3Array *v= new osg::Vec3Array();
		osg::Vec3Array *norm= new osg::Vec3Array();
		v->reserve(2);
		v->push_back(osg::Vec3( 0.5, 0, 0));
		v->push_back(osg::Vec3(-0.5, 0, 0));

//		v->push_back(osg::Vec3( 0.5,-3, 0));
//		v->push_back(osg::Vec3(-0.5,-3, 0));
//		v->push_back(osg::Vec3(-0.5, 3, 0));
//		v->push_back(osg::Vec3( 0.5, 3, 0));
		norm->reserve(1);
		norm->push_back(osg::Vec3(0,0,1));
		geom->setVertexArray( v );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
//		geode->addDrawable(new osg::ShapeDrawable(cyl));
		geode->addDrawable(geom);
		state_set= new osg::StateSet();
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(3));
		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		state_set->setRenderBinDetails(9,"RenderBin");

		geode->setStateSet(state_set);
		selectedLineVisual= geode;

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
//		geode->addDrawable(new osg::ShapeDrawable(cyl));
		geode->addDrawable(geom);
		state_set= new osg::StateSet();
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,0.0,1.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,0.0,1.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(3));
		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		state_set->setRenderBinDetails(9,"RenderBin");
		geode->setStateSet(state_set);
		deselectedLineVisual= geode;
	}
//	selectedLineVisual, deselectedLineVisual
	if (!selectedConnectionVisual.valid() && !deselectedConnectionVisual.valid() && !connectedConnectionVisual.valid())
	{
		osg::Geometry *geom= new osg::Geometry();
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));

		osg::Vec3Array *v= new osg::Vec3Array();
		osg::Vec3Array *norm= new osg::Vec3Array();
		v->reserve(6);
		v->push_back(osg::Vec3( 6, 0, 0));
		v->push_back(osg::Vec3(-7, 0, 0));
		v->push_back(osg::Vec3( 0, 6, 0));
		v->push_back(osg::Vec3( 6, 0, 0));
		v->push_back(osg::Vec3( 6, 0, 0));
		v->push_back(osg::Vec3( 0,-7, 0));

		norm->reserve(1);
		norm->push_back(osg::Vec3(0,0,1));
		geom->setVertexArray( v );
		geom->setNormalArray( norm );
		geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
		geom->setUseDisplayList(true);


		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		selectedConnectionVisual= geode;

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,0.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		deselectedConnectionVisual= geode;

		geode= new osg::Geode();
		geode->setNodeMask(nm_Nodes);
		geode->addDrawable(geom);
//		geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),5.0)));
		state_set= new osg::StateSet();
//		state_set->setMode( GL_RESCALE_NORMAL, osg::StateAttribute::ON );
		material= new osg::Material();
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		material->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(0.0,1.0,1.0,1.0));
		state_set->setAttributeAndModes(material);
		state_set->setAttributeAndModes(new osg::LineWidth(1));
		state_set->setRenderBinDetails(10,"RenderBin");
//		state_set->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		geode->setStateSet(state_set);
		connectedConnectionVisual= geode;
	}
}

bool Editor::changeMode(EditorMode _mode)
{
	mode= _mode;
	switch (mode)
	{
		case em_Select:					activateKit(unsigned int(0));				/*statusBar[0]->setText("SELECT");*/ break;
		case em_Move:					activateKit(unsigned int(0));				/*statusBar[0]->setText("MOVE");*/ break;
		case em_CreatePoints:			activateKit(edPoint::getClassID());			/*statusBar[0]->setText("CREATE POINTS");*/ break;
		case em_CreatePointFeatures:	activateKit(edPointFeature::getClassID());	/*statusBar[0]->setText("CREATE POINT FEATURES");*/ break;
		case em_CreateLines:			activateKit(edLine::getClassID());			/*statusBar[0]->setText("CREATE LINES");*/ break;
		case em_ConnectPoints:			activateKit(edFlexTrack::getClassID());		/*statusBar[0]->setText("CONNECT TRACKS");*/ break;
		case em_CreateRailLines:		activateKit(edRailLine::getClassID());		/*statusBar[0]->setText("CREATE RAIL LINES");*/ break;
		case em_CreateTracks:			activateKit(edTemplateTrack::getClassID());	/*statusBar[0]->setText("CREATE TRACKS");*/ break;
//		case em_CreateTracks:			activateKit(edFlexTrack::getClassID());		/*statusBar[0]->setText("CREATE TRACKS");*/ break;
		case em_CreateSignals:			activateKit(edSignal::getClassID());		/*statusBar[0]->setText("CREATE SIGNALS");*/ break;
		case em_CreateFence:			activateKit(edFenceFeature::getClassID());	/*statusBar[0]->setText("CREATE FENCE FEATURE");*/ break;
		case em_CreateCatenary:			activateKit(edWireFeature::getClassID());	/*statusBar[0]->setText("CREATE CATENARY");*/ break;
		case em_Divide:					activateKit(unsigned int(0));				/*statusBar[0]->setText("DIVIDE");*/ break;
		case em_Fill:					activateKit('TERR');						/*statusBar[0]->setText("FILL");*/ break;
		case em_Import:					activateKit(edSCNImport::instance());		/*statusBar[0]->setText("IMPORT");*/ break;
	}
	mainView->exposed();
	return true;
}

void Editor::selectNode(edNode *node, osg::Geode *geom)
{
	deselectNode();
	selectedNode= node;
	updateNodeDesc();
	if (node)
		node->onSelect(geom);
}

void Editor::deselectNode()
{
	edNode *node= selectedNode.get();
	if (node)
		node->onDeselect();
	selectedNode= NULL;
	updateNodeDesc();
	activateKit(unsigned int(0));
}

void Editor::updateNodeDesc()
{
	static char buf[1024];
	if (selectedNode.valid())
	{
		selectedNode->getNodeDesc(buf);
		Publisher::set_status_msg(5,buf);
//		statusBar[4]->setText(buf);
	}
	else
	{
		Publisher::set_status_msg(5,"");
//		statusBar[4]->setText("");
	}
}

void Editor::selectMaterial(int i)
{
	printf("select material!\n");
	if (i<0 || !materials[i].valid())
	{
		material= NULL;
//		Publisher::set_status_msg(4,"blend");
//		statusBar[7]->setText("blend");
	}
	else
	{
		material= materials[i].get();
//		Publisher::set_status_msg(4,material->tex.c_str());
//		statusBar[7]->setText(material->tex);
		
	}
}

void Editor::selectModel(int i)
{
	static std::string models[]= { "szopastara1.ive","tree100.ive","powermst1.ive","pompka.ive" };
	if (i>=0 && i<4)
	{
		model= models[i];
//		statusBar[6]->setText(model.c_str());
		edPointFeature *ptftr= dynamic_cast<edPointFeature*>(selectedNode.get());
		if (ptftr)
		{
			ptftr->set("model",model.c_str());
//			for (PropertySet::Properties::iterator it= ptftr->getProps().begin(); it!=ptftr->getProps().end(); it++)
//				it->second.set(model);
//				it->second.getType();
//			ptftr->setModel(model.c_str());
		}

	}
}

edPoint *Editor::createPoint(osg::Vec3d pos)
{
	edPoint *pt= NULL;
//	osg::MatrixTransform *mt= NULL;
//	osg::AutoTransform *at= NULL;
//	osg::Switch *sw= NULL;
//	CreateVisuals();
//	mt= new osg::MatrixTransform();
//	at= new osg::AutoTransform();
//	at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//	at->setAutoScaleToScreen(true);
//	sw= new osg::Switch();
//	sw->addChild(new osg::Geode(*deselectedPointVisual.get()),true);
//	sw->addChild(new osg::Geode(*selectedPointVisual.get()),false);
//	mt->addChild(sw);
//	nodesRoot->addChild(mt);
//	at->addChild(sw);
//	nodesRoot->addChild(at);
	pt= new edPoint();
//	pt->setVisual(at,sw);
//	pt->setVisual(mt,sw);
	pt->moveTo(pos);
	return pt;
}

edLine *Editor::createLine()
{
//	osg::AutoTransform *at= NULL;
//	osg::Switch *sw= NULL;
	edLine *ln= NULL;
	/*
	CreateVisuals();
	at= new osg::AutoTransform();
	at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
	at->setAutoScaleToScreen(false);
	sw= new osg::Switch();
	sw->addChild(new osg::Geode(*deselectedLineVisual.get()),true);
	sw->addChild(new osg::Geode(*selectedLineVisual.get()),false);
	at->addChild(sw);
	nodesRoot->addChild(at);*/
	ln= new edLine();
//	ln->setVisual(at,sw);
	return ln;
}

edLine *Editor::createLine(edPoint *pt1, edPoint *pt2, double maxLen, bool heightFromSRTM)
{
	edLine *ln= NULL;
	edPoint *pt= NULL;
	edPoint *ppt= NULL;
	osg::Vec3d pos;
	if (pt1 && pt2 && pt2!=pt1 && pt1->getFlag(nf_ExportToDelanuay) && pt2->getFlag(nf_ExportToDelanuay) && !pt2->connected(pt1))
	{
		double len= (pt1->getPosition()-pt2->getPosition()).length();
		unsigned int n= len/maxLen;
		double t;
		pt= pt1;
		for (unsigned int i=1; i<n; i++)
		{
			t= (double)i/(double)n;
			ppt= pt;
			pos= pt1->getPosition()*(1.0-t) + pt2->getPosition()*t;
			if (heightFromSRTM)
				pos.z()= Editor::instance()->srtmData.getHeight(pos.x(),pos.y());
			pt= createPoint(pos);
			ln= createLine();
			ln->setPoints(pt,ppt);
			ln->update(NULL);
		}
		ln= createLine();
		ln->setPoints(pt,pt2);
		ln->update(NULL);
	}
	return ln;
}

edRailLine *Editor::createRailLine()
{
//	osg::AutoTransform *at= NULL;
//	osg::Switch *sw= NULL;
	edRailLine *ln= NULL;
//	at= new osg::AutoTransform();
//	at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//	at->setAutoScaleToScreen(false);
//	sw= new osg::Switch();
//	at->addChild(sw);
//	nodesRoot->addChild(at);
	ln= new edRailLine();
//	ln->setVisual(at,sw);
	return ln;
}

edRailLine *Editor::createRailLine(edPoint *pt1, edPoint *pt2)
{
	edRailLine *ln= NULL;
	if (pt1 && pt2 && pt2!=pt1 && !pt2->connected(pt1))
	{
		ln= createRailLine();
		ln->setPoints(pt1,pt2);
		ln->update(NULL);
	}
	return ln;
}

edFenceFeature *Editor::createFenceFeature(edPoint *pt1, edPoint *pt2)
{
	edFenceFeature *ln= NULL;
	if (pt1 && pt2 && pt2!=pt1 && !pt2->connected(pt1) && (pt1->getPosition()-pt2->getPosition()).length()<200)
	{
		ln= new edFenceFeature();
		ln->setPoints(pt1,pt2);
		ln->update(NULL);
		ln->applyFeature();
	}
	return ln;
}

edFlexTrack *Editor::createFlexTrack(osg::Vec3d pt1, osg::Vec3d cpt1, osg::Vec3d cpt2, osg::Vec3d pt2, 
							double roll1, double roll2, Segment::ShapesList &bs, Segment::ShapesList &rs)
{
	osg::Vec3d pos((pt1+cpt1+cpt2+pt2)*0.25);
	edFlexTrack *ftrk= new edFlexTrack();
	ftrk->setBezier(pt1-pos,cpt1-pos,cpt2-pos,pt2-pos,roll1,roll2);
	ftrk->setBallastShape(bs);
	ftrk->setRailsShape(rs);
	ftrk->moveTo(pos);
	return ftrk;
}

bool adjust(edPoint *pt, edWireFeature *wf)
{
	for (edPoint::LinesList::iterator it=pt->getLinesList().begin(); 
		it!=pt->getLinesList().end(); it++)
	{
		edWireFeature *wf2= dynamic_cast<edWireFeature*>(*it);
		if (wf2!=wf && wf2!=NULL)
		{
			osg::Quat q;

//		osg::Vec3d dir= (osg::Vec3d(1,0,0)*wf->getRotation());
//		dir.normalize();
//		osg::Quat quat;
		osg::Vec3d d1(1,0,0);
		osg::Vec3d d2(wf->getRotation()*osg::Vec3d(1,0,0)+wf2->getRotation()*osg::Vec3d(1,0,0)); d2.z()= 0; d2.normalize();		
		
		if ( fabs(d1*d2 + 1.0) < 0.0000001 )
			q.makeRotate(osg::DegreesToRadians(180.0),0,0,1);
		else
			q.makeRotate(d1,d2);


/*		if (dir.x()>0)
			q.makeRotate(osg::Vec3d(1,0,0),dir);
		else
			q.makeRotate(osg::Vec3d(-1,0,0),dir);*/

//			q.slerp(0.5,wf2->getRotation(),wf->getRotation());
			if (wf2->getPt1()==wf->getPt2() || wf->getPt1()==wf2->getPt2())
				q*= osg::Quat(osg::DegreesToRadians(90.0),osg::Vec3d(0,0,1));
			pt->setRotation(q);
			return true;
		}
	}
	return false;
}

edWireFeature *Editor::createWireFeature(edPoint *pt1, edPoint *pt2)
{
	edWireFeature *wf= NULL;
	if (pt1 && pt2 && pt2!=pt1 && !pt2->connected(pt1) && (pt1->getPosition()-pt2->getPosition()).length()<200 &&
		dynamic_cast<edPointFeature*>(pt1) && dynamic_cast<edPointFeature*>(pt2))
	{
		wf= new edWireFeature();
		wf->setPoints(pt1,pt2);
		wf->update(NULL);
		if (!adjust(pt1,wf))
			pt1->setRotation(wf->getRotation()*osg::Quat(osg::DegreesToRadians(90.0),osg::Vec3d(0,0,1)));
		pt1->onMoved();
		if (!adjust(pt2,wf))
			pt2->setRotation(wf->getRotation()*osg::Quat(osg::DegreesToRadians(90.0),osg::Vec3d(0,0,1)));
		pt2->onMoved();
//		adjust(pt2,wf);
		wf->applyFeature();
	}
	return wf;
}


bool Editor::loadFromFile(const char *fileName)
{
	disableRedraws();
//	return false;
	unsigned int magic= 0x00000000;
	int ver= -1;
	std::ifstream file;

	try {
		file.open(fileName, std::ios::in | std::ios::binary);
		file.read((char*)&magic,sizeof(magic));
		if (magic!=dwMagic)
			throw "wrong file type";
		file.read((char*)&ver,sizeof(ver));

		unsigned int numPts= 0;
		file.read((char*)&numPts,sizeof(numPts));
		if (numPts==0)
			throw "0 points";

		freeNodes();

		unsigned int type;
		edPoint *pt;
		CollectNodes cn(0xFFFFFFFF);
		cn.ptsList.resize(numPts);
		for (unsigned int i=0; i<numPts; i++)
		{
			file.read((char*)&type,sizeof(type));
			switch (type)
			{
				case '_PT_':
					pt= createPoint(osg::Vec3d(0,0,0));
				break;
				case 'PTFT':
					pt= new edPointFeature();
				break;
				case 'MLPS':
					pt= new edMilePost();
				break;
				case 'SGNL':
					pt= new edSignal();
				break;
//				case '':
//				break;
				case 0: pt= NULL; break;
				default: throw "invalid point";
			}
			cn.ptsList[i]= pt;
			if (pt)
			{
				pt->load(file,ver,&cn);
				pt->updateVisual();
			}
		}

//		nodesRoot->accept(cn);
		unsigned int numLns= 0;
		file.read((char*)&numLns,sizeof(numLns));
		edLine *ln;
		for (unsigned int i=0; i<numLns; i++)
		{
			file.read((char*)&type,sizeof(type));
			switch (type)
			{
				case 'LINE':
					ln= createLine();
				break;
				case 'RLLN':
					ln= createRailLine();
				break;
				case 'FNFT':
					ln= new edFenceFeature();
				break;
				case 'WRFT':
					ln= new edWireFeature();
				break;
//				case '':
//				break;
				default: throw "invalid line";
			}
			ln->load(file,ver,&cn);
			ln->updateVisual();
		}

		unsigned int numTrn= 0;
		file.read((char*)&numTrn,sizeof(numTrn));
		edTerrainNode *tn;
		std::vector< edTerrainNode* > blendTerrains;
		blendTerrains.reserve(1000);
		for (unsigned int i=0; i<numTrn; i++)
		{
			file.read((char*)&type,sizeof(type));
			switch (type)
			{
				case 'TERR':
					tn= new edTerrainNode();
				break;
//				case '':
//				break;
				default: throw "invalid terrain node";
			}
			tn->load(file,ver,&cn);
			if (tn->getMaterialI()>0)
				tn->updateVisual();
			else
				blendTerrains.push_back(tn);
		}
		for (unsigned int i=0; i<blendTerrains.size(); i++)
			blendTerrains[i]->updateVisual();
		unsigned int numTrks= edNode::readUI(file);
		edTrack *trk= NULL;
		cn.trackNodesList.reserve(numTrks);
		for (unsigned int i=0; i<numTrks; i++)
		{
			trk= NULL;
			file.read((char*)&type,sizeof(type));
			switch (type)
			{
				case 'FTRK':
					trk= new edFlexTrack();
				break;
				case 'TMTR':
					trk= new edTemplateTrack();
				break;
				default: throw "invalid terrain node";
			}
			cn.trackNodesList.push_back(trk);
			if (trk)
			{
				trk->load(file,ver,&cn);
				trk->updateVisual();
			}
		}
	} 
	catch (char *txt) {
		printf(txt);
		file.close();
		return false;
	}
	file.close();
	enableRedraws();
	redrawAll();
	return true;
}

bool Editor::saveToFile(const char *fileName)
{
	deselectNode();
	int unsigned val;
	std::ofstream file;
	try {
		file.open(fileName, std::ios::out | std::ios::binary);
		file.write((char*)&dwMagic,sizeof(dwMagic));
		file.write((char*)&version,sizeof(version));
		CollectNodes cn(0xFFFFFFFF);
		nodesRoot->accept(cn);
		terrainRoot->accept(cn);
		if (!cn.ptsList.empty())
		{
			val= cn.ptsList.size();
			file.write((char*)&val,sizeof(val));
			for (unsigned int i=0; i<cn.ptsList.size(); i++)
			{
				if (cn.ptsList[i]->getFlag(nf_NoSave))
					val= 0;
				else
					val= cn.ptsList[i]->getType();
				file.write((char*)&val,sizeof(val));
				if (val!=0)
					cn.ptsList[i]->save(file);
			}
			val= cn.lnsList.size();
			file.write((char*)&val,sizeof(val));
			for (unsigned int i=0; i<cn.lnsList.size(); i++)
			{
				val= cn.lnsList[i]->getType();
				file.write((char*)&val,sizeof(val));
				cn.lnsList[i]->save(file);
			}
			val= cn.terrainNodesList.size();
			file.write((char*)&val,sizeof(val));
			for (unsigned int i=0; i<cn.terrainNodesList.size(); i++)
			{
				val= cn.terrainNodesList[i]->getType();
				file.write((char*)&val,sizeof(val));
				cn.terrainNodesList[i]->save(file);
			}
			edNode::write(file,cn.trackNodesList.size());
			for (unsigned int i=0; i<cn.trackNodesList.size(); i++)
			{
				edNode::write(file,cn.trackNodesList[i]->getType());
				cn.trackNodesList[i]->save(file);
			}
		}
	} 
	catch (...) {
		printf("exception :(\n");
		file.close();
		return false;
	}
	file.close();
	return true;
}

class PagedLODSave : public osg::NodeVisitor
{
public:
	PagedLODSave(const char *_dir) : NodeVisitor(TRAVERSE_ALL_CHILDREN), dir(_dir) {};
	virtual void apply(osg::Node& node) 
	{
		traverse(node); 
	
	}
	virtual void apply(osg::PagedLOD& node) 
	{
		unsigned int i;
		for (i=0; i<node.getNumChildren(); i++)
			if (!node.getFileName(i).empty())
			{
				
				osgUtil::Optimizer optimizer;
				optimizer.optimize(node.getChild(i),
					osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS|
					osgUtil::Optimizer::REMOVE_REDUNDANT_NODES|
//					osgUtil::Optimizer::COMBINE_ADJACENT_LODS|
					osgUtil::Optimizer::SHARE_DUPLICATE_STATE|
					osgUtil::Optimizer::MERGE_GEOMETRY|
					osgUtil::Optimizer::CHECK_GEOMETRY|
					osgUtil::Optimizer::SPATIALIZE_GROUPS|
//					osgUtil::Optimizer::COPY_SHARED_NODES|
                    osgUtil::Optimizer::TRISTRIP_GEOMETRY|
					osgUtil::Optimizer::TESSELATE_GEOMETRY|
					osgUtil::Optimizer::OPTIMIZE_TEXTURE_SETTINGS);
				osgDB::writeNodeFile(*node.getChild(i),dir+node.getFileName(i));
			}
//		traverse(node); 
	}
private:
	std::string dir;
};

bool Editor::exportToDirectory(const char *dirName)
{
	char *deleteDir= new char[strlen(dirName)+10];
	strcpy(deleteDir,dirName);
	strcat(deleteDir,"*.*");
	deleteDir[strlen(deleteDir)+1]= 0;
/*
	SHFILEOPSTRUCT fos;
	memset(&fos,0,sizeof(fos));
	fos.wFunc= FO_DELETE;
	fos.pFrom= deleteDir;
	fos.fFlags= FOF_NOCONFIRMATION;
	int r= SHFileOperation(&fos);
	delete[] deleteDir;*/
	std::string fileName;

	fileName= dirName;
	fileName.append("main.scn");

	deselectNode();
	std::ofstream file;
	try {
		file.open(fileName.c_str(), std::ios::out | std::ios::binary);
		file.write((char*)&dwMagic,sizeof(dwMagic));
		file.write((char*)&version,sizeof(version));
		CollectNodes cn(0xFFFFFFFF);
		nodesRoot->accept(cn);
		terrainRoot->accept(cn);

		edSignal *sig= NULL;
		std::vector<edSignal*> signals;
		signals.reserve(2000);
		for (unsigned int i=0; i<cn.ptsList.size(); i++)
		{
			sig= dynamic_cast<edSignal*>(cn.ptsList[i]);
			if (sig)
			{
				sig->signalID= signals.size();
				signals.push_back(sig);
			}
		}

		edNode::write(file,signals.size());
		for (unsigned int i=0; i<signals.size(); i++)
			signals[i]->export(file);
//		typedef std::map<std::string, edSignal>

		unsigned int id= 0;
		for (unsigned int i=0; i<cn.trackNodesList.size(); i++)
			cn.trackNodesList[i]->setupTrackIDs(id);

		edNode::write(file,id);
		for (unsigned int i=0; i<cn.trackNodesList.size(); i++)
			cn.trackNodesList[i]->export(file);

		edNode::write(file,cn.ttrackNodesList.size());
		for (unsigned int i=0; i<cn.ttrackNodesList.size(); i++)
			cn.ttrackNodesList[i]->exportTemplate(file);


	} 
	catch (...) {
		printf("exception :(\n");
		file.close();
		return false;
	}
	file.close();

	fileName= dirName;
	fileName.append("main.ive");
//	Editor::releaseGeometry= false;
	osgDB::writeNodeFile(*terrainRoot.get(),fileName);
	terrainRoot->accept(PagedLODSave(dirName));
//	Editor::releaseGeometry= true;
	

	return true;
}

osg::MatrixTransform *Editor::getCell(int x, int y)
{
	static char buf[256];
	nameFromCoords5(buf,x,y);
	for (unsigned int i=0; i<terrainRoot->getNumChildren(); i++)
		if (terrainRoot->getChild(i)->getName().compare(buf)==0)
		{
			return dynamic_cast<osg::MatrixTransform*>(terrainRoot->getChild(i)->asGroup()->getChild(0));
//			return dynamic_cast<osg::Group*>(terrainRoot->getChild(i)->asGroup()->getChild(0)->asGroup()->getChild(0));
		}
	return NULL;
}

osg::MatrixTransform *Editor::getOrCreateCell(int x, int y)
{
	static char buf[256];
	osg::MatrixTransform *trans= getCell(x,y);
	if (!trans)
	{
		osg::PagedLOD *lod= new osg::PagedLOD();
		osg::Matrixd mat;
		mat.makeTranslate(getCellX(x),getCellY(y),0);
		trans= new osg::MatrixTransform(mat);
		nameFromCoords5(buf,x,y);
		lod->setName(buf);
		lod->setCenterMode(osg::LOD::CenterMode::USER_DEFINED_CENTER);
		lod->setCenter(mat.getTrans());
		terrainRoot->addChild(lod);
		strcat(buf,".ive");
		lod->addChild(trans,0,6000,buf);		
	}
	return trans;
}

void Editor::addObject(osg::MatrixTransform *mt, float minDist, float maxDist)
{
	osg::Matrixd mat(mt->getMatrix());
	osg::Vec3d pos= mat.getTrans();
	osg::MatrixTransform *gmt= getOrCreateCell(getCellX(pos.x()),getCellY(pos.y()));
	mat.setTrans(pos-gmt->getMatrix().getTrans());
	mt->setMatrix(mat);
	addObject(gmt,mt,minDist,maxDist);
	//mat.setTrans(

}

double maxRad= 100;
void Editor::addObject(osg::MatrixTransform *gmt, osg::MatrixTransform *mt, float minDist, float maxDist)
{
	char buf[8];
	static const double subCellDim= 100;
//	gmt->ad
	osg::Vec3d pos(mt->getMatrix().getTrans());
	int x= floor(pos.x()/subCellDim);
	int y= floor(pos.y()/subCellDim);
//	printf("pos: %f %f %d %d\n",pos.x(),pos.y(),x,y);
	x= clamp(x,-5,4)+5;
	y= clamp(y,-5,4)+5;

	sprintf(buf,"%d%d",x,y);

	osg::LOD *lod= NULL;
	osg::Group *group= NULL;

	for (unsigned int i=0; i<gmt->getNumChildren(); i++)
		if (gmt->getChild(i)->getName().compare(buf)==0)
		{
			lod= dynamic_cast<osg::LOD*>(gmt->getChild(i));
			break;
		}
	if (lod)
	{
		for (unsigned int i=0; i<lod->getNumChildren(); i++)
			if (lod->getRangeList()[i].first==minDist && lod->getRangeList()[i].second==maxDist) 
			{
				group= dynamic_cast<osg::Group*>(lod->getChild(i));
				break;
			}
	}
	else
	{
		lod= new osg::LOD();
		lod->setName(buf);
		gmt->addChild(lod);
	}
	if (!group)
	{
		group= new osg::Group();
		lod->addChild(group,minDist,maxDist);
	}

	if (mt->getNumParents()>0)
		mt->getParent(0)->removeChild(mt);

	group->addChild(mt);

	mt->dirtyBound();

	if (lod->getBound().radius()>maxRad)
	{
		maxRad= lod->getBound().radius();
		printf("radius: %f\n",maxRad);
	}

}

osg::Vec3d Editor::getPointOnTerrain(double x, double y, bool useSRTM, bool useTerrain, int *whatHeight)
{
	osg::Vec3d retPt(x,y,0);
	osgUtil::IntersectVisitor::HitList hits;
//	if (useTerrain && mainView->computeIntersections(getWorldX(),getWorldY(),terrainRoot.get(), hits))
	if (whatHeight)
		*whatHeight= 0;
	if (useTerrain && mainView->computeIntersections(getWorldX(),getWorldY(),terrainRoot.get(), hits, nm_SolidTerrain))
	{
//		for (unsigned int i=0; i<hits.size(); i++)
//			hits[i]._nodePath.
		retPt.z()= hits[0].getWorldIntersectPoint().z();
		if (whatHeight)
			*whatHeight= 2;
		
	}
	else 
		if (useSRTM)
		{
			retPt.z()= srtmData.getHeight(x,y);
			if (whatHeight)
				*whatHeight= 1;
		}
	return retPt;
}

bool Editor::onPush(unsigned int button, unsigned int state)
{
	edPointFeature *pf= NULL;
	edPoint *pt= NULL;
	edPoint *pt2= NULL;
	edTrack *trk= NULL;
	edFlexTrack *ftrk= NULL;
	edTemplateTrack *ttrk= NULL;

	osg::MatrixTransform *mt= NULL;
	osg::Matrixd mat;
	osg::Vec3d hitPt;
	osg::Switch *sw= NULL;
	osgUtil::IntersectVisitor::HitList hits;
	const edRailLine *rl= NULL;
	if (button==3)
		deselectNode();
	switch (mode)
	{
		case em_Select:
		case em_Move:
		case em_Rotate:
		case em_ConnectPoints:
			if (button==1)
				if (pickVisitor->pick(osg::Vec2d(getWorldX(),getWorldY()),0.01*mainView->getScale(),nodesRoot.get()))
					selectNode(pickVisitor->pickedNode.get());
				else 
//					if (viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), 0, terrainRoot.get(), hits))
					if (mainView->computeIntersections(getWorldX(), getWorldY(), terrainRoot.get(), hits))
						selectNode(dynamic_cast<edNode*>(hits[0]._geode->getUserData()),hits[0]._geode.get());
					else
//						if (mainView->computeIntersections(getWorldX(), getWorldY(), dynamicModelsRoot.get(), hits))
//							selectNode(dynamic_cast<edNode*>(hits[0]._geode->getUserData()),hits[0]._geode.get());
//						else
							deselectNode();
			redrawAll();

		break;
		case em_Import:
			if (button==1)
			{
				edSCNImport::instance()->import(getPointOnTerrain(getWorldX(),getWorldY(),true,true),0);
				redrawAll();
			}
		break;
		case em_CreateSignals:
		case em_CreatePoints:
		case em_CreatePointFeatures:
		case em_CreateTracks:
			if (button==1)
//				&&				viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), srtmRoot.get(), hits))
			{
				int what= 0;	//terrain or srtm, HACK :(
				hitPt= getPointOnTerrain(getWorldX(),getWorldY(),true,true,&what);
//				hitPt.set(getWorldX(), getWorldY(),0);
//				hitPt.z()= srtmData.getHeight(hitPt.x(),hitPt.y());
				//hits[0].getWorldIntersectPoint();
//				for (unsigned int i=1; i<hits.size(); i++)
//					if (hits[i].getWorldIntersectPoint().z()>hitPt.z())
//						hitPt= hits[i].getWorldIntersectPoint();
				if (mode==em_CreatePoints)
					pt= createPoint(hitPt);
				else if (mode==em_CreatePointFeatures)
				{
					pf= new edPointFeature();
					
					pf->setModel(model.c_str());
/*
					osg::Billboard *billboard= new osg::Billboard();
					osg::Billboard360 *billb= new osg::Billboard360(10,10,0,8,8);
					billboard->addDrawable(billb);
//					billboard->addDrawable(billb,osg::Vec3f(10,10,0));
//					billboard->addDrawable(billb,osg::Vec3f(-10,10,0));
//					billboard->addDrawable(billb,osg::Vec3f(10,-10,0));
//					billboard->addDrawable(billb,osg::Vec3f(-10,-10,0));
					osg::Texture2D *tex= new osg::Texture2D(osgDB::readImageFile("tree000.dds",osgDB::Registry::CACHE_ALL));
					osg::StateSet *dstate= new osg::StateSet();
					dstate->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON );
					dstate->setAttributeAndModes(new osg::AlphaFunc(osg::AlphaFunc::GREATER,0.9), osg::StateAttribute::ON);
					billboard->setStateSet(dstate);
					osgDB::writeNodeFile(*billboard,"models\\tree100.ive");
					pf->setAddon(billboard);*/
					pf->moveTo(hitPt);
//					pf->getProp("terrain")->item.setText((what==2?"0":"1"));
//					pf->setTerrain((what==2?0:1));	//terrain or srtm, HACK :(
					pt= pf;
						
				}
				else if (mode==em_CreateTracks)
				{
					ttrk= new edTemplateTrack();
					setKitOwnerWithWrite(ttrk);
					ttrk->moveTo(hitPt);
					edTrackConnection *tc= dynamic_cast<edTrackConnection*>(selectedNode.get());
					if (tc)
					{
						ttrk->getConnection(0)->connectAndAllign(tc->getOwner());
						selectNode(ttrk->getConnection(1)->getNode());

					}
					else
						selectNode(ttrk);
					/*
					ftrk= new edFlexTrack();
					ftrk->setBezier(osg::Vec3f(0,50,0),osg::Vec3f(5,30,0),osg::Vec3f(5,-30,0),osg::Vec3f(0,-50,0),0,0);

					Segment::ShapesList bs(1);
					bs[0].reserve(4);
					bs[0].push_back(osg::Vec3f(-2.3,-0.2,0));
					bs[0].push_back(osg::Vec3f(-1.2,0.2,0.33));
					bs[0].push_back(osg::Vec3f(1.2,0.2,0.67));
					bs[0].push_back(osg::Vec3f(2.3,-0.2,1));
					ftrk->setBallastShape(bs);

					Segment::ShapesList rs(2);
					rs[0].reserve(4);
					rs[0].push_back(osg::Vec3d(-0.79,0.2,0));
					rs[0].push_back(osg::Vec3d(-0.79,0.38,0.44));
					rs[0].push_back(osg::Vec3d(-0.718,0.38,0.56));
					rs[0].push_back(osg::Vec3d(-0.718,0.2,1));
					rs[1].reserve(4);
					rs[1].push_back(osg::Vec3d( 0.718,0.2,1));
					rs[1].push_back(osg::Vec3d( 0.718,0.38,0.56));
					rs[1].push_back(osg::Vec3d( 0.79,0.38,0.44));
					rs[1].push_back(osg::Vec3d( 0.79,0.2,0));
					ftrk->setRailsShape(rs);

					ftrk->moveTo(hitPt);
					setKitOwnerWithWrite(ftrk);
					edTrackConnection *tc= dynamic_cast<edTrackConnection*>(selectedNode.get());
					if (tc)
					{
						ftrk->getConnection(0)->connectAndAllign(tc->getOwner());
						selectNode(ftrk->getConnection(1)->getNode());

					}
					else
						selectNode(ftrk);
						*/
				}
				else if (mode==em_CreateSignals)
				{
					edFlexTrack *trk= dynamic_cast<edFlexTrack*>(selectedNode.get());
					if (trk)
					{
						edSignal *sig= new edSignal();
						sig->moveTo(hitPt);
						sig->setTrackPiece(trk->getTrackPiece(0));

						pt= sig;
					}
				}
				if (pt)
				{
					setKitOwnerWithWrite(pt);
					selectNode(pt);
					if (pt==pf)
					{
						pf->setTerrain((what==2?0:1));	//terrain or srtm, HACK :(
						setKitOwnerWithRead(pf);
					}
				}
				redrawAll();
				return true;
			}
		break;
		case em_CreateLines:
		case em_CreateRailLines:
		case em_CreateFence:
		case em_CreateCatenary:
			if (button==1)
			{
				pt= NULL;
				pt2= dynamic_cast<edPoint*>(selectedNode.get());
//				if (viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), 0, nodesRoot.get(), hits))
				if (pickVisitor->pick(osg::Vec2d(getWorldX(),getWorldY()),0.01*mainView->getScale(),nodesRoot.get()))
					pt= dynamic_cast<edPoint*>(pickVisitor->pickedNode.get());
				else
				{
					hitPt= getPointOnTerrain(getWorldX(),getWorldY(),true,true);
	//				hitPt.set(getWorldX(), getWorldY(),0);
	//				hitPt.z()= srtmData.getHeight(hitPt.x(),hitPt.y());
					if (mode==em_CreateCatenary)
					{
						pf= new edPointFeature();
						std::string mdl(edWireFeature::getCurrentModel());
						if (mdl.empty())
							setKitOwnerWithWrite(pf);
						else
						{
							pf->setModel(mdl.c_str());
							edWireFeature::shiftModels();
						}
						pf->moveTo(hitPt);
						pt= pf;

					}
					else
						pt= createPoint(hitPt);
				}
				if (pt && pt2)
					if (mode==em_CreateLines)
						createLine(pt,pt2,( state&GDK_CONTROL_MASK ? 200 : 1000 ), !(state&GDK_SHIFT_MASK) );
					else if (mode==em_CreateRailLines)
					{
						bool p1OK,p2OK;
						p1OK=p2OK= false;

						if (pt->getLinesList().size()==0)
							p1OK= true;
						else  
						{
							rl= dynamic_cast<const edRailLine*>(pt->getLinesList().back());
							if (pt->getLinesList().size()==1 && rl && rl->getPt1()==pt)
								p1OK= true;
						}
						if (pt2->getLinesList().size()==0)
							p2OK= true;
						else  
						{
							rl= dynamic_cast<const edRailLine*>(pt2->getLinesList().back());
							if (pt2->getLinesList().size()==1 && rl && rl->getPt2()==pt2)
								p2OK= true;
						}
						if (p1OK && p2OK)
							setKitOwnerWithWrite(createRailLine(pt2,pt));
					}
					else if (mode==em_CreateFence)
						setKitOwnerWithWrite(createFenceFeature(pt,pt2));
					else if (mode==em_CreateCatenary)
						setKitOwnerWithWrite(createWireFeature(pt,pt2));
//				if (pt->getLinesList().size()==1)
					selectNode(pt);
//				else 
//					deselectNode();
				changeMode(mode);
				redrawAll();
				return true;
			}
		break;
		case em_Divide:
			if (button==1)
				if (pickVisitor->pick(osg::Vec2d(getWorldX(),getWorldY()),0.01*mainView->getScale(),nodesRoot.get()))
//				if (viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), 0, nodesRoot.get(), hits))
				{
					selectNode(pickVisitor->pickedNode.get());
//					selectNode(dynamic_cast<edNode*>(hits[0]._geode->getUserData()),hits[0]._geode.get());
					edLine *ln= dynamic_cast<edLine*>(selectedNode.get());
					if (ln && !ln->locked())
					{
						osg::Vec3d p1= ln->getPt1()->getPosition();
						osg::Vec3d p2= ln->getPt2()->getPosition();
						osg::Vec3d dir= p2-p1;
						double len= dir.length();
						double dist= (p1-hits[0].getWorldIntersectPoint()).length();
						edLine *ln2= ln->clone();
						if (ln2)
						{
							pt= createPoint(p1+dir/len*dist);
							ln2->setPoints(pt,ln->getPt2());
							ln2->update(NULL);
//						createLine(pt,ln->getPt2());
							ln->setPt2(pt);
							ln->update(NULL);
						}
					}
					redrawAll();
				}
		break;
		case em_Fill:
			if (button==1 && !mainView->computeIntersections(getWorldX(), getWorldY(), terrainRoot.get(), hits))
//				viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), srtmRoot.get(), hits))
			{
				hitPt= getPointOnTerrain(getWorldX(),getWorldY(),true,true);
//				hitPt.set(getWorldX(), getWorldY(),0);
//				hitPt.z()= srtmData.getHeight(hitPt.x(),hitPt.y());
					/*
				hitPt= hits[0].getWorldIntersectPoint();
				for (unsigned int i=1; i<hits.size(); i++)
					if (hits[i].getWorldIntersectPoint().z()>hitPt.z())
						hitPt= hits[i].getWorldIntersectPoint();*/
				fill(hitPt);
//				redrawAll();
				return true;
			}

		break;
	}
	return false;
}
bool Editor::onRelease(unsigned int button, unsigned int state)
{
	osg::MatrixTransform *mt= NULL;
	osg::Switch *sw= NULL;
	edPoint *pt= NULL;
	edPoint *sel_pt= NULL;
	edLine *ln= NULL;
	osgUtil::IntersectVisitor::HitList hits;
	osg::Vec3d hitPt,pos;
	edTrackConnection *c1= NULL;
	edTrackConnection *c2= NULL;
	switch (mode)
	{
		case em_Move:
			if (button==1 && selectedNode.valid() && !selectedNode->locked())
			{
				selectedNode->onMoved();
			}
		break;
		/*
		case em_Move:
			if (ea.getButton()==osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON && selectedNode.valid() && !selectedNode->locked())
			{
				pos= selectedNode->getMatrix().getTrans();//+osg::Vec3d(getMouseDX(),getMouseDY(),0.0);
				pos.z()= srtmData.getHeight(pos.x(),pos.y());
				selectedNode->moveTo(pos);
				updateNodeDesc();
			}

		break;*/
		case em_ConnectPoints:
			if (button==1)
				if (pickVisitor->pick(osg::Vec2d(getWorldX(),getWorldY()),0.01*mainView->getScale(),nodesRoot.get()))
//				if (viewer->computeIntersections(ea.getXnormalized(), ea.getYnormalized(), 0, nodesRoot.get(), hits))
				{
//					pt= dynamic_cast<edPoint*>(pickVisitor->pickedNode.get());
//					sel_pt= dynamic_cast<edPoint*>(selectedNode.get());

//					createLine(pt,sel_pt);

					c1= dynamic_cast<edTrackConnection*>(pickVisitor->pickedNode.get());
					c2= dynamic_cast<edTrackConnection*>(selectedNode.get());
					if (c1 && c2)
					{
						double dist= (c1->getPosition()-c2->getPosition()).length();
						if (dist>0.2)
						{
							dist/= 3;
							osg::Vec3d pos((c1->getPosition()+c2->getPosition())*0.5);
							edFlexTrack *ft= new edFlexTrack();
							osg::Vec3f pt1= c1->getPosition()-pos;
							osg::Vec3f pt2= c2->getPosition()-pos;
							osg::Vec3f cpt1= c1->getOwner()->getWorldDir();
							cpt1.normalize();
							cpt1= cpt1*dist+pt1;
							osg::Vec3f cpt2= c2->getOwner()->getWorldDir();
							cpt2.normalize();
							cpt2= cpt2*dist+pt2;

							ft->setBezier(pt1,cpt1,cpt2,pt2,0,0);

							edFlexTrack *ftrk= dynamic_cast<edFlexTrack*>(c1->getOwner()->getOwner());
							if (!ftrk)
								ftrk= dynamic_cast<edFlexTrack*>(c2->getOwner()->getOwner());

							if (ftrk)
							{
								ft->setBallastShape(ftrk->ballast);
								ft->setRailsShape(ftrk->rails);
							}
							else
							{
								Segment::ShapesList bs(1);
								bs[0].reserve(4);
								bs[0].push_back(osg::Vec3f(-2.3,-0.0,0));
								bs[0].push_back(osg::Vec3f(-1.2,0.2,0.33));
								bs[0].push_back(osg::Vec3f(1.2,0.2,0.67));
								bs[0].push_back(osg::Vec3f(2.3,-0.0,1));
								ft->setBallastShape(bs);

								Segment::ShapesList rs(2);
								rs[0].reserve(4);
								rs[0].push_back(osg::Vec3d(-0.79,0.2,0));
								rs[0].push_back(osg::Vec3d(-0.79,0.38,0.44));
								rs[0].push_back(osg::Vec3d(-0.718,0.38,0.56));
								rs[0].push_back(osg::Vec3d(-0.718,0.2,1));
								rs[1].reserve(4);
								rs[1].push_back(osg::Vec3d( 0.718,0.2,1));
								rs[1].push_back(osg::Vec3d( 0.718,0.38,0.56));
								rs[1].push_back(osg::Vec3d( 0.79,0.38,0.44));
								rs[1].push_back(osg::Vec3d( 0.79,0.2,0));
								ft->setRailsShape(rs);
							}

							ft->moveTo(pos);
							setKitOwnerWithWrite(ft);
							selectNode(ft);
							c1->getOwner()->connectTo(ft->getConnection(0));
							c2->getOwner()->connectTo(ft->getConnection(1));
							redrawAll();
						}
						else 
							c1->getOwner()->connectTo(c2->getOwner());
//						osg::Vec3d dir
					}

					return true;
				}
		break;
	}
	return false;
}

bool Editor::onDoubleClick(unsigned int button, unsigned int state)
{
	if (button==3)
	{
		osg::Vec3d hitPt;

//		hitPt.set(getWorldX(), getWorldY(),0);
//		hitPt.z()= srtmData.getHeight(hitPt.x(),hitPt.y());
		hitPt= getPointOnTerrain(getWorldX(),getWorldY(),true,true);
		(*perspectiveViews.begin())->setTarget(hitPt);
		(*perspectiveViews.begin())->exposed();
	}
	return false;
}

bool Editor::onDrag(unsigned int state)
{
	osg::Vec3d pos;
	if (mode==em_Move && state&GDK_BUTTON1_MASK && selectedNode.valid() && !selectedNode->locked())
	{
		if (state&(GDK_MOD1_MASK|GDK_MOD3_MASK|GDK_MOD4_MASK|GDK_MOD5_MASK))
		{
			pos= selectedNode->getPosition()+osg::Vec3d(0.0,0.0,getWorldDY());
			selectedNode->moveTo(pos);
		}
		else if (state&GDK_CONTROL_MASK)
			selectedNode->rotate(osg::Quat(DegToRad(getNormDX()*400),osg::Vec3d(0,0,1)));
		else 
		{
			pos= selectedNode->getPosition()+osg::Vec3d(getWorldDX(),getWorldDY(),0.0);
			if (!(state&GDK_SHIFT_MASK))
				pos= getPointOnTerrain(pos.x(),pos.y(),true,true);
//				pos.z()= srtmData.getHeight(pos.x(),pos.y());
			selectedNode->moveTo(pos);
		}
		updateNodeDesc();
		redrawAll();
		return onMove(state);
	}
	return onMove(state);
}
bool Editor::onMove(unsigned int state)
{
	osg::Vec3d hitPt;

	hitPt= getPointOnTerrain(getWorldX(),getWorldY(),true,false);
//	hitPt.set(getWorldX(), getWorldY(),0);
//	hitPt.z()= srtmData.getHeight(hitPt.x(),hitPt.y());

	static char buf[256];
	sprintf(buf,"X:%0.2f",hitPt.x());
//	sprintf(buf,"X:%0.2f %d",hitPt.x(),getCellX(hitPt.x()));
//	statusBar[0]->setText(buf);
	Publisher::set_status_msg(0,buf);
	sprintf(buf,"Y:%0.2f",hitPt.y());
//	sprintf(buf,"Y:%0.2f %d",hitPt.y(),getCellY(hitPt.y()));
//	statusBar[1]->setText(buf);
	Publisher::set_status_msg(1,buf);
	sprintf(buf,"Z:%0.2f",hitPt.z());
//	statusBar[2]->setText(buf);
	Publisher::set_status_msg(2,buf);

	if (selectedNode.valid())		
		sprintf(buf,"dist:%0.2f",(selectedNode->getPosition()-hitPt).length());
	else
		buf[0]= 0;
//	statusBar[3]->setText(buf);
	Publisher::set_status_msg(3,buf);
	redrawAll();

	return true;
}

bool Editor::freeSelectedNode()
{
	if (selectedNode.valid() && !selectedNode->locked())
	{
		edNode *node= selectedNode.get();
		deselectNode();
		node->free();
		return true;
	}
	return false;
}

/*
bool Editor::onKeyDown(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
	edTerrainNode *tn= NULL;
	switch (mode)
	{
		case em_Select:
		case em_Move:
		case em_Rotate:
			tn= dynamic_cast<edTerrainNode*>(selectedNode.get());
			if (tn)
			{
				switch (ea.getKey())
				{
					case '1' : tn->applyCulture("very_sparse_trees.clt"); return true;
//					case '2' : tn->applyCulture("dense_trees.clt"); return true;
//					case '3' : tn->applyCulture("sparse_trees.clt");  return true;
				}
			}
		break;
		case em_Fill:
			switch (ea.getKey())
			{
				case '0' : selectMaterial(0); return true;
				case '1' : selectMaterial(1); return true;
				case '2' : selectMaterial(2); return true;
				case '3' : selectMaterial(3); return true;
				case '4' : selectMaterial(4); return true;
				case '5' : selectMaterial(5); return true;
				case '6' : selectMaterial(6); return true;
				case '7' : selectMaterial(7); return true;
				case '8' : selectMaterial(8); return true;
				case '9' : selectMaterial(9); return true;
			}
		break;
		case em_CreatePointFeatures:
			switch (ea.getKey())
			{
				case '0' : selectModel(0); return true;
				case '1' : selectModel(1); return true;
				case '2' : selectModel(2); return true;
				case '3' : selectModel(3); return true;
				case '4' : selectModel(4); return true;
				case '5' : selectModel(5); return true;
				case '6' : selectModel(6); return true;
				case '7' : selectModel(7); return true;
				case '8' : selectModel(8); return true;
				case '9' : selectModel(9); return true;
			}
		break;
	}
	edRailLine *rl= NULL;
	edSplineFeature *sf= NULL;
	edLineFeature *lf= NULL;
	switch (ea.getKey())
	{
		case osgGA::GUIEventAdapter::KEY_Return: 
			lf= dynamic_cast<edLineFeature*>(selectedNode.get());
			if (lf)
			{
				deselectNode();
				rl= dynamic_cast<edRailLine*>(lf);
				if (rl)
				{
					while (!rl->isFirst())
					{
						rl->getFirst()->applyFeature();
					}
					rl->applyFeature();
				}
				else
					lf->applyFeature();
			}
		return true;
		case 'c' : case 'C' :
			sf= dynamic_cast<edSplineFeature*>(selectedNode.get());
			if (sf)
				sf->toggleCurve();
		return true;
		case 's' : case 'S' : changeMode(em_Select); return true;
		case 'm' : case 'M' : changeMode(em_Move); return true;
//		case 'r' : case 'R' : changeMode(em_Rotate); return true;
		case 'p' : case 'P' : changeMode(em_CreatePoints); return true;
		case 'n' : case 'N' : changeMode(em_CreatePointFeatures); return true;
		case 'l' : case 'L' : changeMode(em_CreateLines); return true;
//		case 'c' : case 'C' : changeMode(em_ConnectPoints); return true;
		case 'r' : case 'R' : changeMode(em_CreateRailLines); return true;
		case 'w' : case 'W' : changeMode(em_CreateFence); return true;
		case 'd' : case 'D' : changeMode(em_Divide); return true;
		case 'f' : case 'F' : changeMode(em_Fill); return false;
//		case 'h' : case 'H' : nodesRoot->setNodeMask(!nodesRoot->getNodeMask()); return true;
		case osgGA::GUIEventAdapter::KEY_F2 : mapsRoot->setValue(0,!mapsRoot->getValue(0)); return true;
		case osgGA::GUIEventAdapter::KEY_F3 : mapsRoot->setValue(1,!mapsRoot->getValue(1)); return true;
		case osgGA::GUIEventAdapter::KEY_F4 : nodesRoot->setNodeMask(~nodesRoot->getNodeMask()); return true;
				
//	CFileDialog dlg(true,"bscn","*.bscn",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY,"BSCN Files (*.bscn)|*.bscn|All Files (*.*)|*.*||",NULL);
			
//		case osgGA::GUIEventAdapter::KEY_F5 : return true;
		case osgGA::GUIEventAdapter::KEY_F6 : freeNodes(); loadFromFile("tmp.bscn"); return true;
		case osgGA::GUIEventAdapter::KEY_F7 : saveToFile("tmp.bscn"); return true;
												  //nodesRoot->setNodeMask(!nodesRoot->getNodeMask()); return true;
		case osgGA::GUIEventAdapter::KEY_Delete : 
			if (selectedNode.valid() && !selectedNode->locked())
			{
				edNode *node= selectedNode.get();
				deselectNode();
				node->free();
				return true;
			}
		break;
//		case osgGA::GUIEventAdapter::KEY_Up: previewPitch+= M_PI*0.05; break;
//		case osgGA::GUIEventAdapter::KEY_Down: previewPitch-= M_PI*0.05; break;
//		case osgGA::GUIEventAdapter::KEY_Left: previewYaw-= M_PI*0.05; break;
//		case osgGA::GUIEventAdapter::KEY_Right: previewYaw+= M_PI*0.05; break;
//		case osgGA::GUIEventAdapter::KEY_Page_Up: previewDist/= 1.25; break;
//		case osgGA::GUIEventAdapter::KEY_Page_Down: previewDist*= 1.25; break;

	}
	return false; 
}
*/

/*
struct ltpair
{
	bool operator()(const std::pair<int,int> p1, const std::pair<int,int> p2) const
	{
		if (p1.first < p2.first)
			return true;
		else
			if (p1.first == p2.first)
				return (p1.second < p2.second);
	return false;
	}
};*/
struct Tri
{
	int v1,v2,v3;
	inline osg::Vec3d getPt1(triangulateio &out)
	{ return osg::Vec3d(out.pointlist[v1*2+0],out.pointlist[v1*2+1],out.pointattributelist[v1]); };
	inline osg::Vec3d getPt2(triangulateio &out)
	{ return osg::Vec3d(out.pointlist[v2*2+0],out.pointlist[v2*2+1],out.pointattributelist[v2]); };
	inline osg::Vec3d getPt3(triangulateio &out)
	{ return osg::Vec3d(out.pointlist[v3*2+0],out.pointlist[v3*2+1],out.pointattributelist[v3]); };
	inline osg::Vec3d getNormal(triangulateio &out)
	{
		osg::Vec3d v= (getPt1(out)-getPt2(out))^(getPt1(out)-getPt3(out));
		v.normalize();
		return v;
	}
};

typedef std::vector<Tri> TriList;
typedef std::pair<int,int> CellCoords;
typedef std::map< CellCoords, TriList, ltpair> Terrain;

bool Editor::fill(osg::Vec3d pos)
{
//	double cellDim= 1000.0;
	osg::Matrix texMat;
	texMat.makeIdentity();

	osg::Vec3d pt;
	osg::ref_ptr<CollectNodesNearby> cnn= new CollectNodesNearby(nf_ExportToDelanuay,1000,osg::Vec2d(pos.x(),pos.y()));
	nodesRoot->accept(*cnn);
	printf("found %d points and %d lines\n",cnn->ptsList.size(),cnn->lnsList.size());
	if ( cnn->ptsList.size()<3 || cnn->lnsList.size()<3 )
		return false;
	triangulateio in,out;
	memset(&in,0,sizeof(in));
	memset(&out,0,sizeof(out));
	in.pointlist= (REAL*)malloc(sizeof(REAL)*cnn->ptsList.size()*2);
	in.pointattributelist= (REAL*)malloc(sizeof(REAL)*cnn->ptsList.size());
	in.pointmarkerlist= NULL;
	in.numberofpoints= cnn->ptsList.size();
	in.numberofpointattributes= 1;
	for (unsigned int i=0; i<cnn->ptsList.size(); i++)
	{
		pt= cnn->ptsList[i]->getMatrix().getTrans();
		in.pointlist[2*i]= pt.x();
		in.pointlist[2*i+1]= pt.y();
		in.pointattributelist[i]= pt.z();
	}
	in.segmentlist= (int*)malloc(sizeof(int)*cnn->lnsList.size()*2);
	in.segmentmarkerlist= NULL;
	in.numberofsegments= cnn->lnsList.size();
	for (unsigned int i=0; i<cnn->lnsList.size(); i++)
	{
		in.segmentlist[i*2]= cnn->lnsList[i]->getPt1()->getUID();
		in.segmentlist[i*2+1]= cnn->lnsList[i]->getPt2()->getUID();
	}

	in.regionlist= (REAL*)malloc(sizeof(REAL)*1*4);
	in.regionlist[0]= pos.x();
	in.regionlist[1]= pos.y();
	in.regionlist[2]= 1;
	in.regionlist[3]= 0;
	in.numberofregions= 1;

//	out.pointlist= in.pointlist;
//	out.pointattributelist= in.pointattributelist;

	triangulate("PpeAz",&in,&out,NULL);

//	Tri tri;
	osg::Vec3d center;
	CellCoords coords;
	coords.first= 3;
	coords.second= -2;
	Terrain terrain;
	unsigned int nt= 0;
	for (int i=0; i<out.numberoftriangles; i++)
		if (out.triangleattributelist[i]==1)
			nt++;

	edTerrainNode *tn= NULL;
	if (nt>0 && in.numberofpoints==out.numberofpoints && material!=NULL)
	{
		tn= new edTerrainNode();
		tn->setMaterial(material);

		tn->setNumTriangles(nt);

		int j= 0;
		for (int i=0; i<out.numberoftriangles; i++)
			if (out.triangleattributelist[i]==1)
			{
				tn->setTriangle(j,cnn->ptsList[out.trianglelist[i*3+0]],cnn->ptsList[out.trianglelist[i*3+1]],cnn->ptsList[out.trianglelist[i*3+2]]);
				j++;
			}
	}

	SafeFree(in.pointlist);
	SafeFree(in.pointattributelist);
	SafeFree(in.segmentlist);
	SafeFree(in.regionlist);
//	free(out.pointlist);
//	free(out.pointattributelist);
	SafeFree(out.pointmarkerlist);
	SafeFree(out.trianglelist);
	SafeFree(out.triangleattributelist);
	SafeFree(out.segmentlist);
	SafeFree(out.segmentmarkerlist);
	SafeFree(out.edgelist);
	SafeFree(out.edgemarkerlist);
	SafeFree(out.neighborlist);

	if (tn)
	{
		setKitOwnerWithWrite(tn);
		tn->updateVisual();
//		selectNode(tn);
	}

	return false;
}

osg::Object* Editor::getObjectFromCache(const std::string &name)
{
	ObjectsCache::iterator it;
	it=	objectsCache.find(name);
	return ( it!=objectsCache.end() ? it->second.get() : NULL ); 
}

void Editor::redrawAll()
{
	if (allowRedraws==0)
	{
		mainView->exposed();
		PerspectiveViewsList::iterator it;
		for (it=perspectiveViews.begin(); it!=perspectiveViews.end(); ++it)
			(*it)->exposed();
	}
}

bool Editor::addKit(unsigned int classID, PropertySet::Properties &props)
{

	std::pair<KitsMap::iterator,bool> ret= kits.insert(KitsMap::value_type(classID,NULL));
	if (!ret.second)
		return false;

	Kit *kit=ret.first->second= new Kit();


	PropertySet::Properties::iterator it;
	for (it= props.begin(); it!=props.end(); ++it)
	{
		switch (it->second.getType())
		{
			case Property::Type::pt_String : 
				if (it->second.dir.empty())
					it->second.item= kit->addTextEntry(it->first.c_str(),it->second.defaultStr.c_str());
				else
				{
					char buf[256];
					GetCurrentDirectory(256,buf);
					strcat(buf,it->second.dir.c_str());
					unsigned int t= GetTickCount();
					kit->addFileSel(it->first.c_str(),buf,it->second.pattern.c_str(),it->second.defaultStr.c_str());
					printf("filesel time: %d\n",GetTickCount()-t);
				}
			break;
			case Property::Type::pt_Int : 
				if (it->second.min==0 && it->second.max==0)
//					kit->addButton(it->first.c_str(),it->second.defaultVal==1);
					kit->addButton(it->first.c_str(),it->second.getm.asInt!=NULL,it->second.defaultVal==1);
				else
					kit->addIntEntry(it->first.c_str(),it->second.min,it->second.max,it->second.step,it->second.defaultVal);
			break;
		}
	}
	return true;
}

void Editor::toggleCurve()
{
	edFlexTrack *ft= dynamic_cast<edFlexTrack*>(selectedNode.get());
	if (ft)
		ft->split();

	redrawAll();
}

void Editor::applyFeatures()
{
			edLineFeature *lf= dynamic_cast<edLineFeature*>(selectedNode.get());
			if (lf)
			{
				deselectNode();
				edRailLine *rl= dynamic_cast<edRailLine*>(lf);
				if (rl)
				{
					rl= dynamic_cast<edRailLine*>(rl->getLast());
					while (!rl->isFirst())
					{
						rl->getFirst()->applyFeature();
					}
					rl->applyFeature();
				}
				else
					lf->applyFeature();
			}
			else
			{
				edTerrainNode *tn= dynamic_cast<edTerrainNode*>(selectedNode.get());
				if (tn)
					tn->applyCulture("very_sparse_trees.clt");
			}
			/*
	edRailLine *rl= dynamic_cast<edRailLine*>(selectedNode.get());
	if (rl)
		rl->applyFeature();
*/	
}
