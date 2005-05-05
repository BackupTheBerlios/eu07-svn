#ifndef TRACKS_HPP
#define TRACKS_HPP 1

#include "nodes.h"
#include "segment.h"
#include "TrackPiece.h"

class edTrack;
class Connection;
//class TrackPiece;

class edTrack : public edGeomNode
{
public:
	SETUP_PROPS(edTrack,'TRCK');
	static void setupProps(Properties &pr)
	{
		edGeomNode::setupProps(pr);
		registerProp(pr,"name",setName,getName,"");
	}	
//	virtual unsigned int getType() { return 'TRCK'; };
	edTrack() : edGeomNode(), railsTex("Rail_screw_used1.dds"), ballastTex("TpD1.dds") {};
//	virtual float getDist2(osg::Vec2d pt) { return edPoint::getDist2(pt); };
	virtual void free();
	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void setupTrackIDs(unsigned int &id);
	virtual void export(std::ostream &stream);

	virtual void onSelect(osg::Geode *geom);
	virtual void onDeselect();

	virtual void updateVisual();

	void setBallastTex(const char* tex) 
	{ 
		ballastTex= tex; 
		if (ballastTex2D.valid()) 
		{ 
			ballastTex2D->setImage(osgDB::readImageFile(ballastTex.c_str())); 
			ballastTex2D->releaseGLObjects();
			redrawAll(); 
		} 
	};
	const char* getBallastTex() { return ballastTex.c_str(); };
	void setRailsTex(const char* tex) 
	{
		railsTex= tex; 
		if (railsTex2D.valid()) 
		{
			railsTex2D->setImage(osgDB::readImageFile(railsTex.c_str())); 
			railsTex2D->releaseGLObjects();
			redrawAll(); 
		} 
	};
	const char* getRailsTex() { return railsTex.c_str(); };


	inline unsigned int getConnectionID(Connection *con) { for (unsigned int i=0; i<connections.size(); i++) if (connections[i].get()==con) return i; return 0xFFFFFFFF; };
	inline Connection* getConnection(unsigned int id) { return ( id<connections.size() ? connections[id].get() : NULL );  };

	inline TrackPiece* getTrackPiece(unsigned int i) { return (i<trackPieces.size() ? trackPieces[i].get() : NULL ); };

	virtual const char* getGroup() { return NULL; };

	TrackPiece *findAdjacentTrack(osg::Vec3d pos, osg::Vec3d dir, int &type);

	void setName(const char* tex) { name= tex;  onSetName(); };
	const char* getName() { return name.c_str(); };

private:

protected:
	virtual ~edTrack() {};

	virtual void onSetName() {};

	std::string name;

	std::vector< osg::ref_ptr<TrackPiece> > trackPieces;
	std::vector< osg::ref_ptr<Connection> > connections;

	std::string ballastTex;
	std::string railsTex;

	osg::ref_ptr<osg::Texture2D> ballastTex2D;
	osg::ref_ptr<osg::Texture2D> railsTex2D;
};

class Connection : public osg::Referenced
{
public:
	typedef enum { ct_None=-1, ct_Prev=0, ct_Next=1 } ConnectionType;
//		Connection(edTrack *_owner, ConnectionType _type=ct_None) : owner(_owner), node(NULL), pos(0,0,0), dir(0,1,0), type(_type), connected(NULL)	{};
	Connection(edTrack *_owner, ConnectionType _type=ct_None);
	virtual ~Connection();

	inline edTrack* getOwner() { return owner; };
	inline Connection* getConnected() { return connected; };
	inline class edTrackConnection *getNode() { return node; };

	inline unsigned int getID() { return (owner ? owner->getConnectionID(this) : 0xFFFFFFFF); };

	inline ConnectionType getType() { return type; };

