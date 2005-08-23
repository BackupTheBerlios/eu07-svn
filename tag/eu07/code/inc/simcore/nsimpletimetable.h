#ifndef N_SIMPLETIMETABLE_H
#define N_SIMPLETIMETABLE_H
//------------------------------------------------------------------------------
/**
    @class nSimpleTimetable

    @brief a brief description of the class

    a detailed description of the class

    (C)	2005	user
*/

#include "kernel/nkernelserver.h"

#ifndef N_ROOT_H
#include "kernel/nroot.h"
#endif

#undef N_DEFINES
#define N_DEFINES nSimpleTimetable
#include "kernel/ndefdllclass.h"

#include "simcore/nsimevent.h"

#include <vector>
//#include <set>
#include <string>
#include <osg/Referenced>

//------------------------------------------------------------------------------
class TimetableEntry : public osg::Referenced
{
public:
	std::string stationName;
	double arrivalTime;
	double departureTime;
	double minStopTime;
	std::vector<nSimEvent*> events;
private:
};

//------------------------------------------------------------------------------
class nSimpleTimetable : public nRoot
{
public:
	typedef std::vector< osg::ref_ptr<TimetableEntry> > TimetableEntries;
    /// constructor
    nSimpleTimetable();
    /// destructor
    virtual ~nSimpleTimetable();
    /// persistency
    virtual bool SaveCmds(nPersistServer* persistServer);

	virtual void Initialize();

    /// pointer to nKernelServer
    static nKernelServer* kernelServer;


	inline const TimetableEntry* actEntry() { return entries[actEntryID].get(); };
	bool nextEntry();


//private:

	TimetableEntries entries;

	unsigned int actEntryID;
};
//------------------------------------------------------------------------------
#endif

