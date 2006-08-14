#ifndef OSG_MOVEMENTPATH
#define OSG_MOVEMENTPATH 1

#include <map>

#include <osg/Array>
#include <osg/Object>
#include <osg/Matrix>
#include <osg/Quat>
#include <osg/NodeCallback>

/*!
*
* \class MovementPath
* \brief Path of movement, based on osg::AnimationPath
*
* \date 22-04-2006
* \author Zbyszek "ShaXbee" Mandziejewicz
*
* \todo Events that being trigerred at entering movement path, passing over some distance, changing connections
*
*/

namespace spt {

class MovementPath {

public:

	/*!
	*
	* \class ControlPoint
	* \brief Object containing information about position, length and rotation of MovementPath part
	*
	* \date 22-04-2006
	* \author Zbyszek "ShaXbee" Mandziejewicz
	*
	*/
	class ControlPoint //: public Serializable
	{

	public:
		ControlPoint() : m_position(), m_rotation(), m_length(0) {} // default constructor, needed for creating containers

		ControlPoint(const osg::Vec3d& position): m_position(position),	m_rotation() {}

		ControlPoint(const osg::Vec3d& position, const osg::Quat& rotation, const double& length = 0.0f): m_position(position), m_rotation(rotation), m_length(length) {}

		//! Comparision operator for ControlPoints
		bool operator==(ControlPoint& cp) {	return (m_position	== cp.m_position); };

		void setPosition(const osg::Vec3d& position) { m_position =	position; }
		const osg::Vec3d& getPosition()	const {	return m_position; }
		void getPosition(osg::Vec3d& position) { position = m_position; }

		void setRotation(const osg::Quat& rotation)	{ m_rotation = rotation; }
		const osg::Quat& getRotation() const { return m_rotation; }
		void getRotation(osg::Quat& rotation) { rotation = m_rotation; }

		const double& getLength() const { return m_length; }

		//! Get position & rotation of point placed at ratio% between ControlPoint first and second
		inline void	interpolate(double ratio,const ControlPoint& first,	const ControlPoint&	second)
		{
			float oneMinusRatio	= 1.0f-ratio;
			m_position = first.m_position *	oneMinusRatio +	second.m_position*ratio;
			m_rotation.slerp(ratio,	first.m_rotation,second.m_rotation);
		}

		//! Get ControlPoint translation matrix
		inline void	getMatrix(osg::Matrix& matrix) const
		{
			matrix.postMult(osg::Matrix::rotate(m_rotation));
			matrix.postMult(osg::Matrix::translate(m_position));
		}

		//! Get ControlPoint inverted translation matrix
		inline void	getInverse(osg::Matrix&	matrix)	const
		{
			matrix.preMult(osg::Matrix::rotate(m_rotation.inverse()));
			matrix.preMult(osg::Matrix::translate(-m_position));
		}

	protected:

		osg::Vec3d	m_position;
		osg::Quat	m_rotation;
		double		m_length;

	};

	typedef std::pair<double, ControlPoint> ControlPointPair;
	typedef std::map<double, ControlPoint>  ControlPointMap;

	/*!
	*
	* \class Tip
	* \brief Manages connection between MovementPath's
	*
	* \date 22-04-2006
	* \author Zbyszek "ShaXbee" Mandziejewicz
	*
	*/

	class Tip
	{

	public:
		typedef enum {UNDEF = 0, FRONT = 1, BACK = 2} ConnType;
		typedef struct {MovementPath* path; ConnType connType;} Connection;

		Tip() : m_opposite(false), m_valid(false) { };
		Tip(Connection first, Connection second) { connect(first, second); };
		Tip(MovementPath* firstPath, ConnType firstType, MovementPath* secondPath, ConnType secondType) { connect(firstPath, firstType, secondPath, secondType); };

		void connect(Connection first, Connection second);
		void connect(MovementPath* firstPath, ConnType firstType, MovementPath* secondPath, ConnType secondType);

		void disconnect(MovementPath* path);

		MovementPath* getNext(MovementPath* from);

		const bool isOpposite() const { return m_opposite; };
		const bool isValid() const { return m_valid; };

	protected:

		inline ConnType readConnType(char ch);
		inline char writeConnType(ConnType ct);

		Connection m_first;
		Connection m_second;

		bool m_opposite;
		bool m_valid;

	}; // MovementPath::Tip

	class SmartIterator
	{

	public:
		SmartIterator() : m_movementPath(NULL), m_valid(false), m_dir(true) { }
		SmartIterator(MovementPath* movementPath): m_dir(true) { setMovementPath(movementPath); }

		virtual ~SmartIterator() { }

		bool isValid() { return m_valid; }

		MovementPath* getMovementPath() { return m_movementPath; }
		void setMovementPath(MovementPath* movementPath, bool dir = true);

		bool getDir() { return m_dir; }
		void setDir(bool dir) { m_dir = dir; }

		virtual SmartIterator& operator ++();
		virtual SmartIterator& operator --();
		virtual ControlPoint& operator ->();
		virtual ControlPoint& operator *();

		void move(double distance);

	protected:
		void getTipPath(MovementPath::Tip* tip);
		void getNextPath() { getTipPath(m_movementPath->getFrontTip()); }
		void getPrevPath() { getTipPath(m_movementPath->getBackTip()); }

		inline void incIter();
		inline void decIter();

		inline void getControlPoint();

		//! path that iterator is using
		MovementPath* m_movementPath;
		//! internal control point map
		const MovementPath::ControlPointMap* m_controlPointMap; 
		//! internal control point map iterator
		MovementPath::ControlPointMap::const_iterator m_fIter;
		MovementPath::ControlPointMap::const_iterator m_fNextIter;
		//! internal control point map reverse_iterator
		MovementPath::ControlPointMap::const_reverse_iterator m_rIter;
		MovementPath::ControlPointMap::const_reverse_iterator m_rNextIter;

		MovementPath::ControlPoint m_cp;

		//! distance from ControlPoint pointed by m_iter
		double m_distance;

		//! iterator valid
		bool m_valid;
		//! real direction of movement
		bool m_dir;

	}; // MovementPath::SmartIterator

	MovementPath() : m_length(0.0f), m_backTip(NULL), m_frontTip(NULL) { }
	MovementPath(osg::Vec3Array* points) : m_length(0.0f), m_backTip(NULL), m_frontTip(NULL) { insert(points); }

	virtual ~MovementPath() { }

	//! Given a specific distance, return the local ControlPoint frame for a point.
	virtual bool getInterpolatedControlPoint(double distance, ControlPoint& controlPoint) const;

	//! Insert new ControlPoint
	void insert(osg::Vec3d position);
	void insert(double distance, ControlPoint controlPoint);
	void insert(osg::Vec3Array* points);

	ControlPoint& front() { return m_controlPointMap.rbegin()->second; }
	ControlPoint& back() { return m_controlPointMap.begin()->second; }

	void setFrontTip(Tip* tip);
	Tip* getFrontTip() { return m_frontTip; };
	void setBackTip(Tip* tip);
	Tip* getBackTip() { return m_backTip; };

	const double& getLength() const { return m_length; }

	void setControlPointMap(ControlPointMap& cpm) { m_controlPointMap = cpm; }
	const ControlPointMap& getControlPointMap() const { return m_controlPointMap; }

	bool empty() const { return m_controlPointMap.empty(); }

protected:

	ControlPointMap m_controlPointMap;
	ControlPointMap::iterator m_lastCPIter;

	double m_length;

	Tip* m_backTip;
	Tip* m_frontTip;

}; // class MovementPath

}

#endif
