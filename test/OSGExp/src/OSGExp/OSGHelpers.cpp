/**********************************************************************
 *
 *	FILE:			OSGHelpers.cpp
 *
 *	DESCRIPTION:	Methods to create and apply user defined stuff
 *					from helper objects in MAX.
 *
 *	CREATED BY:		Rune Schmidt Jensen, rune@schmidt-jensen.com
 *
 *	HISTORY:		Created 29.01.2003
 *
 *	Copyright 2003 VR-C
 **********************************************************************/
 
#include "OSGExp.h"
#include <osg/OccluderNode>
#include <osg/PagedLOD>

/**
 * Method to handle nodes referenced by an OSG Helper NodeMask Object.
 * The method will apply a user defined nodemask value to an OSG::Node.
 */
void OSGExp::applyNodeMaskValue(INode* node, TimeValue t, osg::Node* osgNode){
	IParamBlock2* pblock2;
	RefList refList;

	// Assure the node is referenced by an OSG Helper NodeMask object.
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
					if(id == NODEMASK_CLASS_ID){
						// Get properties from OSG helper NodeMask.
						Interval iv;
						int mode;
						unsigned int value;
						TCHAR* pValue;
						pblock2->GetValue(nodemask_mode, t, mode, iv);
						// Swith the nodemask modes
						switch(mode){
							case 0:		// Terrain
								value = 0xFF000000;
								break;
							case 1:		// Collision
								value = 0x00FF00000;
								break;
							case 2:		// No Collision
								value = 0x0000FFFF;
								break;
							case 3:		// Custom; get entered value
								pblock2->GetValue(nodemask_value, t, pValue, iv);
								value = Util::getUInt(pValue);
								break;
						}
						// Set NodeMask value.
						osgNode->setNodeMask(value);
					}
				}
			}
		}
		ptr = ptr->next;
	}
}

/**
 * Method to handle nodes referenced by an OSG Helper StateSet Object.
 * The method will apply user defined properties to an OSG::StateSet.
 */
void OSGExp::applyStateSetProperties(INode* node, TimeValue t, osg::StateSet* stateset){
	if(!stateset)
		return;

	IParamBlock2* pblock2;
	RefList refList;

	// Assure the node is referenced by an OSG helper StateSet object.
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
					if(id == STATESET_CLASS_ID){
						// Get properties from OSG helper StateSet.
						Interval iv;
						int renderBinNum;
						TCHAR* renderBinName;
						int renderBinMode;
						BOOL cullface;
						BOOL normalize;
						pblock2->GetValue(render_bin_num, t, renderBinNum, iv);
						pblock2->GetValue(render_bin_name, t, renderBinName, iv);
						pblock2->GetValue(render_bin_mode, t, renderBinMode, iv);
						pblock2->GetValue(glmode_cullface, t, cullface, iv);
						pblock2->GetValue(glmode_normalize, t, normalize, iv);

						// Set properties on the given osg::StateSet.
						switch(renderBinMode){
							case 0:
								stateset->setRenderBinDetails(renderBinNum, std::string(renderBinName), osg::StateSet::INHERIT_RENDERBIN_DETAILS);
								break;
							case 1:
								stateset->setRenderBinDetails(renderBinNum, std::string(renderBinName), osg::StateSet::USE_RENDERBIN_DETAILS);
								break;
							case 2:
								stateset->setRenderBinDetails(renderBinNum, std::string(renderBinName), osg::StateSet::OVERRIDE_RENDERBIN_DETAILS);
								break;
							case 3:
								stateset->setRenderBinDetails(renderBinNum, std::string(renderBinName), osg::StateSet::ENCLOSE_RENDERBIN_DETAILS);
								break;
						}
						
						if(cullface)
							stateset->setMode(GL_CULL_FACE,osg::StateAttribute::ON);
						else
							stateset->setMode(GL_CULL_FACE,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);

						if(normalize)
							stateset->setMode(GL_NORMALIZE,osg::StateAttribute::ON);
						else
							stateset->setMode(GL_NORMALIZE,osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);

						break;
					}
				}
			}
		}
		ptr = ptr->next;
	}
}