	inline void setTrackPieceOwner(TrackPiece *tp, ConnectionType _type=ct_None) { trackPieceOwner= tp; type= _type; };
	inline TrackPiece *getTrackPieceOwner() { return trackPieceOwner; };

//		inline osg::Vec3d getWorldPos() { return owner->getMatrix().getTrans(); };
//		inline osg::Vec3d getWorldPos() { return owner->getMatrix()*osg::Vec3d(pos); };
	inline osg::Vec3d getWorldPos() { return osg::Vec3d(pos)*owner->getMatrix(); };
	inline osg::Vec3f getWorldDir() { return osg::Matrixd::transform3x3(dir,owner->getMatrix()); };
//		inline osg::Vec3d getWorldDir() { return osg::Matrixd::transform3x3(owner->getMatrix(),dir); };
	inline osg::Vec3f getPos() { return pos; };
	inline osg::Vec3f getDir() { return dir; };

	void setPos(osg::Vec3f _pos);
	void setDir(osg::Vec3f _dir);

	bool connectTo(Connection *connection);
	bool connectAndAllign(Connection *connection);
	bool disconnect();

	virtual void update();

private:
	Connection() {};
	osg::Vec3f pos,dir;
	edTrack *owner;
	edTrackConnection *node;
	ConnectionType type;
	Connection *connected;
	TrackPiece *trackPieceOwner;
};

class edTrackConnection : public edPoint
{
	friend class Connection;
public:
	SETUP_PROPS(edTrackConnection, 'TRKC');
	static void setupProps(Properties &pr)
	{
		edPoint::setupProps(pr);
	}
//	virtual unsigned int getType() { return 'TRKC'; };
	edTrackConnection() : edPoint(), owner(NULL) { setFlag(nf_NoSave|nf_Locked); clearFlag(nf_ExportToDelanuay); };
	//void onDeselect();
	virtual unsigned int getDeselectedVisualNum() { return (getOwner()->getConnected() ? 2 : 0); };
	void onConnectionChange();
	virtual void updateVisual();
	inline Connection *getOwner() { return owner; };

protected:
	Connection *owner;
};

class edFlexTrack : public edTrack
{
public:
	SETUP_PROPS(edFlexTrack, 'FTRK');
	static void setupProps(Properties &pr)
	{
		edTrack::setupProps(pr);
		registerProp(pr,"ballast tex",setBallastTex,getBallastTex,"TpD1.dds","\\texture","*.dds");
		registerProp(pr,"rails tex",setRailsTex,getRailsTex,"Rail_screw_used1.dds","\\texture","*.dds");
		registerProp(pr,"flags",setFlags,getFlags,"0");
	}
	//	virtual unsigned int getType() { return 'FTRK'; };

	typedef std::pair<edFlexTrack*,edFlexTrack*> FlexPair;

	edFlexTrack();
	virtual edFlexTrack *clone();

	virtual void load(std::istream &stream, int version, CollectNodes *cn);
	virtual void save(std::ostream &stream);
	virtual void export(std::ostream &stream);

	virtual FlexPair split(float maxLen= 10000);

	virtual void updateVisual();

	void setBezier(osg::Vec3f pt1, osg::Vec3f cpt1, osg::Vec3f cpt2, osg::Vec3f pt2, float roll1, float roll2);
	inline void setRailsShape(const Segment::ShapesList &shapes) { rails= shapes; };
	inline void setBallastShape(const Segment::ShapesList &shapes) { ballast= shapes; };

	Segment::ShapesList rails;
	Segment::ShapesList ballast;

protected:
	virtual ~edFlexTrack() { totalFexTracks--; };

	virtual void onSetName() { if (trackPieces.size()==1) trackPieces.front()->setName(name.c_str()); };
//	Segment segment;
	void setFlags(const char* f) { sscanf(f,"%x",&flags); trackPieces.front()->setFlags(flags); };
	const char* getFlags() { static char buf[16]; sprintf(buf,"%08x",flags); return buf; };

	unsigned int flags;

	static int totalFexTracks;
//	osg::ref_ptr<osg::Geode> visual;
//	osg::ref_ptr<osg::Geode> selectedVisual;
};

#endif