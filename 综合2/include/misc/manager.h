#ifndef manager_H
#define manager_H

#include "user.h"

class manager : public User
{
public:
	manager();
	USERTYPE getUserType();
};

#endif