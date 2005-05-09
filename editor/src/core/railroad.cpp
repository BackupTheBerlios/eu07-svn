#include "railroad.h"
#include "lineFeatures.h"
#include "editor.h"
#include "tracks.h"
#include "MilePost.h"
/*
namespace FitBezier {
extern "C" {
#include "fitbezier.h"
}
};*/

	void edRailLine::setupProps(Properties &pr)
	{
		edSplineFeature::setupProps(pr);
		registerProp(pr,"ballast tex",setBallastTex,getBallastTex,"","\\texture","*.dds");
		registerProp(pr,"rails tex",setRailsTex,getRailsTex,"","\\texture","*.dds");
		registerProp(pr,"ballast shape",setBallastShape,getBallastShape,"StdBallast.shp","\\shapes","*.shp");
		registerProp(pr,"rails shape",setRailsShape,getRailsShape,"Std1435Rails.shp","\\shapes","*.shp");
//		registerProp(pr,"distance",setDistance,getDistance);
		registerProp(pr,"tracks dist [dm]",setTrackDistance,getTrackDistance,45,-1,9999999,1);
		registerProp(pr,"distance [hm]",setDistance,getDistance,-1,-1,9999999,1);
		registerProp(pr,"create",applyButton,NULL);
		registerProp(pr,"mileposts",setMilepostsI,getMilepostsI,1);
		registerProp(pr,"tracks",setNumTracks,getNumTracks,1,0,4);
		registerProp(pr,"terrain",setTerrainI,getTerrainI,1);
		registerProp(pr,"crone width",setCroneWidth,getCroneWidth,10,1,100);
		registerProp(pr,"crone step",setCroneStep,getCroneStep,20,5,100);
		registerProp(pr,"posts offset",setPostsOffset,getPostsOffset,3,1,20);
	}

edRailLine::edRailLine() : edSplineFeature(), numTracks(1), tracksDist(4.5), 
	tracksStep(5), croneWidth(10), croneStep(20), postsOffset(3), 
	distance(-1), side(0), mileposts(true), terrain(true) 
{ 
	setFlag(nf_DummyFlag);
	clearFlag(nf_ExportToDelanuay); 
}

void edRailLine::applyButton(int s)
{
	if (s==0)
		Editor::instance()->applyFeatures();
}

const osg::Vec3d ballastShapePts[]= { osg::Vec3d(-2.3,-0.2,0), osg::Vec3d(-1.2,0.2,0.33), osg::Vec3d(1.2,0.2,0.67), osg::Vec3d(2.3,-0.2,1) };
const int ballastShape[]= { 4 };
const osg::Vec3d railsShapePts[]= { osg::Vec3d(-0.79,0.2,0), osg::Vec3d(-0.79,0.38,0.44), osg::Vec3d(-0.718,0.38,0.56), osg::Vec3d(-0.718,0.2,1),
								 osg::Vec3d( 0.718,0.38,0.56), osg::Vec3d( 0.718,0.2,1), osg::Vec3d( 0.79,0.2,0), osg::Vec3d( 0.79,0.38,0.44) };
const int railsShape[]= { 4, 4 };

void edRailLine::load(std::istream &stream, int version, CollectNodes *cn)
{
	edSplineFeature::load(stream, version, cn);
	stream.read((char*)&numTracks,sizeof(numTracks));
	stream.read((char*)&tracksDist,sizeof(tracksDist));
	stream.read((char*)&tracksStep,sizeof(tracksStep));
	stream.read((char*)&croneWidth,sizeof(croneWidth));
	stream.read((char*)&croneStep,sizeof(croneStep));
	read(stream,postsOffset);
	read(stream,mileposts);
	read(stream,terrain);
	read(stream,ballastShape);
	read(stream,railsShape);
	read(stream,distance);
}

void edRailLine::save(std::ostream &stream)
{
	edSplineFeature::save(stream);
	stream.write((char*)&numTracks,sizeof(numTracks));
	stream.write((char*)&tracksDist,sizeof(tracksDist));
	stream.write((char*)&tracksStep,sizeof(tracksStep));
	stream.write((char*)&croneWidth,sizeof(croneWidth));
	stream.write((char*)&croneStep,sizeof(croneStep));
	write(stream,postsOffset);
	write(stream,mileposts);
	write(stream,terrain);
	write(stream,ballastShape);
	write(stream,railsShape);
	write(stream,distance);
}


