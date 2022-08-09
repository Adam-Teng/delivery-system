#include "include/misc/delivery.h"
#include "include/data.h"

Delivery::Delivery()
{
	
}

int Delivery::getPrice() const
{
	return Data::getInstance()->delivery->getPrice(this);
}