/**
 * This method will create an OSG billboard defined by an OSG
 * billboard helper object. 
 */
osg::ref_ptr<osg::Billboard> OSGExp::createBillboardFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){
	// Make the billboard from properties in helper object.
	osg::ref_ptr<osg::Billboard> bilbo = new osg::Billboard();
	bilbo->setName(node->GetName());
	// Set static datavariance for better performance
	bilbo->setDataVariance(osg::Object::STATIC);
	// Use default node mask
	if(_options->getUseDefaultNodeMaskValue())
		bilbo->setNodeMask(_options->getDefaultNodeMaskValue());

	// The billboard properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	int mode;
	osg::Vec3 axis;
	osg::Vec3 normal;

	if(pblock2){
		pblock2->GetValue(bilbo_mode, t, mode, iv);
		pblock2->GetValue(bilbo_axis_x, t, axis[0], iv);
		pblock2->GetValue(bilbo_axis_y, t, axis[1], iv);
		pblock2->GetValue(bilbo_axis_z, t, axis[2], iv);
		pblock2->GetValue(bilbo_normal_x, t, normal[0], iv);
		pblock2->GetValue(bilbo_normal_y, t, normal[1], iv);
		pblock2->GetValue(bilbo_normal_z, t, normal[2], iv);
	}
	// Set properties on OSG billboard.
	switch(mode){
	case 0:	
		bilbo->setMode(osg::Billboard::AXIAL_ROT);
		break;
	case 1:
		bilbo->setMode(osg::Billboard::POINT_ROT_WORLD);
		break;
	case 2:
		bilbo->setMode(osg::Billboard::POINT_ROT_EYE);
		break;
	}
	bilbo->setAxis(axis);
	bilbo->setNormal(normal);


	// Add geometry to OSG billboard.
	if(pblock2){
		// The j'th OSG geometry node added
		int j=0;
		for(int n=0; n<pblock2->Count(bilbo_nodes); n++){
			INode* bilboNode = pblock2->GetINode(bilbo_nodes, t, n);
			if(bilboNode){
				// Assert this is a geometry node
				ObjectState os = bilboNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){

					// Get the absolute position of geometry.
					osg::Matrix mat = convertMat(bilboNode->GetObjTMBeforeWSM(t));
	
					// Create the node's geometry; position of this is relative!
					osg::ref_ptr<osg::Geode> geode = createMultiMeshGeometry(rootTransform, bilboNode, os.obj, t);
					
					// MAX Geometry are exported as OSG geodes.
					if(geode.valid()){
						// Get drawables from geode. We know this is always
						// a _single_geometry_ node, because it was create in the
						// createMultiMesh method. 
						osg::Geometry* geometry = (osg::Geometry*)geode->getDrawable(0);
						// Rotate, scale the geometry and get its position.
						osg::Vec3 pos = transformBillboardGeometry(geometry, mat);
						// Add it to the billboard,
						bilbo->addDrawable(geometry);
						// Set the position.
						bilbo->setPosition(j++,pos);
					}
				} // End geometry node.
			}
		}// End for all nodes in pblock2
	}

	// Return OSG billboard.
	return bilbo;
}

/** 
 * This method will transform the given geometry's vertices
 * and normals by the given matrix.
 */