void edRailLine::applyFeature()
{
	Editor::instance()->disableRedraws();
	osg::Texture2D *tex2d= NULL;
//	osg::Material *mater= NULL;
	osg::StateSet *dstate= NULL;
	osg::Geode *geode= NULL;
	osg::MatrixTransform *trans= NULL;
	osg::Matrixd mat;
	osg::Geometry *geom= NULL;

	osg::Vec3d origin;
	std::vector< osg::ref_ptr<edPoint> > points;
	edRailLine *nrl= NULL;
	osg::Vec3d dir1(segment.GetCPt1()-segment.GetPt1()); dir1.normalize();
	osg::Vec3d dir2(segment.GetCPt2()-segment.GetPt2()); dir2.normalize();
	Segment segments[5];
//	edRailLine *prl= NULL;
	if (points.empty())
	{
		
		nrl= dynamic_cast<edRailLine*>(getNext());

		double trackOffset= (numTracks-1)*tracksDist;
//		double croneOffset= trackOffset*0.5;
		osg::Vec3d parallel,pos;
		int steps= 0;
//		if (curve)
//		{
		//segment.ComputeTsBuffer();
		segment.PointsChange();
		double len= segment.GetLength();
//		double len= (pt1->getPosition()-pt2->getPosition()).length();
		parallel= (pt2->getPosition()-pt1->getPosition())^osg::Vec3d(0,0,1);
		parallel.normalize();

		segment.PointsChange();

		osg::Vec3d par1(dir1^osg::Vec3d(0,0,1)); par1.normalize();
		osg::Vec3d par2(dir2^osg::Vec3d(0,0,1)); par2.normalize();

		for (int i=0; i<numTracks; i++)
		{
			osg::Vec3d p1(pt1->getPosition()+par1*(tracksDist*i-trackOffset/2));
			osg::Vec3d p2(pt2->getPosition()-par2*(tracksDist*i-trackOffset/2));
			double len2= cvecLen((p1-p2).length());
//			double len2= (p1-p2).length()*0.3;
	
			segments[i].Init(p1, p1+dir1*len2, p2+dir2*len2, p2, 0, 0, curve);
		}

		if (terrain)
		{

		if (curve)
			steps= len/croneStep;
		else
			steps= len/200.0;

		if (steps<1)
			steps= 1;

		double t= 0;

		points.reserve((steps+1)*2);

		int fi= 0;
//		prl= dynamic_cast<edRailLine*>(getPrev());
		if (corners[0].valid() && corners[1].valid())
		{
			fi+= 1;
			points.push_back(corners[0]);
			points.push_back(corners[1]);
		}

		if (curve)
		{
			for (int i=fi; i<=steps; i++)
			{
				t= (double)i/(double)steps;
				
//				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t+parallel*croneWidth*0.5));
//				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t-parallel*croneWidth*0.5));
				parallel= segment.FastGetDirection(t,0.01)^osg::Vec3d(0,0,1);
				parallel.normalize();
				points.push_back(Editor::lastInstance()->createPoint(segment.FastGetPoint(t)+parallel*croneWidth*0.5));
				points.push_back(Editor::lastInstance()->createPoint(segment.FastGetPoint(t)-parallel*croneWidth*0.5));
			}
		}
		else
		{
			for (int i=fi; i<=steps; i++)
			{
				t= (double)i/(double)steps;
				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t+parallel*croneWidth*0.5));
				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t-parallel*croneWidth*0.5));
			}
		}

		Editor::lastInstance()->createLine(points[0].get(),points[1].get(),10000);
		if (nrl)
		{
			nrl->corners[0]= points[steps*2].get();
			nrl->corners[1]= points[steps*2+1].get();
		}
		for (int i=0; i<steps; i++)
		{
			Editor::lastInstance()->createLine(points[i*2].get(),points[(i+1)*2].get(),10000);
			Editor::lastInstance()->createLine(points[i*2+1].get(),points[(i+1)*2+1].get(),10000);
		}
		Editor::lastInstance()->createLine(points[steps*2].get(),points[steps*2+1].get(),10000);

		}


	//	FitBezier::Point2 beziers[1000];
//		osg::Vec3d tpt;
	//	FitBezier::Begin(1000);

		std::string bTex= "";
		Segment::ShapesList bs;
		Segment::LoadShapes(bs,bTex,ballastShape);

		std::string rTex= "";
		Segment::ShapesList rs;
		Segment::LoadShapes(rs,rTex,railsShape);

