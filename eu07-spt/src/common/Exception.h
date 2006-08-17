#ifndef SPT_EXCEPTION
#define SPT_EXCEPTION 1

#include <string>

namespace sptCommon {

class Exception{
public:
	Exception(std::string error);
	~Exception();
	std::string getError(){return _error;};
private:
	std::string _error;
};

} // namespace sptFileIO

#endif