osg::Vec3 OSGExp::transformBillboardGeometry(osg::Geometry* geometry, osg::Matrix mat){

	// Rotate and scale every vertex to absolute coordinates.
	// The geometry will still be positioned in its local coordinate system.
	osg::Matrix cMat(mat);
	cMat.setTrans(0,0,0);
	osg::Vec3Array* coords = static_cast<osg::Vec3Array*>(geometry->getVertexArray());
	if(coords){
		int numCoords = coords->getNumElements();
		for(int i=0;i<numCoords;i++){
			osg::Vec3 coord = (*coords)[i];
			coord = cMat.preMult(coord);
			(*coords)[i].set(coord.x(), coord.y(), coord.z());
		
		}
	}
	
	// Transform every normal to absolute coordinates.
	// Use the inverse matrix when translating normals.
	osg::Matrix nMat;
	nMat.invert(mat);
	osg::Vec3Array* normals = dynamic_cast<osg::Vec3Array*>(geometry->getNormalArray());
	if(normals){
		int numNormals = normals->getNumElements();
		for(int i=0;i<numNormals;i++){
			osg::Vec3 normal = (*normals)[i];
			normal = nMat.preMult(normal);
			normal.normalize();
			(*normals)[i].set(normal.x(), normal.y(), normal.z());
		}
	}
	return mat.getTrans();
}

/**
 * This method will create an osg::(Paged)LOD defined by an OSG
 * LOD helper object. 
 */
osg::ref_ptr<osg::LOD> OSGExp::createLODFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){

	// The lod properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	osg::Vec3 center;
	BOOL usePagedLOD;
	TCHAR* templateFilename;

	if(pblock2){
		pblock2->GetValue(lod_center_x, t, center[0], iv);
		pblock2->GetValue(lod_center_y, t, center[1], iv);
		pblock2->GetValue(lod_center_z, t, center[2], iv);
		pblock2->GetValue(lod_usepaged, t, usePagedLOD, iv);
		pblock2->GetValue(lod_paged_filename, t, templateFilename, iv);
	}

	// Make the LOD from properties in helper object.
	osg::ref_ptr<osg::LOD> lod;
	if(usePagedLOD)
		lod = new osg::PagedLOD();
	else
		lod = new osg::LOD();
	lod->setName(node->GetName());
	// Set static datavariance for better performance
	lod->setDataVariance(osg::Object::STATIC);
	// Use default node mask
	if(_options->getUseDefaultNodeMaskValue())
		lod->setNodeMask(_options->getDefaultNodeMaskValue());

	// Set properties on OSG lod.
	lod->setCenter(center);

	// Add geometry to lod.
	if(pblock2){
		for(int n=0; n<NUM_LOD_OBJECTS; n++){
			INode* lodGeomNode = pblock2->GetINode(lod_node+n, t);
			if(lodGeomNode){
				// Assert this is a geometry node
				ObjectState os = lodGeomNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){

					// Create lod geometry; this geometry is placed relative!
					osg::ref_ptr<osg::MatrixTransform> osgGeomNode =  createGeomObject(rootTransform, lodGeomNode, os.obj, t);

					// Get min and max values from helper object.
					float min;
					float max;
					pblock2->GetValue(lod_min, t, min, iv, n);
					pblock2->GetValue(lod_max, t, max, iv, n);

					// If the lodGeomNode has a parentnode different from
					// the MAX rootNode, we need to create an OSG MatrixTransform
					// to put the lod geometry in before adding it to the OSG LOD.
					if(!lodGeomNode->GetParentNode()->IsRootNode()){
						// Get absolute position of parent node.
						osg::Matrix parentMat = convertMat(lodGeomNode->GetParentNode()->GetObjTMBeforeWSM(t));
						osg::ref_ptr<osg::MatrixTransform> parentTransform = new osg::MatrixTransform();
						// Set static datavariance for better performance
						parentTransform->setDataVariance(osg::Object::STATIC);
						// Use default node mask
						if(_options->getUseDefaultNodeMaskValue())
							parentTransform->setNodeMask(_options->getDefaultNodeMaskValue());

						parentTransform->setMatrix(parentMat);
						parentTransform->addChild(osgGeomNode.get());
						osgGeomNode = parentTransform;
					}

					// Use paged LOD
					if(usePagedLOD && dynamic_cast<const osg::PagedLOD*>(lod.get())){
						char filename[500];
						
						sprintf(filename,"%s%s", node->GetName(),													
													_options->getExportExtension().c_str());
//						sprintf(filename,"%s_%d%s", templateFilename,
//													n,
//													_options->getExportExtension().c_str());
						osg::PagedLOD* pagedLOD = (osg::PagedLOD*)(lod.get());
						pagedLOD->addChild(osgGeomNode.get(), min, max);
						pagedLOD->setFileName(n,filename);

						std::string absoluteFilename = std::string(_options->getExportPath());
						absoluteFilename.append(filename);
						osgDB::writeNodeFile(*osgGeomNode, absoluteFilename);
					}
					// or use standard LODs
					else{
						lod->addChild(osgGeomNode.get(), min, max);
					}

				} // End geometry node.
			}
		}// End for number of LOD objects in pblock2
	}

	// Return OSG LOD.
	return lod;
}

