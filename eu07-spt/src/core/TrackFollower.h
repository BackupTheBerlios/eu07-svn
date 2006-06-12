#include "PathFollower.h"
#include "Track.h"

namespace spt
{

class TrackFollower : public PathFollower
{

public:
	TrackFollower();
	TrackFollower(Track* track, distance = 0.0f);
	TrackFollower(TrackFollower* master);

protected:

} // TrackFollower

} // namespace spt
