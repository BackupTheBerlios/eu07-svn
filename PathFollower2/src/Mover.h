#ifndef MOVER_H
#define MOVER_H 1

namespace spt
{

class Mover
{

public:
	Mover();

	void update(float& dt);

private:
	std::string id;
	std::string physicsDll;
	std::string physicsType;
	
}

}

#endif