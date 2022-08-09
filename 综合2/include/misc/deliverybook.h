#ifndef DELIVERYBOOK_H
#define DELIVERYBOOK_H

#include "delivery.h"

class DeliveryBook : public Delivery
{
public:
	DeliveryBook();
	DELIVERYTYPE getDeliveryType() const;
};

#endif
