#include "terrainnode.h"
#include "tracks.h"
#include "TrackPiece.h"
#include "signals.h"
#include "editor.h"
/*
TrackPiece *TrackPiece::TPConnection::getTrack()
{
	if (connectionType==ct_Track)
		return track;
	else
		return connection->
}
*/
TrackPiece::TrackPiece() : UID(0xFFFFFFFF), flags(0) //prevSwitchDirection(false), nextSwitchDirection(false), 
{
	connections[0]=connections[1]= NULL;
}

TrackPiece::TrackPiece(Connection *con1, Connection *con2, edTrack *own, const char *pieceName) : 
		UID(0xFFFFFFFF), owner(own), flags(0) //prevSwitchDirection(false), nextSwitchDirection(false), 
{
	connections[0]= con1;
	if (con1)
		con1->setTrackPieceOwner(this,Connection::ConnectionType::ct_Prev);
	connections[1]= con2;
	if (con2)
		con2->setTrackPieceOwner(this,Connection::ConnectionType::ct_Next);
	name= pieceName;
}

TrackPiece::~TrackPiece()
{
	while (!signals.empty())
		(*signals.begin())->clearTrackPiece(this);
}

void TrackPiece::loadSignals(std::istream &stream, int version, CollectNodes *cn)
{
	unsigned int n= edNode::readUI(stream);
	for (unsigned int i=0; i<n; i++)
	{
		unsigned int id= edNode::readUI(stream);
		edSignal *sig= dynamic_cast<edSignal*>(cn->ptsList[id]);
		sig->setTrackPiece(this);
	}
	
}

void TrackPiece::saveSignals(std::ostream &stream)
{
	edNode::write(stream,signals.size());
	for (SignalsList::iterator it= signals.begin(); it!=signals.end(); ++it)
		edNode::write(stream,(*it)->getUID());
	
}

void TrackPiece::export(std::ostream &stream)
{
	const unsigned int nullUID= 0xFFFFFFFF;
	int t= 0;
	unsigned int cUID= nullUID;
	const char *g= owner->getGroup();
	std::string group(g?g:"");
	edNode::write(stream,group);
	edNode::write(stream,name);
	edNode::write(stream,flags);
	osg::Matrixd &mat(owner->getMatrix());
	edNode::write(stream,mat.preMult(segment.GetPt1()));
	edNode::write(stream,mat.preMult(segment.GetCPt1()));
	edNode::write(stream,mat.preMult(segment.GetCPt2()));
	edNode::write(stream,mat.preMult(segment.GetPt2()));
	edNode::write(stream,segment.getRoll1());
	edNode::write(stream,segment.getRoll2());
	for (int i=0; i<2; i++)
	{
		if (getConnectedTrack(i,cUID,t))
		{
//			t= connections[i]->getConnected()->getType();
			printf("track %d connection %d UID: %d type: %d\n",UID,i,cUID,t);
			edNode::write(stream,cUID);//connections[i]->getConnected()->getTrackPieceOwner()->UID);
			edNode::write(stream,t);
		}
		else
			edNode::write(stream,nullUID);
	}
	edNode::write(stream,signals.size());
	for (SignalsList::iterator it= signals.begin(); it!=signals.end(); ++it)
		edNode::write(stream,(*it)->signalID);
}

bool TrackPiece::getConnectedTrack(unsigned int i, unsigned int &UID, int &type)
{
	UID= 0xFFFFFFFF;
	if (i<2)
		if (connections[i])
			if (connections[i]->getConnected())
			{
				UID= connections[i]->getConnected()->getTrackPieceOwner()->UID;
				type= connections[i]->getConnected()->getType();
				return true;
			}
			else;
		else
		{
			TrackPiece *tp= NULL;
			int t= -1;
			if (i==0)
				tp= getOwner()->findAdjacentTrack(segment.GetPt1(),segment.GetDirection1(),t);
			else
				tp= getOwner()->findAdjacentTrack(segment.GetPt2(),segment.GetDirection2(),t);

			if (tp)
			{
				UID= tp->UID;
				type= t;
				return true;
			}
		}
	return false;
}


void TrackPiece::setBezier(osg::Vec3d pt1, osg::Vec3d cpt1, osg::Vec3d cpt2, osg::Vec3d pt2, float roll1, float roll2)
{
	segment.Init(pt1,cpt1,cpt2,pt2,roll1,roll2);
	segment.PointsChange();
}

void TrackPiece::markSignals(bool m)
{
	for (SignalsList::iterator it= signals.begin(); it!=signals.end(); ++it)
		(*it)->setMarked(m);
}
