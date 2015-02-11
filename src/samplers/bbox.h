#ifndef __BBOX_H
#define __BBOX_H

#include "global.h"
#include "vector.h"

#include <vector>

class CBBox
{
  public:
    CVector Min;
    CVector Max;

	  void Reset();
		void Fit(const CVector& Vector);
		int GetLargestAxis() const;
		bool Check(const CVector& Vector) const;
    float ComputeVolume() const;
		bool isProximal(const CBBox& BBox) const;

		int GetLargestAxis2D() const;
    int GetLargestAxisID() const;
    float ComputeVolume2D() const;

  private:

};


#endif
