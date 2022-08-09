#ifndef DELIVERYFRAGILE_H
#define DELIVERYFRAGILE_H

#include "delivery.h"

class DeliveryFragile : public Delivery
{
public:
	DeliveryFragile();
	DELIVERYTYPE getDeliveryType() const;
};

#endif
