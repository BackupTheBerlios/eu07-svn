
#include "scnImport.h"
#include "parser.h"
#include "tracks.h"
#include "templateTracks.h"

#include <osg/ref_ptr>



edSCNImport *edSCNImport::instance()
{
	static osg::ref_ptr<edSCNImport> self = new edSCNImport;
	//static edSCNImport self;
	return self.get();
}
	
edSCNImport::edSCNImport() : angle(0)
{
}

edSCNImport::~edSCNImport()
{
	printf("edSCNImport deleted :)");
}

bool edSCNImport::import(osg::Vec3d pos, double r)
{
	std::vector<edTrack*> tracks;
	edTrack* trk= NULL;
	if (scnFile.empty())
		return false;
	try
	{
		cParser parser(scnFile.c_str(), cParser::buffer_FILE);
		std::string token;

		token = "";
		parser.getTokens();
		parser >> token;
		tracks.reserve(1000);
		//for ( parser >> token; token != ""; parser >> token ) 
		while (!token.empty())
		{
			trk= NULL;
			if (token=="node")
			{
//				parser.ne
				parser.getTokens();	parser >> token;
				parser.getTokens();	parser >> token;
				parser.getTokens();	parser >> token;
				std::string name( token=="none" ? "" : token );
				parser.getTokens();	parser >> token;
				if (token=="track")
				{
					parser.getTokens();	parser >> token;
					if (token=="normal")
					{
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						osg::Vec3d pt1;
						osg::Vec3d cpt1;
						osg::Vec3d cpt2;
						osg::Vec3d pt2;
						double roll1, roll2;
						parser.getTokens(4);
						parser >> pt1.x() >> pt1.z() >> pt1.y() >> roll1;
						pt1.x()= -pt1.x();
						parser.getTokens(3);
						parser >> cpt1.x() >> cpt1.z() >> cpt1.y();
						cpt1.x()= -cpt1.x();
						parser.getTokens(3);
						parser >> cpt2.x() >> cpt2.z() >> cpt2.y();
						cpt2.x()= -cpt2.x();
						parser.getTokens(4);
						parser >> pt2.x() >> pt2.z() >> pt2.y() >> roll2;
						pt2.x()= -pt2.x();
						osg::Vec3d center(pt1*0.5+pt2*0.5);
						pt1-= center;
						pt2-= center;
						if (cpt1!=osg::Vec3d(0,0,0))
							cpt1+= pt1;
						else
							cpt1= pt1*(2.0/3.0)+pt2*(1.0/3.0);

						if (cpt2!=osg::Vec3d(0,0,0))
							cpt2+= pt2;
						else
							cpt2= pt1*(1.0/3.0)+pt2*(2.0/3.0);

							edFlexTrack *ft= new edFlexTrack();
							ft->setBezier(pt1,cpt1,cpt2,pt2,0,0);

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

							ft->moveTo(osg::Quat(osg::DegreesToRadians(angle),osg::Vec3d(0,0,1))*center+pos);
							ft->setRotation(osg::Quat(osg::DegreesToRadians(angle),osg::Vec3d(0,0,1)));
							trk= ft;
					}
					else if (token=="switch")
					{
						int l= 0;
						parser.getTokens();	parser >> l;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						parser.getTokens();	parser >> token;
						osg::Vec3d pt1;
						osg::Vec3d cpt1;
						osg::Vec3d cpt2;
						osg::Vec3d pt2;
						double roll1, roll2;
						parser.getTokens(4);
						parser >> pt1.x() >> pt1.z() >> pt1.y() >> roll1;
						pt1.x()= -pt1.x();
						parser.getTokens(3);
						parser >> cpt1.x() >> cpt1.z() >> cpt1.y();
						cpt1.x()= -cpt1.x();
						parser.getTokens(3);
						parser >> cpt2.x() >> cpt2.z() >> cpt2.y();
						cpt2.x()= -cpt2.x();
						parser.getTokens(4);
						parser >> pt2.x() >> pt2.z() >> pt2.y() >> roll2;
						pt2.x()= -pt2.x();

						pt2.z()= pt1.z();

						osg::Vec3d dr(pt2-pt1);
						dr.normalize();
						double a= acos(dr.y())*(dr.x()<0 ? 1 : -1);
//						double dp= dr*osg::Vec3d(0,1,0);
//						if (dr.



						for (int i=0; i<15; i++)
							parser.getTokens();	parser >> token;
						int r= 0;
						parser.getTokens();	parser >> r;

						//switchL34R300.ive
						if ((r==300 || r==-300) && l==34)
						{
						char dir= (r<0 ? 'R' : 'L');
						char buf[256];
						sprintf(buf,"switch%c%dR%d.ive",dir,l,abs(r));

						edTemplateTrack *tt= new edTemplateTrack();
						tt->setTemplate(buf);
						pt1+= osg::Quat(a,osg::Vec3d(0,0,1))*osg::Vec3d(0,l/2,0);
						tt->moveTo(osg::Quat(osg::DegreesToRadians(angle),osg::Vec3d(0,0,1))*pt1+pos);
						tt->setRotation(osg::Quat(osg::DegreesToRadians(angle)+a,osg::Vec3d(0,0,1)));
						trk= tt;
						}

					}

					if (trk)
					{

					for (unsigned int j=0; j<tracks.size(); j++)
						for (unsigned int c1=0; trk->getConnection(c1)!=NULL; c1++)
							for (unsigned int c2=0; tracks[j]->getConnection(c2)!=NULL; c2++)
								if ((trk->getConnection(c1)->getWorldPos()-tracks[j]->getConnection(c2)->getWorldPos()).length2()<0.01)
									trk->getConnection(c1)->connectTo(tracks[j]->getConnection(c2));
					tracks.push_back(trk);
					trk->setName(name.c_str());
					printf("importing %s\n",name.c_str());
					}

				}
			}

			do { 
				token.clear(); parser.getTokens(); parser >> token; 
			} while ( token!="node" && !token.empty() );
		} 
	}
	catch (...)
	{
		return false;
	}
/*
	for (unsigned int i=0; i<tracks.size(); i++)
		for (unsigned int j=i+1; j<tracks.size(); j++)
			for (unsigned int c1=0; tracks[i]->getConnection(c1)!=NULL; c1++)
				for (unsigned int c2=0; tracks[j]->getConnection(c2)!=NULL; c2++)
					if ((tracks[i]->getConnection(c1)->getWorldPos()-tracks[j]->getConnection(c2)->getWorldPos()).length2()<0.01)
						tracks[i]->getConnection(c1)->connectTo(tracks[j]->getConnection(c2));
						*/
	return true;
}
