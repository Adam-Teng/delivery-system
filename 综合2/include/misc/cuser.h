#ifndef CUSER_H
#define CUSER_H

#include "user.h"

class Cuser : public User
{
public:
	Cuser();
	USERTYPE getUserType();
};

#endif