#include <osg/Group>
#include <osg/Geode>
#include <osg/CameraNode>

#include <osgProducer/Viewer>

#include "Rectangle.h"
#include "TextInput.h"

int main() {

    osgProducer::Viewer viewer;
    viewer.setUpViewer();

    // create the model
    osg::Group* root = new osg::Group;

	// hud camera
    osg::CameraNode* camera = new osg::CameraNode;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,1024));

    // set the view matrix    
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::CameraNode::POST_RENDER);

    osg::StateSet* stateset = camera->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	osg::Geode* hud = new osg::Geode;

	{
		sptConsole::TextInput* input;
		input = new sptConsole::TextInput(hud);
		input->setPositionAndSize(10, 1024 - 400 + 20, 1280 - 20, 20);
		input->setFillColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
		input->build();

		registerTextInput(input, viewer);
	};
	
	{
		sptConsole::Rectangle* rect;
		rect = new sptConsole::Rectangle(hud);
		rect->setPositionAndSize(5, 1024 - 5, 1280 - 10, 400);
		rect->setFrameColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.5f));
		rect->setFrameWidth(2.0f);
		rect->setFillColor(osg::Vec4(0.5f, 0.5f, 1.0f, 0.5f));
		rect->build();
	};
	

	camera->addChild(hud);
	root->addChild(camera);

    // add model to viewer.
    viewer.setSceneData( root );

    // create the windows and run the threads.
    viewer.realize();

    while( !viewer.done() )
    {
        // wait for all cull and draw threads to complete.
        viewer.sync();

        // update the scene by traversing it with the the update visitor which will
        // call all node update callbacks and animations.
        viewer.update();
         
        // fire off the cull and draw traversals of the scene.
        viewer.frame();
        
    }
    
    // wait for all cull and draw threads to complete before exit.
    viewer.sync();

    return 0;

};