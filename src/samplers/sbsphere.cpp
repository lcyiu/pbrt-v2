#include "stdafx.h"

#include "bsphere.h"





void CBSphere::Combine(const CBSphere& BSphere)
{
	CVector Offset = BSphere.Center - this->Center;
	Offset.Normalize();

	bool isAllZero = true;
	for (int i = 0; i < DIMENSION; ++i)
	{
		if (Offset.Elements[i] != 0.0f)
		{
			isAllZero = false;
			break;
		}
	}
	if (isAllZero)
	{
		Offset.Elements[0] = 1.0f;
	}

	CVector Vector0 = BSphere.Center + Offset * sqrtf(BSphere.SquaredRadius);
	float SquaredDistance = Vector0.SquaredDistance(this->Center);

	if (SquaredDistance > this->SquaredRadius)
	{
		this->SquaredRadius = SquaredDistance;
	}
}





bool CBSphere::Check(const CVector& Vector) const
{
	float SquaredDistance = Vector.SquaredDistance(this->Center);

	if (SquaredDistance > this->SquaredRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}





void CBSphere::Expand(const CVector& Vector)
{
	float SquaredDistance = Vector.SquaredDistance(this->Center);

	if (SquaredDistance > this->SquaredRadius)
	{
		this->SquaredRadius = SquaredDistance;
	}
}
