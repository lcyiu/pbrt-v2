#include "stdafx.h"

#include "vector.h"





void CVector::UniformRandomHyperSpehere(MTRand& mtrand)
{
  do
  {
    for (int i = 0; i < DIMENSION; i++)
    {
      this->Elements[i] = 2.0f * (float)mtrand.rand() - 1.0f;
    }
  }
  while (this->DotProduct((*this)) > 1.0f);
}





void CVector::Normalize()
{
	float Length = sqrtf(this->DotProduct(*this));
  float Scale = 1.0f / (Length + 1E-20f);

  for (int i = 0; i < DIMENSION; i++)
  {
    this->Elements[i] = this->Elements[i] * Scale;
  }
}





CVector CVector::operator*(float Scalar) const
{
  CVector Result;
  for (int i = 0; i < DIMENSION; i++)
  {
    Result.Elements[i] = this->Elements[i] * Scalar;
  }

  return Result;
}





CVector CVector::operator+(const CVector& Vector) const 
{
  CVector Result;
  for (int i = 0; i < DIMENSION; i++)
  {
    Result.Elements[i] = this->Elements[i] + Vector.Elements[i];
  }

  return Result;
}







 
float CVector::SquaredDistance(const CVector& Vector) const
{
  CVector Difference = (*this) - Vector;
  return Difference.DotProduct(Difference);
}





float CVector::Distance(const CVector& Vector) const
{
  return sqrtf(this->SquaredDistance(Vector));
}
