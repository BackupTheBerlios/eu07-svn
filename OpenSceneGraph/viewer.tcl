namespace import osgTcl::*

loadwrapper osg
loadwrapper osgDB
loadwrapper osgUtil
loadwrapper osgGA
loadwrapper osgProducer

set root [readNodeFile dynamic/pkp/eu07/eu07-303e-m.ive]

set viewer [new osgProducer::Viewer]

call $viewer setUpViewer

call $viewer setSceneData $root

call $viewer realize

set done [call $viewer done]

while { $done == 0 } {
	call $viewer sync	
	call $viewer update	
	call $viewer frame
	set done [call $viewer done]
}

call $viewer sync

call $viewer cleanup_frame

call $viewer sync

exit

