#include "stdafx.h"

#include "pbrt.h"
#include "film.h"
#include "paramset.h"
#include "sampler.h"

// image reconstruction plugin


struct Splat
{
  float params[5];
  Spectrum L0, L1, L2, L3;
};


// SamoaImageFilm Declarations
class SamoaImageFilm : public Film 
{
  public:
	  // SamoaImageFilm Public Methods
	  SamoaImageFilm(int xres, int yres, const float crop[4], const string &filename, bool premult, int wf);
	  ~SamoaImageFilm() {delete pixels; delete isSampled;}

	  //void SamoaImageFilm::AddSample(float imageX, float imageY, float * params, const Spectrum &L, float alpha);
	  void AddSample(float imageX, float imageY, float * params, const Spectrum &L, float alpha, const Spectrum L0, const Spectrum L1, const Spectrum L2, const Spectrum L3);
    void AddSample(const Sample &sample, const Ray &ray, const Spectrum &L, float alpha) {Error("Add Sample requires more parameters.");}
	  void AddSample(const Sample &sample, const Ray &ray, const Spectrum &L, float alpha, void* data);
	  void GetSampleExtent(int *xstart, int *xend, int *ystart, int *yend) const;
	  void Clear();
	  void WriteImage();
	  void WriteImage(int frame);



  private:
	  // SamoaImageFilm Private Data
	  int writeFrequency, sampleCount;
	  string filename;
	  bool premultiplyAlpha;
	  float cropWindow[4];
	  int xPixelStart, yPixelStart, xPixelCount, yPixelCount;
	  struct Pixel 
    {
		  Pixel() : L(0.0f) 
      {
			  alpha = 0.0f;
			  weightSum = 0.0f;
		  }
		  Spectrum L;
		  float alpha;
      float weightSum;
	  };
	  BlockedArray<Pixel> *pixels;
    BlockedArray<bool> *isSampled;
};





// SamoaImageFilm Method Definitions
SamoaImageFilm::SamoaImageFilm(int xres, int yres, const float crop[4], const string &fn, bool premult, int wf)
: Film(xres, yres) 
{
  memcpy(cropWindow, crop, 4 * sizeof(float));
  filename = fn;
  premultiplyAlpha = premult;
  writeFrequency = sampleCount = wf;

  // Compute film image extent
  xPixelStart = Ceil2Int(xResolution * cropWindow[0]);
  xPixelCount = max(1, Ceil2Int(xResolution * cropWindow[1]) - xPixelStart);
  yPixelStart = Ceil2Int(yResolution * cropWindow[2]);
  yPixelCount = max(1, Ceil2Int(yResolution * cropWindow[3]) - yPixelStart);

  // Allocate film image storage
  pixels = new BlockedArray<Pixel>(xPixelCount, yPixelCount);
  isSampled = new BlockedArray<bool>(xPixelCount, yPixelCount);
}





