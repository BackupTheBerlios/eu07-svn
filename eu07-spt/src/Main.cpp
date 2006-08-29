#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgProducer/Viewer>

int main() {

   osg::Node* sem = NULL;
   osg::Group* root = NULL;
   osgProducer::Viewer viewer;

   sem = osgDB::readNodeFile("SS4zcpbI.ive");

   root = new osg::Group();
   root->addChild(sem);

   viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);
   viewer.setSceneData( root );
   viewer.realize();

   while( !viewer.done() ) {
      viewer.sync();
      viewer.update();
      viewer.frame();
   }

}
