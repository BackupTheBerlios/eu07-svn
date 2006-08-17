#ifndef SPTFILEIO_MOVEMENTPATH
#define SPTFILEIO_MOVEMENTPATH 1

#include "../core/MovementPath.h"

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

namespace sptFileIO {

class DataInputStream;
class DataOutputStream;

class MovementPath: public spt::MovementPath {

public:

	class ControlPoint: public spt::MovementPath::ControlPoint {

	public:
		void read(DataInputStream* in);
		void write(DataOutputStream* out);

	}

	class Tip: public spt::MovementPath::Tip {

	public:
		void read(DataInputStream* in);
		void write(DataOutputStream* out);

	protected:

		inline ConnType readConnType(char ch);
		inline char writeConnType(ConnType ct);

	}; // MovementPath::Tip

	void read(DataInputStream* in);
	void write(DataOutputStream* out);

}; // class MovementPath

}

#endif