/**
 * This method will create an osg::Sequence object from the 
 * properties defined in an OSG Sequence helper object. 
 */
osg::ref_ptr<osg::Sequence> OSGExp::createSequenceFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){
	// Make the LOD from properties in helper object.
	osg::ref_ptr<osg::Sequence> seq = new osg::Sequence();
	seq->setName(node->GetName());
	// Set static datavariance for better performance
	seq->setDataVariance(osg::Object::STATIC);
	// Use default node mask
	if(_options->getUseDefaultNodeMaskValue())
		seq->setNodeMask(_options->getDefaultNodeMaskValue());


	// The sequence properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	float begin;
	float end;
	int loop;
	float speed;
	int reps;
	int mode;
	int approach;
	if(pblock2){
		pblock2->GetValue(sequence_begin, t, begin, iv);
		pblock2->GetValue(sequence_end, t, end, iv);
		pblock2->GetValue(sequence_loop, t, loop, iv);
		pblock2->GetValue(sequence_speed, t, speed, iv);	
		pblock2->GetValue(sequence_reps, t, reps, iv);	
		pblock2->GetValue(sequence_mode, t, mode, iv);
		pblock2->GetValue(sequence_approach, t, approach, iv);
	}
	// Set properties on OSG sequence.
	switch(loop){
		case 0:
			seq->setInterval(osg::Sequence::LOOP, begin, end);			
			break;
		case 1:
			seq->setInterval(osg::Sequence::SWING, begin, end);			
			break;
	}
	seq->setDuration(speed, reps);
	switch(mode){
		case 0:
			seq->setMode(osg::Sequence::START);
			break;
		case 1:
			seq->setMode(osg::Sequence::STOP);
			break;
		case 2:
			seq->setMode(osg::Sequence::PAUSE);
			break;
		case 3:
			seq->setMode(osg::Sequence::RESUME);
			break;
	}
	// Add geometry to sequence.
	if(pblock2){
		// Sample sequence objects.
		if(approach==0){
			INode* seqGeomNode = pblock2->GetINode(sequence_sample_node, t);
			if(seqGeomNode){
				// Assert this is a geometry node
				ObjectState os = seqGeomNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){

					// Frames per sample
					int fps;
					pblock2->GetValue(sequence_frame_per_sample, t, fps, iv);

					// The start and end of animation.
					TimeValue animStart = _ip->GetAnimRange().Start();
					TimeValue animEnd = _ip->GetAnimRange().End();
					TimeValue step;
					int n=0;
					int delta = GetTicksPerFrame()*fps;

					// If the interval given in pblock2 is different from -1
					// we should use this interval instead. This is given in sec.
					if(end > begin){
						animStart = begin*GetFrameRate()*GetTicksPerFrame();
						animEnd = end*GetFrameRate()*GetTicksPerFrame();
					}

					// Sample the MAX geometry node.
					//  frameTime  frameTime  frameTime  frameTime 
					//      A          B           C         D
					// |----------|----------|----------|----------|    time
					// A          B			 C          D	        	sampled geometry 
					osg::Geode* prevGeode = NULL;
					float frameTime = 0.0f;
					for (step=animStart; step<=animEnd-delta; step+=delta) {
						os = seqGeomNode->EvalWorldState(step); 
						// Create sequence geometry; this geometry is placed relative!
						osg::ref_ptr<osg::MatrixTransform> transform =  createGeomObject(rootTransform, seqGeomNode, os.obj,  step);
						osg::Geode* geode = Util::getGeode(transform.get());

						// Only add geode to OSG sequence if it is different
						// from the previous sampled geometry.
						if(!Util::isGeodeEqual(prevGeode, geode)){
							// Save geode so we can compared it to the next sampled geometry.
							prevGeode = geode;

							// Only set the frame time when we know for how long time we are going
							// to see geometry, this is first possible, when we know when we should display
							// the next geometry. Hence do not set it the first time.
							if(step!=animStart){
								seq->setTime(n++, frameTime);
								frameTime = 0.0f;
							}
						
							// If the seqGeomNode has a parentnode different from
							// the MAX rootNode, we need to create an OSG MatrixTransform
							// to put the sequence geometry in before adding it to the OSG Sequence.
							if(!seqGeomNode->GetParentNode()->IsRootNode()){
								// Get absolute position of parent node.
								osg::Matrix parentMat = convertMat(seqGeomNode->GetParentNode()->GetObjTMBeforeWSM(t));
								osg::ref_ptr<osg::MatrixTransform> parentTransform = new osg::MatrixTransform();
								// Set static datavariance for better performance
								parentTransform->setDataVariance(osg::Object::STATIC);
								// Use default node mask
								if(_options->getUseDefaultNodeMaskValue())
									parentTransform->setNodeMask(_options->getDefaultNodeMaskValue());

								parentTransform->setMatrix(parentMat);
								parentTransform->addChild(transform.get());
								seq->addChild(parentTransform.get());
							}
							else{
								seq->addChild(transform.get());
							}
						}
						// The frame time for the geometry in secs.
						frameTime += (float)(delta)/(float)(GetTicksPerFrame()*GetFrameRate());					
					}
					// Set frame time for last geoemtry
					seq->setTime(n++, frameTime);
				}
			}
		}
		// or use static objects.
		else{
			for(int n=0; n<NUM_SEQUENCE_OBJECTS; n++){
				INode* seqGeomNode = pblock2->GetINode(sequence_node+n, t);
				if(seqGeomNode){
					// Assert this is a geometry node
					ObjectState os = seqGeomNode->EvalWorldState(t); 
					if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){

						// Create sequence geometry; this geometry is placed relative!
						osg::ref_ptr<osg::MatrixTransform> transform =  createGeomObject(rootTransform, seqGeomNode, os.obj, t);
	
						// Get time value from helper object.
						float time;
						pblock2->GetValue(sequence_time, t, time, iv, n);
				        seq->setTime(n, time);

						// If the seqGeomNode has a parentnode different from
						// the MAX rootNode, we need to create an OSG MatrixTransform
						// to put the sequence geometry in before adding it to the OSG Sequence.
						if(!seqGeomNode->GetParentNode()->IsRootNode()){
							// Get absolute position of parent node.
							osg::Matrix parentMat = convertMat(seqGeomNode->GetParentNode()->GetObjTMBeforeWSM(t));
							osg::ref_ptr<osg::MatrixTransform> parentTransform = new osg::MatrixTransform();
							// Set static datavariance for better performance
							parentTransform->setDataVariance(osg::Object::STATIC);
							// Use default node mask
							if(_options->getUseDefaultNodeMaskValue())
								parentTransform->setNodeMask(_options->getDefaultNodeMaskValue());
							parentTransform->setMatrix(parentMat);
							parentTransform->addChild(transform.get());
							seq->addChild(parentTransform.get());
						}
						else{
							seq->addChild(transform.get());
						}
					} // End geometry node.
				}
			}// End for number of static sequence objects in pblock2
		}
	}

	// Return OSG Sequence.
	return seq;
}

