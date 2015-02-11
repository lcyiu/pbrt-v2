#ifndef __BSPHERE_H
#define __BSPHERE_H

#include "global.h"
#include "vector.h"

#include <vector>

class CBSphere
{
  public:
    CVector Center;
		float SquaredRadius;

	  void Combine(const CBSphere& BSphere);
		void Expand(const CVector& Vector);
		bool Check(const CVector& Vector) const;

  private:

};


#endif