/*
		Segment::ShapesList bs(1);
		bs[0].reserve(4);
		bs[0].push_back(osg::Vec3f(-2.3,-0.2,0));
		bs[0].push_back(osg::Vec3f(-1.2,0.2,0.33));
		bs[0].push_back(osg::Vec3f(1.2,0.2,0.67));
		bs[0].push_back(osg::Vec3f(2.3,-0.2,1));

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
		rs[1].push_back(osg::Vec3d( 0.79,0.2,0));*/


		
		//if (numTracks==1)
		if (nrl)
			nrl->tracks.reserve(numTracks);
		for (int i=0; i<numTracks; i++)
		{
			edFlexTrack *trk= Editor::createFlexTrack(segments[i].GetPt1(),segments[i].GetCPt1(),
													  segments[i].GetCPt2(),segments[i].GetPt2(),
													  segments[i].getRoll1(),segments[i].getRoll2(),
													  bs,rs);
			trk->setBallastTex( ( ballastTex.empty() ? bTex.c_str() :ballastTex.c_str() ) );
			trk->setRailsTex( ( railsTex.empty() ? rTex.c_str() : railsTex.c_str() ) );
				
			
			edFlexTrack::FlexPair flexPair(trk->split(200));
			if (tracks.size()>unsigned int(i))
				flexPair.first->getConnection(0)->connectTo(tracks[i]->getConnection(1));
				
			if (nrl && nrl->numTracks==numTracks)
				nrl->tracks.push_back(flexPair.second);

//				printf("tracks: %d\n",nrl->tracks.size());

//				flexPair.second->getConnection(1)->connectTo(nrl->tracks[0]->getConnection(0));

//			tracks

		}

/*
		for (int j=0; j<numTracks; j++)
		{
			trackOffset-= tracksDist;

			origin= pt1->getPosition()*0.5+pt2->getPosition()*0.5;

			double len= segment.GetLength();
			int steps= len/100.0;
			double stepLen= len/steps;
			double pt= 0;
			double t= 0;

			edTrack::Connection *con= NULL;
//			edFlexTrack *trk= NULL;
			
			for (int i=1; i<=steps; i++)
			{
				pt= t;
				t= i*len/steps;
				for (int s=0; s<=10; s++)
				{
					tpt= segment.GetPoint(t+((double)s/10.0)*stepLen);
					FitBezier::PushPoint(tpt.x(),tpt.y());

				}
				int n= FitBezier::Fit(beziers);
				for (int b=0; b<n; b+=4)
				{
					osg::Vec3d  pt1(beziers[b+0].x,beziers[b+0].y,0);
					osg::Vec3d cpt1(beziers[b+1].x,beziers[b+1].y,0);
					osg::Vec3d cpt2(beziers[b+2].x,beziers[b+2].y,0);
					osg::Vec3d  pt2(beziers[b+3].x,beziers[b+3].y,0);
					osg::Vec3d pos((pt1+cpt1+cpt2+pt2)*0.25);
					pt1-= pos;
					cpt1-= pos;
					cpt2-= pos;
					pt2-= pos;
					
					edFlexTrack *ftrk= new edFlexTrack();
					ftrk->setBezier(pt1,cpt1,cpt2,pt2,0,0);
					ftrk->setBallastShape(bs);
					ftrk->setRailsShape(rs);
					ftrk->moveTo(pos);
					ftrk->getConnection(0)->connectTo(con);
					con= ftrk->getConnection(1);
				}
			}*/
/*

			geom= segment.CreateLoft(ballastShapePts, ballastShape, 1, 5, 5, origin);

			mat.makeTranslate(origin);

			tex2d= new osg::Texture2D();
			dstate= new osg::StateSet();
			geode= new osg::Geode();
			trans= new osg::MatrixTransform(mat);

			tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
			tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
			tex2d->setImage(osgDB::readImageFile("TpD1.dds",osgDB::Registry::CACHE_ALL));
			dstate->setTextureAttributeAndModes(0, tex2d, osg::StateAttribute::ON );
//			dstate->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
			geom->setStateSet(dstate);
//	osg::Material *mater= NULL;
			trans->addChild(geode);
			geode->addDrawable(geom);
//			geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0),10.0)));

			Editor::lastInstance()->getTerrainRoot()->addChild(trans);*/

//		}
//		FitBezier::End();

		double dist(distance<0 ? 0 : distance);
		double endDist= dist+len;
		double d= 100-fmod(dist,100);
		double t;

		edPointFeature *pf= NULL;
		edMilePost *mp= NULL;

		osg::Quat rot;

//		char buf[10];
//	    osg::Geode* geode = NULL;
//		osg::StateSet* stateset = dynamic_cast<osg::StateSet*>(Editor::getObjectFromCache("PostsStateSet"));
//		osgText::Text* text = NULL;
		float s= (side!=0 ? side : 1);
		