/**
 * This method will create an osg::Switch object from the 
 * properties defined in an OSG Switch helper object. 
 */
osg::ref_ptr<osg::Switch> OSGExp::createSwitchFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){
	osg::ref_ptr<osg::Switch> switcher = new osg::Switch();
	switcher->setName(node->GetName());
	// Set static datavariance for better performance
	switcher->setDataVariance(osg::Object::DYNAMIC);
	// Use default node mask
	if(_options->getUseDefaultNodeMaskValue())
		switcher->setNodeMask(_options->getDefaultNodeMaskValue());


	// The switch properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	// Add geometry to osg::switch.
	if(pblock2){
		for(int n=0; n<pblock2->Count(switch_nodes); n++){
			INode* switchGeomNode = pblock2->GetINode(switch_nodes, t,n);
			if(switchGeomNode){
				// Assert this is a geometry node
				ObjectState os = switchGeomNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){

					// Create switch geometry; this geometry is placed relative!
					osg::ref_ptr<osg::MatrixTransform> transform =  createGeomObject(rootTransform, switchGeomNode, os.obj, t);
	
					// If the switchGeomNode has a parentnode different from
					// the MAX rootNode, we need to create an OSG MatrixTransform
					// to put the switch geometry in before adding it to the OSG Switch.
					if(!switchGeomNode->GetParentNode()->IsRootNode()){
						// Get absolute position of parent node.
						osg::Matrix parentMat = convertMat(switchGeomNode->GetParentNode()->GetObjTMBeforeWSM(t));
						osg::ref_ptr<osg::MatrixTransform> parentTransform = new osg::MatrixTransform();
						// Set static datavariance for better performance
						parentTransform->setDataVariance(osg::Object::STATIC);
						// Use default node mask
						if(_options->getUseDefaultNodeMaskValue())
							parentTransform->setNodeMask(_options->getDefaultNodeMaskValue());
						parentTransform->setMatrix(parentMat);
						parentTransform->addChild(transform.get());
						switcher->addChild(parentTransform.get());
					}
					else{
						switcher->addChild(transform.get());
					}
				} // End geometry node.
			}
		}
	}
	// Return switch.
	return switcher;
}

