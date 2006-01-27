#include "terrainNode.h"
#include "editor.h"
#include <osg/PolygonMode>
#include <osgUtil/IntersectVisitor>
#include <osgUtil/SmoothingVisitor>

const char* v5materials[] = { "blend", "grass.mat", "water.mat", "grassDead2.mat", "groundYellowBrown.mat", "farm0.mat", "farm1.mat", "farm2.mat", "farm3.mat", "farm4.mat", "grassGreen1.mat", "grassGreen2.mat", "water2.mat", "farm5.mat", "farm6.mat" };

TerrainMaterial *Triangle::getValidMaterial(int i)
{
	TerrainMaterial *ret= NULL;
	edPoint::OwnersList::iterator ow_it;
	edPoint::OwnersList &ol(v[i]->getTerrainOwnersList());
	printf("owners %d\n",ol.size());
	for (ow_it=ol.begin(); ow_it!=ol.end(); ++ow_it)
	{
		if ((*ow_it)->material.valid())
			ret= (*ow_it)->material.get();
	}
//			return (*ow_it)->material.get();
	return ret;
//	return NULL;
}

bool Triangle::hasMaterial(int i, TerrainMaterial *mat)
{
	edPoint::OwnersList::iterator ow_it;
	edPoint::OwnersList &ol(v[i]->getTerrainOwnersList());
	for (ow_it=ol.begin(); ow_it!=ol.end(); ++ow_it)
		if ((*ow_it)->material.get()==mat)
			return true;
	return false;
}

osg::Group* edTerrainNode::terrainRoot= NULL;
/*
void edTerrainNode::free()
{
	osg::ref_ptr<edTerrainNode> cont= this;
	for (unsigned int i=0; i<visuals.size(); i++)
	{
		if (visuals[i]->getNumParents()>0)
		{
			osg::Group *gr= visuals[i]->getParent(0);
			gr->removeChild(visuals[i].get());
		}	
		visuals[i]= NULL;
	}
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
	{
		if (selectedVisuals[i]->getNumParents()>0)
		{
			osg::Group *gr= selectedVisuals[i]->getParent(0);
			gr->removeChild(selectedVisuals[i].get());
		}
		selectedVisuals[i]= NULL;
	}
	edNode::free();
//	printf("TerrainNode num_refs: %d  v: %d  sv: %d\n",referenceCount(),visuals.size(),selectedVisuals.size());
	cont= NULL;
}
*/

std::string edTerrainNode::cultureFile= "very_sparse_trees.clt";


void edTerrainNode::free()
{
	for (unsigned int i=0; i<triangles.size(); i++)
		for (unsigned int t=0; t<3; t++)
			if (triangles[i].v[t]) triangles[i].v[t]->remTerrainOwner(this);
	edGeomNode::free();
}

void edTerrainNode::load(std::istream &stream, int version, CollectNodes *cn)
{
	edPoint *p1;
	edPoint *p2;
	edPoint *p3;
	edNode::load(stream, version, cn);
	unsigned int n= 0;
	stream.read((char*)&n,sizeof(n));
	unsigned int ptUID, UID;
	triangles.resize(n);
	for (unsigned int i=0; i<n; i++)
	{
		stream.read((char*)&ptUID,sizeof(ptUID));
		p1= cn->ptsList[ptUID];
		stream.read((char*)&ptUID,sizeof(ptUID));
		p2= cn->ptsList[ptUID];
		stream.read((char*)&ptUID,sizeof(ptUID));
		p3= cn->ptsList[ptUID];
		triangles[i].set(p1,p2,p3);
		p1->addTerrainOwner(this);
		p2->addTerrainOwner(this);
		p3->addTerrainOwner(this);
	}

	if(version<6)
	{

		stream.read((char*)&UID,sizeof(UID));
		_materialName = v5materials[UID];

	} else {

		_materialName = readS(stream);

	};

	material = Editor::instance()->getOrCreateMaterial(_materialName);

	if (version<2)
	{
		osg::Matrixd texMat;
		stream.read((char*)texMat.ptr(),sizeof(osg::Matrixd::value_type)*16);
	}
}

