/**********************************************************************
 *
 *    FILE:            Exception.cpp
 *
 *    DESCRIPTION:    Exception throwed when error occur.
 *
 *    CREATED BY:        Rune Schmidt Jensen
 *
 *    HISTORY:        Created 17.03.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/


#include "Exception.h"

namespace spt
{

Exception::Exception(std::string error){
    _error = error;
}

Exception::~Exception(){}

};