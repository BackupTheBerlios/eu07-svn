#include "common/DJBHash.h"

namespace spt {

unsigned int DJBHash(std::string str)
{

   unsigned int hash = 5381;

   for(unsigned int i = 0; i < str.length(); i++)
   {

      hash = ((hash << 5) + hash) + str[i];

   }

   return (hash & 0x7FFFFFFF);


}; // DJBHash

} // namespace spt