void edTerrainNode::save(std::ostream &stream)
{
	edNode::save(stream);
	unsigned int n= triangles.size();
	stream.write((char*)&n,sizeof(n));
	unsigned int ptUID, UID;
	for (unsigned int i=0; i<n; i++)
	{
		ptUID= triangles[i].v[0]->getUID();
		stream.write((char*)&ptUID,sizeof(ptUID));
		ptUID= triangles[i].v[1]->getUID();
		stream.write((char*)&ptUID,sizeof(ptUID));
		ptUID= triangles[i].v[2]->getUID();
		stream.write((char*)&ptUID,sizeof(ptUID));
	}

	write(stream, _materialName);

//	stream.write((char*)texMat.ptr(),sizeof(osg::Matrixd::value_type)*16);
		
//	Editor::lastInstance()->
}

void edTerrainNode::updateVisual()
{
	osg::ref_ptr<osgUtil::SmoothingVisitor> sv= new osgUtil::SmoothingVisitor();
//	double cellDim= 1000.0;
	CellCoords coords;
	osg::Vec3d center;
	Terrain terrain;
	if (selectedVisuals.empty())
	{
		double maxX= -1000000.0;
		double minX=  1000000.0;
		double maxY= -1000000.0;
		double minY=  1000000.0;
		for (unsigned int i=0; i<triangles.size(); i++)
		{
			center= triangles[i].getCenter();
			if (center.x()<minX) minX= center.x();
			if (center.x()>maxX) maxX= center.x();
			if (center.y()<minY) minY= center.y();
			if (center.y()>maxY) maxY= center.y();
			coords.first=  Editor::lastInstance()->getCellX(center.x());//Editor::cellDim;
			coords.second=  Editor::lastInstance()->getCellY(center.y());//Editor::cellDim;
//			coords.second= center.y()/Editor::cellDim;
			terrain[coords].push_back(&triangles[i]);

		}
		pos.set(minX*0.5+maxX*0.5,minY*0.5+maxY*0.5,0);
		osg::Geometry *geom;
		osg::Texture2D *tex= NULL;
		osg::Material *mater= NULL;
		osg::StateSet *dstate= NULL;
		osg::Geode *geode= NULL;
		osg::TexEnvCombine *envComb= NULL;
		osg::Vec3Array *v;
		osg::Vec3Array *norm;
		osg::Vec2Array *t;
		osg::Vec2Array *t1;
		osg::Vec2Array *t2;
		osg::Vec4Array *col;
		osg::Matrixd mat;
		osg::Vec3d origin,vert,texc;
		TriList::iterator tri_iter;
		Terrain::iterator it;
//		char buf[256];
		setNumVisuals(terrain.size());
		for (it=terrain.begin(); it!=terrain.end(); ++it)
		{
			osg::MatrixTransform *trans= Editor::lastInstance()->getOrCreateCell(it->first.first,it->first.second);
//			trans->setNodeMask(Editor::nm_SolidTerrain);
			origin= trans->getMatrix().getTrans();
//			origin.set(it->first.first*Editor::cellDim,it->first.second*Editor::cellDim,0);
//			nameFromCoords5(buf,it->first.first,it->first.second);
//			for (unsigned int i=0; i<terrainRoot->getNumChildren(); i++)
//				if (terrainRoot->getChild(i)->getName().compare(buf)==0)
//				{
//					trans= dynamic_cast<osg::MatrixTransform*>(terrainRoot->getChild(i));
//					break;
//				}
//			if (trans==NULL)
//			{
//				mat.makeTranslate(origin);
//				trans= new osg::MatrixTransform(mat);
//				trans->setName(buf);
//				terrainRoot->addChild(trans);	
//			}
			unsigned int n= it->second.size();
			if (n>0)
			{
				if (!material.valid())
				{
//					DoubleMaterialTris singleMatTris;
					DoubleMaterialTris dblMatTris;
					TrippleMaterialTris trplMatTris;
					TerrainMaterial* mats[]= { NULL,NULL,NULL };
					MaterialPair matPair;
					for (tri_iter= it->second.begin(); tri_iter!=it->second.end(); ++tri_iter)
					{
						for (unsigned int i=0; i<3; i++)
						{
//							if ((*tri_iter)->v[i]->getTerrainOwnersList().size()>0)
								mats[i]= (*tri_iter)->getValidMaterial(i);//(*tri_iter)->v[i]->getTerrainOwnersList().front()->material.get();
//							else
//								mats[i]= NULL;

						}
						if (mats[0]==NULL || mats[1]==NULL || mats[2]==NULL)
							continue;
						if (mats[0]!=mats[1] && mats[0]!=mats[2] && mats[1]!=mats[2])
						{
							trplMatTris.push_back(*tri_iter);
							continue;
						}
						matPair.first= mats[0];
						if (mats[1]!=matPair.first)
						{
							matPair.second= mats[1];
						}
						else
							matPair.second= mats[2];
						if (matPair.first>matPair.second)
							std::swap(matPair.second,matPair.first);
						
//						if (matPair.first!=matPair.second)
							dblMatTris[matPair].push_back(*tri_iter);

					}
					DoubleMaterialTris::iterator dmt_iter;
					for (dmt_iter=dblMatTris.begin() ;dmt_iter!=dblMatTris.end(); ++dmt_iter)
					{
						unsigned int n= dmt_iter->second.size();
						if (n>0)
						{
							v	 = new osg::Vec3Array;
							norm = new osg::Vec3Array;
							t1	 = new osg::Vec2Array;
							t2	 = new osg::Vec2Array;
							col  = new osg::Vec4Array;
							v->reserve(n*3);
							norm->reserve(n*3);
							t1->reserve(n*3);
							t2->reserve(n*3);
							col->reserve(n*3);
							for (tri_iter= dmt_iter->second.begin(); tri_iter!=dmt_iter->second.end(); ++tri_iter)
							{
								for (unsigned int i=0; i<3; i++)
								{
									vert= (*tri_iter)->v[i]->getPosition()-origin;
									v->push_back(vert);
									norm->push_back((*tri_iter)->getNormal());
									t1->push_back(dmt_iter->first.first->getTexCoord(vert));
									t2->push_back(dmt_iter->first.second->getTexCoord(vert));
									if ((*tri_iter)->hasMaterial(i,dmt_iter->first.first))
										col->push_back(osg::Vec4(1,1,1,0));
									else
										col->push_back(osg::Vec4(1,1,1,1));

									/*
									edPoint::OwnersList::iterator ow_it;
									edPoint::OwnersList &ol((*tri_iter)->v[i]->getTerrainOwnersList());
									for (ow_it=ol.begin(); ow_it!=ol.end(); ++ow_it)
										if ((*ow_it)->material.get()==dmt_iter->first.first)
										{
											col->push_back(osg::Vec4(1,1,1,0));
											break;
										}
									if (ow_it==ol.end())
										col->push_back(osg::Vec4(1,1,1,1));*/
								}
							}
							geom = new osg::Geometry;
						
							geom->setVertexArray( v );
							geom->setNormalArray( norm );
							geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
							fixupTexCoords(*t1);
							geom->setTexCoordArray( 0, t1 );
							fixupTexCoords(*t2);
							geom->setTexCoordArray( 1, t2 );
							geom->setColorArray( col );
							geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
							geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,n*3));
		//					geom->setStateSet( material->dstate.get() );
							geode = new osg::Geode;
							geode->addDrawable( geom );
							geode->setUserData(this);
							geode->setNodeMask(getNodeMask());
//							geode->setStateSet( material->dstate.get() );

								dstate= new osg::StateSet;
								tex = new osg::Texture2D;
								tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
								tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
								
								tex->setImage(osgDB::readImageFile(dmt_iter->first.first->tex));
								
								dstate->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON );
								
								tex = new osg::Texture2D;
								tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
								tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
								
								printf("tex0: %s ; tex1: %s\n",dmt_iter->first.first->tex.c_str(),dmt_iter->first.second->tex.c_str());
								tex->setImage(osgDB::readImageFile(dmt_iter->first.second->tex));
								
								dstate->setTextureAttributeAndModes(1, tex, osg::StateAttribute::ON );
								
								envComb= new osg::TexEnvCombine;
								envComb->setSource0_RGB(osg::TexEnvCombine::TEXTURE1);
								envComb->setSource1_RGB(osg::TexEnvCombine::TEXTURE0);
								envComb->setSource2_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
								envComb->setCombine_RGB(osg::TexEnvCombine::INTERPOLATE); 
								dstate->setTextureAttribute(0, envComb);

								envComb= new osg::TexEnvCombine;
								envComb->setSource0_RGB(osg::TexEnvCombine::PREVIOUS);
								envComb->setSource1_RGB(osg::TexEnvCombine::PRIMARY_COLOR);
								envComb->setCombine_RGB(osg::TexEnvCombine::MODULATE); 
								dstate->setTextureAttribute(1, envComb);
		
								dstate->setMode( GL_ALPHA_TEST, osg::StateAttribute::OFF );        

								geode->setStateSet( dstate );

							sv->reset();
							geode->accept(*sv.get());

//							addVisual(geode);
							trans->addChild(geode);
							addVisual(geode);
						//	printf("geode->referenceCount(): %d\n",geode->referenceCount());
							geode = new osg::Geode;
							geode->addDrawable( geom );
							geode->setUserData(this);
							geode->setNodeMask(Editor::nm_SelectedGeometry);
//							dstate= new osg::StateSet();
//							dstate->setRenderBinDetails(10,"RenderBin");
//							dstate->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS));
//							osg::PolygonMode *pm= new osg::PolygonMode();
//							pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
//							dstate->setAttributeAndModes(pm);
//							dstate->setAttributeAndModes(new osg::LineWidth(3));
  //  dstate->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    //dstate->setRenderBinDetails(20,"RenderBin");
	//						mater= new osg::Material();
	//						mater->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
	//						mater->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
		//					mater->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,1.0,1.0));
	//						dstate->setAttributeAndModes(mater);
	//						geode->setStateSet( dstate );
							geode->setStateSet( Editor::instance()->selectedNodesState.get() );
							trans->addChild(geode);
							addSelectedVisual(geode);						
						}
					}
					if (dblMatTris.empty())
						material= Editor::instance()->getDefaultMaterial();
//					getTerrainOwnersList()
				}
				if (material.valid())
				{
					v	 = new osg::Vec3Array;
					norm = new osg::Vec3Array;
					t	 = new osg::Vec2Array;
					col  = new osg::Vec4Array(1);
					v->reserve(n*3);
					norm->reserve(n*3);
					t->reserve(n*3);
					(*col)[0].set(1,1,1,1);
					for (tri_iter= it->second.begin(); tri_iter!=it->second.end(); ++tri_iter)
					{
						for (unsigned int i=0; i<3; i++)
						{
							vert= (*tri_iter)->v[i]->getPosition()-origin;
							v->push_back(vert);
							norm->push_back(( material->type==1 || !material->reflectionTex.empty() ? osg::Vec3f(0,0,1) : (*tri_iter)->getNormal() ));
//							texc= texMat*vert;
//							t->push_back(osg::Vec2(texc.x()/material->texDim.x(),texc.y()/material->texDim.y()));
							t->push_back(material->getTexCoord(vert));
						}
/*
						vert= tri_iter->getPt2(out)-origin;
						v->push_back(vert);
						norm->push_back(tri_iter->getNormal(out));
						texc= texMat*vert;
						t->push_back(osg::Vec2(texc.x()/material->texDim.x(),texc.y()/material->texDim.x()));

						vert= tri_iter->getPt3(out)-origin;
						v->push_back(vert);
						norm->push_back(tri_iter->getNormal(out));
						texc= texMat*vert;
						t->push_back(osg::Vec2(texc.x()/material->texDim.x(),texc.y()/material->texDim.x()));*/
					}
					geom = new osg::Geometry;
					if (material->drawCallback.valid())
					{
						geom->setDrawCallback(material->drawCallback.get());
						geom->setUseDisplayList(false);
					}
				
					geom->setVertexArray( v );
					geom->setNormalArray( norm );
					geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
					fixupTexCoords(*t);
					geom->setTexCoordArray( 0, t );
					geom->setColorArray( col );
					geom->setColorBinding( osg::Geometry::BIND_OVERALL );
					geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,n*3));
