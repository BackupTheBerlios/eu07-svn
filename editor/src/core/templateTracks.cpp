
#include "templateTracks.h"
#include "tracks.h"
#include "editor.h"

void edTemplateTrack::load(std::istream &stream, int version, CollectNodes *cn)
{
	read(stream,templateFile); loadTemplateFile();	//HACK
	edTrack::load(stream,version,cn);
}

void edTemplateTrack::save(std::ostream &stream)
{
	write(stream,templateFile);
	edTrack::save(stream);
}

void edTemplateTrack::exportTemplate(std::ostream &stream)
{
	write(stream,getClassID());
	write(stream,std::string(getGroup()));
	unsigned int i= templateFile.find_last_of('.');
	if (i<templateFile.size())
		write(stream,templateFile.substr(0,i));
	else
		write(stream,templateFile);
	write(stream,pos);
	write(stream,rot.asVec4());
	unsigned int params= 0;
	write(stream,params);
//	edTrack::export(stream);
}
/*
class SetupGeoms : public osg::NodeVisitor
{
public:
	SetupGeoms() : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(NULL) {};
	SetupGeoms(osg::Referenced *_data) : NodeVisitor(TRAVERSE_ALL_CHILDREN), data(_data) {};

	virtual void apply(osg::Geode& node) 
	{
		node.setUserData(data);
		traverse(node);
	}
	virtual void apply(osg::Transform& node) 
	{
		node.setUserData(data);
		traverse(node);
	}
	osg::Referenced *data;
private:
};
*/
void edTemplateTrack::updateVisual()
{
	if (!geomTransform.valid())//(visuals.empty())
	{
		geomTransform= new osg::MatrixTransform;
		geomTransform->setNodeMask(Editor::nm_Tracks);

		std::string file(templateFile);
		geomTransform->addChild(osgDB::readNodeFile(file));
//		geomTransform->accept(SetupGeoms(this));

		Editor::instance()->getDynamicModelsRoot()->addChild(geomTransform.get());
//		setNumVisuals(1);
//		addVisual(geomTransform.get());
//		addSelectedVisual(geomTransform.get());

	}
	edNode::updateVisual();
	edTrack::updateVisual();
}

bool edTemplateTrack::loadTemplateFile()
{
//	typedef std::map< std::string, osg::ref_ptr<TrackPiece> > TracksMap;
//	TracksMap tracks;
	std::string fileName("templates/tracks/");

	unsigned int i= templateFile.find_last_of('.');
	if (i<templateFile.size())
		fileName.append(templateFile.substr(0,i));
	else
		fileName.append(templateFile);

//	fileName.append(templateFile);
	fileName.append(".tpl");

	std::ifstream file;
	try {
		file.open(fileName.c_str());
		std::string type;
		std::string name;
		unsigned int flags;
		std::string con1;
		std::string con2;
		osg::Vec3d pt1,cpt1,cpt2,pt2;
		Connection *c1;
		Connection *c2;

		while (!file.eof())
		{
			c1=c2= NULL;
			type.clear();
			file >> type;
			if (type.compare("#track")==0)
			{
				file >> name >> std::hex >> flags >> std::dec >> con1 >> con2 >> type;
				if (type.compare("#curve")==0)
				{
					file >>  pt1.x() >>  pt1.y() >>  pt1.z();
					file >> cpt1.x() >> cpt1.y() >> cpt1.z();
					file >> cpt2.x() >> cpt2.y() >> cpt2.z();
					file >>  pt2.x() >>  pt2.y() >>  pt2.z();
				}
				else if (type.compare("#line")==0)
				{
					file >>  pt1.x() >>  pt1.y() >>  pt1.z();
					file >>  pt2.x() >>  pt2.y() >>  pt2.z();
					cpt1= pt1*(2.0/3.0)+pt2*(1.0/3.0);
					cpt2= pt1*(1.0/3.0)+pt2*(2.0/3.0);
				}
				else
					throw "bad track type";

				if (con1.compare("#connection")==0)
				{
					c1= new Connection(this);
					c1->setPos(pt1);
					c1->setDir(pt1-cpt1);
					connections.push_back(c1);
				}
				if (con2.compare("#connection")==0)
				{
					c2= new Connection(this);
					c2->setPos(pt2);
					c2->setDir(pt2-cpt2);
					connections.push_back(c2);
				}
				trackPieces.push_back(new TrackPiece(c1,c2,this,name.c_str()));
				trackPieces.back()->setBezier(pt1,cpt1,cpt2,pt2,0,0);
				trackPieces.back()->setFlags(flags);
//				tracks[name]= new TrackPiece(NULL,NULL,this,name.c_str());


			}
/*			else if (type.compare("#connection")==0)
			{

			}*/
			else
				throw "bad type";
		}

	}
	catch (char *txt) {
		printf("%s\n",txt);
		file.close();
		return false;
	}
	file.close();
	return true;
}
