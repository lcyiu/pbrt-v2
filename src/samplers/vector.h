#ifndef __VECTOR_H
#define __VECTOR_H

#include "global.h"
#include <math.h>
#include "mt.h"





class CVector
{
  public:
    float Elements[DIMENSION];
 
    CVector operator+(const CVector& Vector) const;
    inline CVector operator-(const CVector& Vector) const;
    CVector operator*(float Scalar) const;

		CVector()
		{
			for (int i = 0; i < DIMENSION; i++)
			{
				this->Elements[i] = 0.0f;
			}
		}

    inline float DotProduct(const CVector& Vector) const;
    float Distance(const CVector& Vector) const;
    float SquaredDistance(const CVector& Vector) const;
		void Normalize();
    void UniformRandomHyperSpehere(MTRand& mtrand);

  private:

};


 


inline CVector operator*(float Scalar, const CVector& Vector)
{
  CVector Result;
  for (int i = 0; i < DIMENSION; i++)
  {
    Result.Elements[i] = Vector.Elements[i] * Scalar;
  }

  return Result;
}



inline CVector CVector::operator-(const CVector& Vector) const 
{
  CVector Result;
  for (int i = 0; i < DIMENSION; i++)
  {
    Result.Elements[i] = this->Elements[i] - Vector.Elements[i];
  }

  return Result;
}





inline float CVector::DotProduct(const CVector& Vector) const
{
  float Result = 0.0f;

  for (int i = 0; i < DIMENSION; i++)
  {
    Result = Result + this->Elements[i] * Vector.Elements[i];
  }

  return Result;
}




class CMatrix
{
  public:
    CVector Rows[DIMENSION];

    CMatrix()
    {
      // empty
    }
};



#endif