//					geom->setStateSet( material->dstate.get() );
					geode = new osg::Geode;
					geode->addDrawable( geom );
					geode->setUserData(this);
					geode->setNodeMask(getNodeMask());
					geode->setStateSet( material->dstate.get() );
//					addVisual(geode);
					sv->reset();
					geode->accept(*sv.get());
					trans->addChild(geode);
					addVisual(geode);
					printf("geode->referenceCount(): %d\n",geode->referenceCount());
					geode = new osg::Geode;
					geode->addDrawable( geom );
					geode->setUserData(this);
					geode->setNodeMask(Editor::nm_SelectedGeometry);
					/*
					dstate= new osg::StateSet();

							osg::PolygonMode *pm= new osg::PolygonMode();
							pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
							dstate->setAttributeAndModes(pm);
							dstate->setAttributeAndModes(new osg::LineWidth(1));
    dstate->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    dstate->setRenderBinDetails(20,"RenderBin");

					mater= new osg::Material();
					mater->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
					mater->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
//					mater->setEmission(osg::Material::Face::FRONT_AND_BACK, osg::Vec4(1.0,0.0,1.0,1.0));
					dstate->setAttributeAndModes(mater);
					geode->setStateSet( dstate );*/
					geode->setStateSet( Editor::instance()->selectedNodesState.get() );
					trans->addChild(geode);
					addSelectedVisual(geode);
				}
			}	
		}
	}
}
/*
void edTerrainNode::onSelect(osg::Geode *geom)
{
	for (unsigned int i=0; i<visuals.size(); i++)
		if (visuals[i]->getNumParents()>0)
		{
			osg::Group *gr= visuals[i]->getParent(0);
			gr->removeChild(visuals[i].get());
			gr->addChild(selectedVisuals[i].get());
		}
}

void edTerrainNode::onDeselect()
{
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
		if (selectedVisuals[i]->getNumParents()>0)
		{
			osg::Group *gr= selectedVisuals[i]->getParent(0);
			gr->removeChild(selectedVisuals[i].get());
			gr->addChild(visuals[i].get());
		}
}
*/
void edTerrainNode::applyCulture(const char *cultureFile)
{
	Editor::instance()->disableRedraws();
	cultures.push_back(new Culture());
	cultures.back()->load(cultureFile);
	for (unsigned int i=0; i<selectedVisuals.size(); i++)
	{
		cultures.back()->apply(selectedVisuals[i]->getParent(0)->asTransform()->asMatrixTransform(),dynamic_cast<osg::Geode*>(selectedVisuals[i].get()));
	}
	Editor::instance()->enableRedraws();
	redrawAll();
}

