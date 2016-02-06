

#include "HelperOpenMP.hpp"


bool	HelperOpenMP::isAvailable()
{
#ifdef	_OPENMP
	return true;
#else
	return false;
#endif
}

bool	HelperOpenMP::toUse = false;

