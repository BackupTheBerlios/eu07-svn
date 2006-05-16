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

namespace spt
{

class DataInputStream;
class DataOutputStream;

class MovementPath
{
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

		ControlPoint(const osg::Vec3d& position, const osg::Quat& rotation): m_position(position), m_rotation(rotation) {}

		//! Comparision operator for ControlPoints
		bool operator==(ControlPoint& cp) {	return (m_position	== cp.m_position); };

		void setPosition(const osg::Vec3d& position) { m_position =	position; }
		const osg::Vec3d& getPosition()	const {	return m_position; }
		void getPosition(osg::Vec3d& position) { position = m_position; }

		void setRotation(const osg::Quat& rotation)	{ m_rotation = rotation; }
		const osg::Quat& getRotation() const { return m_rotation; }
		void getRotation(osg::Quat& rotation) { rotation = m_rotation; }

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

		void read(DataInputStream* in);
		void write(DataOutputStream* out);

	protected:

		osg::Vec3d	m_position;
		osg::Quat	m_rotation;
		double		m_length;

	};

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

		void read(DataInputStream* in);
		void write(DataOutputStream* out);

		void debug();

	protected:

		inline ConnType readConnType(char ch);
		inline char writeConnType(ConnType ct);

		Connection m_first;
		Connection m_second;

		bool m_opposite;
		bool m_valid;

	}; // MovementPath::Tip

	MovementPath() : m_frontTip(NULL), m_backTip(NULL), m_length(0) { }
	MovementPath(osg::Vec3Array* points) : m_frontTip(NULL), m_backTip(NULL), m_length(0) { insert(points); };

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

	typedef std::pair<double, ControlPoint> ControlPointPair;
	typedef std::map<double, ControlPoint>  ControlPointMap;

	void setControlPointMap(ControlPointMap& cpm) { m_controlPointMap = cpm; }
	const ControlPointMap& getControlPointMap() const { return m_controlPointMap; }

	bool empty() const { return m_controlPointMap.empty(); }

	void debug();

	void read(DataInputStream* in);
	void write(DataOutputStream* out);

protected:

	virtual ~MovementPath() {}

	ControlPointMap m_controlPointMap;
	ControlPointMap::iterator m_lastCPIter;

	double m_length;

	Tip* m_backTip;
	Tip* m_frontTip;

}; // class MovementPath

};

#endif