//	    std::string timesFont("arial.ttf");
//		if (numTracks==1)
		{
			while (d+dist<=endDist)
			{
//				int km= floor((d+dist)/1000);
//				int hm= fmod(d+dist,1000)/100;
				if (mileposts)
				{
					for (int i=0; i<numTracks; i+= ( numTracks<=1 ? 1 : numTracks-1 ) )
					{

					t= segment.GetTFromS(d);
					parallel= (segment.FastGetDirection(t,0.0001)^osg::Vec3d(0,0,1))*s;
					parallel.normalize();
					pos= segment.FastGetPoint(t);

					if ( fabs(osg::Vec3d(1,0,0)*parallel + 1.0) < 0.0000001 )
						rot.makeRotate(osg::DegreesToRadians(180.0),0,0,1);
					else
						rot.makeRotate(osg::Vec3d(1,0,0),parallel);

	//				rot.makeRotate(osg::Vec3d(1,0,0),parallel);

					mp= new edMilePost(d+dist);
					mp->setModel("hekt.ive");
					mp->setPositionAndRotation(pos+parallel*(postsOffset+trackOffset*0.5)*(i==0?1:-1),rot);
					}
				}
/*
				pf= new edPointFeature();
				pf->setModel("hekt.ive");
				pf->setPositionAndRotation(pos+parallel*postsOffset,rot);
				*/
//				pf->setFlag(nf_SaveVisuals);
/*
			    geode = new osg::Geode();
				if (stateset==NULL)
				{
					stateset= new osg::StateSet();
					stateset->setAttributeAndModes(new osg::PolygonOffset(-3,0));
					Editor::insertObjectToCache("PostsStateSet",stateset);
				}
				geode->setStateSet(stateset);
				text = new  osgText::Text;
				geode->addDrawable( text );
				text->setFont(timesFont);
				text->setCharacterSizeMode(osgText::Text::CharacterSizeMode::OBJECT_COORDS);
				text->setCharacterSize(0.3);
				text->setColor(osg::Vec4(0,0,0,1));
				text->setPosition(osg::Vec3d(0,-0.1,0.52));
				sprintf(buf,"%d",km);
				text->setText(buf);
				text->setAxisAlignment(osgText::Text::AxisAlignment::XZ_PLANE);
				text->setAlignment(osgText::Text::AlignmentType::CENTER_TOP);
//				text->setStateSet(stateset);

				text = new  osgText::Text;
				geode->addDrawable( text );
				text->setFont(timesFont);
				text->setCharacterSizeMode(osgText::Text::CharacterSizeMode::OBJECT_COORDS);
				text->setCharacterSize(0.3);
				text->setColor(osg::Vec4(0,0,0,1));
				text->setPosition(osg::Vec3d(0,-0.1,0.25));
				sprintf(buf,"%d",hm);
				text->setText(buf);
				text->setAxisAlignment(osgText::Text::AxisAlignment::XZ_PLANE);
				text->setAlignment(osgText::Text::AlignmentType::CENTER_TOP);
//				text->setStateSet(stateset);

//				pf->getModelVisual()->addChild(geode);
				pf->setAddon(geode);*/

				d+= 100;
				s= -s;
			}
		//	for (unsigned int i=0; i<
		}
		
		if (nrl)
		{
			if (nrl->distance<=0)
				nrl->distance= endDist;
			if (nrl->side==0)
				nrl->side= s;

		}

//		}
//		else
//		{
			/*
			double len= (pt1->getPosition()-pt2->getPosition()).length();
			parallel= (pt1->getPosition()-pt2->getPosition())^osg::Vec3d(0,0,1);
			parallel.normalize();

			int steps= len/200.0;
			double t= 0;


			points.reserve((steps+1)*2);

			int fi= 0;
			prl= dynamic_cast<edRailLine*>(getPrev());
			if (prl)
			{
				fi+= 1;
				points.push_back(prl->corners[0]);
				points.push_back(prl->corners[1]);
			}

			for (int i=fi; i<=steps; i++)
			{
				t= (double)i/(double)steps;
				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t+parallel*croneWidth*0.5));
				points.push_back(Editor::lastInstance()->createPoint(pt1->getPosition()*(1.0-t) + pt2->getPosition()*t-parallel*croneWidth*0.5));
			}

			Editor::lastInstance()->createLine(points[0].get(),points[1].get(),10000);
			corners[0]= points[steps*2].get();
			corners[1]= points[steps*2+1].get();
			for (int i=0; i<steps; i++)
			{
				Editor::lastInstance()->createLine(points[i*2].get(),points[(i+1)*2].get(),10000);
				Editor::lastInstance()->createLine(points[i*2+1].get(),points[(i+1)*2+1].get(),10000);
			}
			Editor::lastInstance()->createLine(points[steps*2].get(),points[steps*2+1].get(),10000);


			for (int j=0; j<numTracks; j++)
			{
				trackOffset-= tracksDist;
			}*/
//		}

//		nrl= dynamic_cast<edRailLine*>(getNext());
//		if (nrl)
//			nrl->applyFeature();
		edPoint *pt= ( isLast() ? pt2.get() : NULL);
		pt1->free();
		if (pt)
			pt->free();
	}
	Editor::instance()->enableRedraws();

}

void edRailLine::clearFeature()
{
	edLineFeature::clearFeature();
}

