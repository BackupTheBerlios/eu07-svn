#ifndef TRACK_PIECE_HPP
#define TRACK_PIECE_HPP 1

#include <osg/ref_ptr>
#include <osg/Referenced>
//#include "nodes.h"
#include "segment.h"
//#include "tracks.h"

class edTrack;
class Connection;
class edSignal;

class TrackPiece : public osg::Referenced
{
	friend class edFlexTrack;
public:
	/*
	class TPConnection
	{
	public:
		TPConnection();
		void setTrack(TrackPiece *t) { track= t; connectionType= ct_Track; };
		void setConnection(Connection *c) { connection= c; connectionType= ct_Connection; };
		TrackPiece *getTrack();

	private:
		union
		{
			Connection *connection;
			TrackPiece *track;
		};
		typedef enum { ct_Connection, ct_Track } ConnectionType;
		ConnectionType connectionType;

	};*/
	virtual unsigned int getType() { return 'STDT'; };
	TrackPiece(Connection *con1, Connection *con2, edTrack *own, const char *pieceName=NULL);
	virtual ~TrackPiece();

	virtual void loadSignals(std::istream &stream, int version, CollectNodes *cn);
	virtual void saveSignals(std::ostream &stream);

	virtual void export(std::ostream &stream);

	void setBezier(osg::Vec3d pt1, osg::Vec3d cpt1, osg::Vec3d cpt2, osg::Vec3d pt2, float roll1, float roll2);

	inline edTrack* getOwner() { return owner; };

	inline addSignal(edSignal *sig) { signals.insert(sig); };
	inline remSignal(edSignal *sig) { signals.erase(sig); };

	void markSignals(bool m);

	inline void setName(const char *n) { name= n; };

	inline void setUID(unsigned int val) { UID= val; };

	inline int isAdjacent(osg::Vec3d pos, osg::Vec3d dir)
	{
		dir.normalize();
		osg::Vec3d d1= segment.GetDirection1(); d1.normalize();
		osg::Vec3d d2= segment.GetDirection2(); d2.normalize();
		if (segment.GetPt1()==pos && d1*dir<-0.9)
			return 0;
		else if (segment.GetPt2()==pos && d2*dir<-0.9)
			return 1;
		else
			return -1;
	}

	inline void setFlags(unsigned int f) { flags= f; };
protected:
	TrackPiece();

	bool getConnectedTrack(unsigned int i, unsigned int &UID, int &type);

	Connection* connections[2];
	Segment segment;
	edTrack *owner;

	typedef std::set<edSignal*> SignalsList;

	std::string name;

	SignalsList signals;
//	osg::ref_ptr<TrackPiece> prev, next;
//	bool prevSwitchDirection, nextSwitchDirection;
	unsigned int UID;

	unsigned int flags;
};

#endif