/**
 * This method will create an osg::Impostor object from the 
 * properties defined in an OSG Impostor helper object. 
 */
osg::ref_ptr<osg::Group> OSGExp::createImpostorFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){

	osg::ref_ptr<osg::Group> group = new osg::Group();
	group->setName(node->GetName());
	// Set static datavariance for better performance
	group->setDataVariance(osg::Object::STATIC);
	// Use default node mask
	if(_options->getUseDefaultNodeMaskValue())
		group->setNodeMask(_options->getDefaultNodeMaskValue());

	/*
	// The impostor properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	// Add geometry to osg::impostor
	if(pblock2){
		for(int n=0; n<pblock2->Count(impostor_nodes); n++){
			INode* impostorGeomNode = pblock2->GetINode(impostor_nodes, t,n);
			if(impostorGeomNode){
				// Assert this is a geometry node
				ObjectState os = impostorGeomNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ){
					// Create an impostor for every geometry node.
					osg::ref_ptr<osg::Impostor> impostor = new osg::Impostor;
					// Set static datavariance for better performance
					impostor->setDataVariance(osg::Object::STATIC);
					// Use default node mask
					if(_options->getUseDefaultNodeMaskValue())
						impostor->setNodeMask(_options->getDefaultNodeMaskValue());

					// Create impostor geometry; this geometry is placed relative!
					osg::ref_ptr<osg::MatrixTransform> transform =  createGeomObject(rootTransform, impostorGeomNode, os.obj, t);
					
					// Bounding sphere for the impostor geometry.
					osg::BoundingSphere bs;
	
					// If the impostorGeomNode has a parentnode different from
					// the MAX rootNode, we need to create an OSG MatrixTransform
					// to put the impostor geometry in before adding it to the osg:Impostor.
					if(!impostorGeomNode->GetParentNode()->IsRootNode()){
						// Get absolute position of parent node.
						osg::Matrix parentMat = convertMat(impostorGeomNode->GetParentNode()->GetObjTMBeforeWSM(t));
						osg::ref_ptr<osg::MatrixTransform> parentTransform = new osg::MatrixTransform();
						// Set static datavariance for better performance
						parentTransform->setDataVariance(osg::Object::STATIC);
						// Use default node mask
						if(_options->getUseDefaultNodeMaskValue())
							parentTransform->setNodeMask(_options->getDefaultNodeMaskValue());
						parentTransform->setMatrix(parentMat);
						parentTransform->addChild(transform.get());
						impostor->addChild(parentTransform.get());
						bs = parentTransform->getBound();
					}
					else{
						impostor->addChild(transform.get());
						bs = transform->getBound();
					}

					// Set impostor properties.
					float threshold;
					Interval iv;
					pblock2->GetValue(threshold_value, t, threshold, iv);
					impostor->setImpostorThresholdToBound(threshold);
					impostor->setRange(0,0.0f,1e7f);
					impostor->setCenter(bs.center());
					// Add impostor to group.
					group->addChild(impostor.get());

				} // End geometry node.
			}
		}
	}*/
	// Return impostor.
	return group;
}