void SamoaImageFilm::AddSample(float imageX, float imageY, float * params, const Spectrum &L, float alpha, const Spectrum L0, const Spectrum L1, const Spectrum L2, const Spectrum L3) 
{
  
  int x = (int)imageX;
  int y = (int)imageY;
  x = max(x, this->xPixelStart);
  x = min(x, this->xPixelStart + this->xPixelCount - 1);
  y = max(y, this->yPixelStart);
  y = min(y, this->yPixelStart + this->yPixelCount - 1);

  // Update pixel values with filtered sample contribution
  Pixel &pixel = (*pixels)(x - xPixelStart, y - yPixelStart);
  pixel.alpha += alpha * params[4];
  pixel.weightSum += params[4];   
  pixel.L.AddWeighted(params[4], L);

 
 // int x = (int)imageX;
 // int y = (int)imageY;
 // x = max(x, this->xPixelStart);
 // x = min(x, this->xPixelStart + this->xPixelCount - 1);
 // y = max(y, this->yPixelStart);
 // y = min(y, this->yPixelStart + this->yPixelCount - 1);

 // // Update pixel values with filtered sample contribution
 // Pixel &pixel = (*pixels)(x - xPixelStart, y - yPixelStart);
 // pixel.L.AddWeighted(1.0f, L);
 // pixel.alpha += alpha ;
 // pixel.weightSum += 1.0f;

	//(*isSampled)(x, y) = true;    

/*
  // Compute sample's raster extent
  int x0 = (int)(params[0]);
  int x1 = (int)(params[2]);
  int y0 = (int)(params[1]);
  int y1 = (int)(params[3]);
  x0 = max(x0, xPixelStart);
  x1 = min(x1, xPixelStart + xPixelCount);
  y0 = max(y0, yPixelStart);
  y1 = min(y1, yPixelStart + yPixelCount);
  
  for (int y = y0; y < y1; ++y)
  {
    for (int x = x0; x < x1; ++x) 
    {
      float su = (x - x0) / (float)(x1 - x0);
      float sv = (y - y0) / (float)(y1 - y0);

			Spectrum Lint = (1.0f - su) * ((1.0f - sv) * L0 + sv * L1) + su * ((1.0f - sv) * L2 + sv * L3);


      // Update pixel values with filtered sample contribution
      Pixel &pixel = (*pixels)(x - xPixelStart, y - yPixelStart);
      pixel.L.AddWeighted(params[4], Lint);
      pixel.alpha += alpha * params[4];
      pixel.weightSum += params[4];
    }
  }
*/
/*
  float k   = params[0];
  float xxF = params[1];
  float xyF = params[2];
  float yyF = params[3];
  float sd = params[4];
  float xrange = 2.5f * sd;
  float yrange = 2.5f * sd;

  // Compute sample's raster extent
  float dImageX = imageX - 0.5f;
  float dImageY = imageY - 0.5f;
  int x0 = Ceil2Int (dImageX - xrange);
  int x1 = Floor2Int(dImageX + xrange);
  int y0 = Ceil2Int (dImageY - yrange);
  int y1 = Floor2Int(dImageY + yrange);
  x0 = max(x0, xPixelStart);
  x1 = min(x1, xPixelStart + xPixelCount - 1);
  y0 = max(y0, yPixelStart);
  y1 = min(y1, yPixelStart + yPixelCount - 1);
  
  for (int y = y0; y <= y1; ++y)
  {
    for (int x = x0; x <= x1; ++x) 
    {
      // Evaluate filter value at $(x,y)$ pixel
      float dx = imageX - x;
      float dy = imageY - y;
      float filterWt = k * exp(-(xxF*dx*dx + xyF*dx*dy + yyF*dy*dy));

      // Update pixel values with filtered sample contribution
      Pixel &pixel = (*pixels)(x - xPixelStart, y - yPixelStart);
      pixel.L.AddWeighted(filterWt, L);
      pixel.alpha += alpha * filterWt;
      pixel.weightSum += filterWt;
    }
  }
*/
}





void SamoaImageFilm::AddSample(const Sample &sample, const Ray &ray, const Spectrum &L, float alpha, void* data) 
{
  Splat* splt = (Splat*)data;
	//float* sampleData = (float*)data;
	AddSample(sample.imageX, sample.imageY, &(splt->params[0]), L, alpha, splt->L0, splt->L1, splt->L2, splt->L3);
}





void SamoaImageFilm::Clear() 
{
  for (int y = 0; y < yPixelCount; ++y) 
  {
    for (int x = 0; x < xPixelCount; ++x)
    {
		  Pixel &pixel = (*pixels)(x, y);
		  pixel.L = Spectrum();
		  pixel.alpha = 1.0f;
		  pixel.weightSum = 0;
      
		  (*isSampled)(x, y) = false;    
    } 
	}
}





void SamoaImageFilm::GetSampleExtent(int *xstart, int *xend, int *ystart, int *yend) const 
{
	*xstart = Floor2Int(xPixelStart + .5f - 3.f);
	*xend   = Floor2Int(xPixelStart + .5f + xPixelCount + 3.f);
	*ystart = Floor2Int(yPixelStart + .5f - 3.f);
	*yend   = Floor2Int(yPixelStart + .5f + yPixelCount + 3.f);
}





void SamoaImageFilm::WriteImage(int frame)
{
	string oldfilename = filename;
	char suffix[20];
	filename += suffix;
	WriteImage();
	filename = oldfilename;
}





