#include "stdafx.h"

#include "bbox.h"





bool CBBox::isProximal(const CBBox& BBox) const
{
	// get the axis that coincides
	int ProximalAxis = -1;
	for (int i = 0; i < DIMENSION; ++i)
	{
		if ((this->Min.Elements[i] == BBox.Max.Elements[i]) || (this->Max.Elements[i] == BBox.Min.Elements[i]))
		{
			ProximalAxis = i;
			break;
		}
	}

	if (ProximalAxis == -1)
	{
		return false;
	}
 
	for (int i = 0; i < DIMENSION; ++i)
	{

		if (i == ProximalAxis)
		{
			continue;
		}

		if ((this->Min.Elements[i] > BBox.Max.Elements[i]) || (BBox.Min.Elements[i] > this->Max.Elements[i]))
		{
			return false;
		}

		//if ((this->Min.Elements[i] - BBox.Max.Elements[i]) > 1E-3)
		//{
		//	return false;
		//}
		//	
		//if ((BBox.Min.Elements[i] - this->Max.Elements[i]) > 1E-3)
		//{
		//	return false;
		//}


	}

	return true;
}



float CBBox::ComputeVolume() const
{
  float Volume = 1.0f;

  for (int i = 0; i < DIMENSION; ++i)
  {
    Volume *= (this->Max.Elements[i] - this->Min.Elements[i]);
  }

  return Volume;
}
 




void CBBox::Reset()
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		this->Min.Elements[i] = 1E20f;
		this->Max.Elements[i] = -1E20f;
	}
}





void CBBox::Fit(const CVector& Vector)
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		if (this->Min.Elements[i] > Vector.Elements[i]) 
		{
			this->Min.Elements[i] = Vector.Elements[i];
		}
		if (this->Max.Elements[i] < Vector.Elements[i]) 
		{
			this->Max.Elements[i] = Vector.Elements[i];
		}
	}
}





bool CBBox::Check(const CVector& Vector) const
{
	for (int i = 0; i < DIMENSION; ++i)
	{
		if (this->Min.Elements[i] > Vector.Elements[i]) 
		{
			return false;
		}
		if (this->Max.Elements[i] < Vector.Elements[i]) 
		{
			return false;
		}
	}

	return true;
}





int CBBox::GetLargestAxis() const
{
	int LargestAxis = 0;
	float MaxLength = 0.0f;
	for (int i = 0; i < DIMENSION; ++i)
	{
		float Length = this->Max.Elements[i] - this->Min.Elements[i];
		if (Length > MaxLength)
		{
			LargestAxis = i;
			MaxLength = Length;
		}
	}

	return LargestAxis;
}




float CBBox::ComputeVolume2D() const
{
  float Volume = 1.0f;

  for (int i = 0; i < 2; ++i)
  {
    Volume *= (this->Max.Elements[i] - this->Min.Elements[i]);
  }

  return Volume;
}



int CBBox::GetLargestAxisID() const
{
	int LargestAxis = 0;
	float MaxLength = 0.0f;
	for (int i = 2; i < DIMENSION; ++i)
	{
		float Length = this->Max.Elements[i] - this->Min.Elements[i];
		if (Length > MaxLength)
		{
			LargestAxis = i;
			MaxLength = Length;
		}
	}

	return LargestAxis;
}


int CBBox::GetLargestAxis2D() const
{
	int LargestAxis = 0;
	float MaxLength = 0.0f;
	for (int i = 0; i < 2; ++i)
	{
		float Length = this->Max.Elements[i] - this->Min.Elements[i];
		if (Length > MaxLength)
		{
			LargestAxis = i;
			MaxLength = Length;
		}
	}

	return LargestAxis;
}