/**
 * This method will create an osg::Occluder object from the 
 * properties defined in an OSG Occluder helper object. 
 */
void OSGExp::createOccluderFromHelperObject(osg::Group* rootTransform, INode* node, Object* obj, TimeValue t){


	// The Occluder properties are saved in a parameter block.
	IParamBlock2* pblock2 = (IParamBlock2*)obj->GetParamBlock();
	Interval iv;
	// Add geometry to osg::Occluder.
	if(pblock2){
		for(int n=0; n<pblock2->Count(occluder_planes); n++){
			INode* geomNode = pblock2->GetINode(occluder_planes, t,n);
			if(geomNode){
				// Assert this is a shape node
				ObjectState os = geomNode->EvalWorldState(t); 
				if(os.obj->SuperClassID() == SHAPE_CLASS_ID ){

					// Get the absolute position of shape.
					osg::Matrix mat = convertMat(geomNode->GetObjTMBeforeWSM(t));
	
					// Get shape geometry.
					osg::ref_ptr<osg::Geode>  geode = createShapeGeometry(rootTransform, geomNode, os.obj, t);

					// Transform all vertices in shape geometry to absolute position
					// and insert them into occluder.
					osg::Vec3Array* coords = static_cast<osg::Vec3Array*>(((osg::Geometry*)geode->getDrawable(0))->getVertexArray());
					if(coords){
						// Create occluder.
						osg::OccluderNode* occluderNode = new osg::OccluderNode;
						occluderNode->setName(node->GetName());
						// Set static datavariance for better performance
						occluderNode->setDataVariance(osg::Object::STATIC);
						// Use default node mask
						if(_options->getUseDefaultNodeMaskValue())
							occluderNode->setNodeMask(_options->getDefaultNodeMaskValue());
						osg::ConvexPlanarOccluder* cpo = new osg::ConvexPlanarOccluder;
						occluderNode->setOccluder(cpo);
						osg::ConvexPlanarPolygon& occluder = cpo->getOccluder();
						// Add occluder to rootTransform.
						rootTransform->addChild(occluderNode);
						int numCoords = coords->getNumElements();
						for(int i=0;i<numCoords;i++){
							osg::Vec3 v = (*coords)[i];
							v = mat.preMult(v);
							occluder.add(v);
						}
					}
				}
			}
		}
	}
}

