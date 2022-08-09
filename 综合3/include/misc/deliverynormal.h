#ifndef DELIVERYNORMAL_H
#define DELIVERYNORMAL_H 

#include "delivery.h"

class DeliveryNormal : public Delivery
{
public:
	DeliveryNormal();
	DELIVERYTYPE getDeliveryType() const;
};

#endif