void SamoaImageFilm::WriteImage() 
{
	// Convert image to RGB and compute final pixel values
	int nPix = xPixelCount * yPixelCount;
	float *rgb = new float[3*nPix], *alpha = new float[nPix];
	int offset = 0;

  float MaxWeight = 0.0f;
  for (int y = 0; y < yPixelCount; ++y) 
  {
		for (int x = 0; x < xPixelCount; ++x) 
    {
			float weightSum = (*pixels)(x, y).weightSum;
			if (weightSum > MaxWeight)
      {
        MaxWeight = weightSum;
      }
		}
	}


	for (int y = 0; y < yPixelCount; ++y) 
  {
		for (int x = 0; x < xPixelCount; ++x) 
    {
			// Convert pixel spectral radiance to RGB
			float xyz[3];
			(*pixels)(x, y).L.XYZ(xyz);

			const float rWeight[3] = { 3.240479f, -1.537150f, -0.498535f };
			const float gWeight[3] = {-0.969256f,  1.875991f,  0.041556f };
			const float bWeight[3] = { 0.055648f, -0.204043f,  1.057311f };

			rgb[3*offset  ] = rWeight[0]*xyz[0] + rWeight[1]*xyz[1] + rWeight[2]*xyz[2];
			rgb[3*offset+1] = gWeight[0]*xyz[0] + gWeight[1]*xyz[1] + gWeight[2]*xyz[2];
			rgb[3*offset+2] = bWeight[0]*xyz[0] + bWeight[1]*xyz[1] + bWeight[2]*xyz[2];
			alpha[offset] = 1.0f;//(*pixels)(x, y).alpha;

      
   //   // for visualization purpose
   //   if ((*isSampled)(x, y))
   //   {
			//	rgb[3*offset  ] += 0.1f;
			//	rgb[3*offset+1] += 0.1f;
			//	rgb[3*offset+2] += 0.1f;
			//}
   //   

			// Normalize pixel with weight sum
			float weightSum = (*pixels)(x, y).weightSum;
			if (weightSum != 0.0f) 
      {
				float invWt = 1.0f / weightSum;
        //invWt = 1.0f;// / MaxWeight;
				rgb[3*offset  ] = Clamp(rgb[3*offset  ] * invWt, 0.0f, INFINITY);
				rgb[3*offset+1] = Clamp(rgb[3*offset+1] * invWt, 0.0f, INFINITY);
				rgb[3*offset+2] = Clamp(rgb[3*offset+2] * invWt, 0.0f, INFINITY);

				//rgb[3*offset  ] = Clamp(weightSum, 0.0f, INFINITY);
				//rgb[3*offset+1] = Clamp(weightSum, 0.0f, INFINITY);
				//rgb[3*offset+2] = Clamp(weightSum, 0.0f, INFINITY);

				alpha[offset] = 1.0f;//Clamp(alpha[offset] * invWt, 0.0f, 1.0f);
			}

			// Compute premultiplied alpha color
			if (premultiplyAlpha) 
      {
				rgb[3*offset  ] *= alpha[offset];
				rgb[3*offset+1] *= alpha[offset];
				rgb[3*offset+2] *= alpha[offset];
			}




			++offset;
		}
	}

	// Write RGBA image
	WriteRGBAImage(filename+ ".exr", rgb, alpha, xPixelCount, yPixelCount, xResolution, yResolution, xPixelStart, yPixelStart);

	// Release temporary image memory
	delete[] alpha;
	delete[] rgb;
}





extern "C" DLLEXPORT Film *CreateFilm(const ParamSet &params)
{
	string filename = params.FindOneString("filename", "pbrt.exr");
	bool premultiplyAlpha = params.FindOneBool("premultiplyalpha", true);

	int xres = params.FindOneInt("xresolution", 400);
	int yres = params.FindOneInt("yresolution", 400);
	float crop[4] = { 0, 1, 0, 1 };
	int cwi;
	const float *cr = params.FindFloat("cropwindow", &cwi);

	if (cr && cwi == 4) 
  {
		crop[0] = Clamp(min(cr[0], cr[1]), 0., 1.);
		crop[1] = Clamp(max(cr[0], cr[1]), 0., 1.);
		crop[2] = Clamp(min(cr[2], cr[3]), 0., 1.);
		crop[3] = Clamp(max(cr[2], cr[3]), 0., 1.);
	}

	int writeFrequency = params.FindOneInt("writefrequency", -1);

	return new SamoaImageFilm(xres, yres, crop, filename, premultiplyAlpha, writeFrequency);
}
