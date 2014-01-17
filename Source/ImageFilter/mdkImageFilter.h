#ifndef __mdkImageFilter_h
#define __mdkImageFilter_h

#include <vector>

#include "mdkAlgorithmObject.h"
#include "mdkImage.h"
#include "mdkDataArrayTemplate.h"
#include "mdkImageFilterMask.h"

namespace mdk
{

class mdkImageFilter : public mdkAlgorithmObject
{
private:
	//-------------- input ----------------------

	// inputport 0: 
	mdkImage*   m_InputImage;
	
	//port 1:
	mdkMatrix*  m_InterestRegion;  // compute values at the points in the region
	
	//port 2:
	mdkMatrix*  m_InterestPositionList;  // compute values at the points in the list
    
	//port 3:
	mdkImageFilterMask* m_InputMask;

	//---------------output ---------------------

	// outputport 0:
	mdkImage* m_OutputImage;

	// outputport 1:
	mdkMatrix* m_OutputData;

	// ------------- internal data -----------------

	int m_ThreadNumber;

public:		
	mdkImageFilter();
	~mdkImageFilter();
  
	void Run();

	void Apply(int NumberOfThread, std::vector<mdkObject>& InputList, std::vector<mdkObject>& OutputList);

	void Apply(int NumberOfThread, mdkImage* InputImage, mdkImage* OutputImage);

	void Apply(int NumberOfThread, mdkImage* InputImage, mdkMatrix* OutputMatrix);

protected:
	void SetInputImageData(mdkImage* Input);

	void SetInputImageRegion(double* Input);

	void SetInputImageRegion(int x, int y, int z, int Lx, int Ly, int Lz);

	void SetInputMask(mdkImageFilterMask* Input);

	void SetOutputImageData(mdkImage* Output);

private:
	void Run_OutputImage();
	void Run_OutputMatrix();

	void RunFilter_OutputImage(double& Region);

private:
	mdkImageFilter(const mdkImageFilter&);        // Not implemented.
	void operator=(const mdkImageFilter&);  // Not implemented.
};
}//end namespace mdk

#endif