/*void edTerrainNode::setMaterialI(int val)
{
//	Editor::instance()->selectMaterial(val);
	setMaterial(Editor::instance()->getOrCreateMaterial(val)); 
	redrawAll(); 
}

int edTerrainNode::getMaterialI()
{
	return (material.valid() ? material->UID : 0);
}
*/

void edTerrainNode::setMaterialName(const char* name)
{

	printf("setMaterialName %s\n", name);
	_materialName = name;
//	TerrainMaterial* mat = Editor::instance()->getOrCreateMaterial(name);

	material = Editor::instance()->getOrCreateMaterial(name);
	Editor::instance()->selectMaterial(material.get());

	redrawAll();

}

const char* edTerrainNode::getMaterialName()
{

//	printf("atrapa");
	return _materialName.c_str();

}

unsigned int edTerrainNode::getNodeMask() 
{ 
	if ( material.valid() && material->type==2 )
		return Editor::nm_DynamicFlatReflection;
	else 
		return Editor::nm_SolidTerrain;
//	return ( material.valid() && material->type==3 ? Editor::nm_DynamicFlatReflection : Editor::nm_SolidTerrain ) ;
}

void edTerrainNode::fixupTexCoords(osg::Vec2Array &coords)
{
	if (coords.size()<1)
		return;

	osg::Vec2 min(coords[0]);
	osg::Vec2 max(coords[0]);

	for (unsigned int i=1; i<coords.size(); i++)
	{
		if (coords[i].x()>max.x())
			max.x()= coords[i].x();
		if (coords[i].x()<min.x())
			min.x()= coords[i].x();
		if (coords[i].y()>max.y())
			max.y()= coords[i].y();
		if (coords[i].y()<min.y())
			min.y()= coords[i].y();
	}

	osg::Vec2 delta( floor((min.x()+max.x())*0.5+0.5),floor((min.y()+max.y())*0.5+0.5) ) ;
	
	for (unsigned int i=0; i<coords.size(); i++)
		coords[i]-= delta;
}