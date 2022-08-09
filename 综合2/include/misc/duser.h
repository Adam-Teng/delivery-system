#ifndef DUSER_H
#define DUSER_H

#include "user.h"

class Duser : public User
{
public:
	Duser();
	USERTYPE getUserType();
};

#endif