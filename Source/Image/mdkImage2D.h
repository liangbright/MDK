#ifndef __mdkImage2D_h
#define __mdkImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>

#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D Image Class
// --------------------------------------------------------------------------------------------------------//

//----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_2DImage_Operator_CheckBound
#endif
//----------------------------------------------------

struct ImageSize2D
{
	int_max Lx;
	int_max Ly;
};

struct ImageSpacing2D
{
    double Sx;
    double Sy;
};

struct ImagePhysicalSize2D
{
    double Lx;
    double Ly;
};

struct ImageOrigin2D
{
    double x;
    double y;
};

// ImageOrientation is a 2x2 double DenseMatrix

struct Image2DBoxRegionOf2DIndex
{
    bool IsEmpty;

    int_max x0;
    int_max y0;

    int_max x1;
    int_max y1;
 
//-------------------------------------
    Image2DBoxRegionOf2DIndex()
    {
        IsEmpty = true;
        x0 = 0;
        y0 = 0;
        x1 = 0;
        y1 = 0;
    };

    int_max Lx() const
    {
        return x1 - x0 + 1;
    }

    int_max Ly() const
    {
        return y1 - y0 + 1;
    }
};


struct Image2DBoxRegionOf2DPhysicalPosition
{
    bool IsEmpty;

    double x0;
    double y0;

    double x1;
    double y1;

//-------------------------------------

    Image2DBoxRegionOf2DPhysicalPosition()
    {
        IsEmpty = true;
        x0 = 0;
        y0 = 0;
        x1 = 0;
        y1 = 0;
    };

    double Lx() const
    {
        return x1 - x0;
    }

    double Ly() const
    {
        return y1 - y0;
    }
};

//===================================================================================================================//
//--------------------------------------------------- ImageData2D struct --------------------------------------------//

template<typename PixelType>
struct ImageData2D
{
    int_max m_Size[2];                  // {Lx, Ly} number of Pixels in each direction

    double m_Spacing[2];                // i.e., Pixel Spacing in DICOM image (ITK, VTK)

    double m_Origin[2];                 // i.e., Origin in DICOM image (ITK, VTK) {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 2x2 Matrix

    std::vector<PixelType> m_DataArray;

    PixelType m_NaNPixel;
//-----------------------------------------------------------

    ImageData2D();

    ~ImageData2D();

    void Clear();

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

    inline PixelType& operator()(int_max LinearIndex);

    inline const PixelType& operator()(int_max LinearIndex) const;
   
    inline PixelType& operator()(int_max xIndex, int_max yIndex);

    inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

    inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

    inline void TransformLinearIndexTo2DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex) const;

    inline void TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex, double& x, double& y) const;

    inline void Transform2DIndexTo2DPhysicalPosition(int_max xIndex, int_max yIndex, double& x, double& y) const;

    inline void Transform2DPhysicalPositionTo2DIndex(double x, double y, double& xIndex, double& yIndex) const;

private:
//deleted:
    ImageData2D(const ImageData2D&) = delete;
    ImageData2D(ImageData2D&&) = delete;
    void operator=(const ImageData2D&) = delete;
    void operator=(ImageData2D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class Image2D: public Object
{
public:
	typedef Pixel_Type PixelType;

private:

    std::shared_ptr<ImageData2D<PixelType>> m_ImageData;

    PixelType* m_PixelPointer; // keep tracking m_ImageData->m_DataArray.data();
	
public:

    Image2D();

    Image2D(const Image2D& InputImage);

    Image2D(Image2D&& InputImage);

    ~Image2D();

    void operator=(const Image2D& InputImage);

    void operator=(Image2D&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (PixelType_Input) to float (PixelType), etc

    template<typename PixelType_Input>
    void Copy(const Image2D<PixelType_Input>& InputImage);

    template<typename PixelType_Input>
    bool Copy(const Image2D<PixelType_Input>* InputImage);

    inline bool Fill(const PixelType& Pixel);

    //---------------------------------------------------------//

    bool Share(Image2D& InputImage);

    bool Share(Image2D* InputImage);

    void ForceShare(const Image2D& InputImage);

    bool ForceShare(const Image2D* InputImage);

    //---------------------------------------------------------//

    void Take(Image2D&& InputImage);

    void Take(Image2D& InputImage);

    bool Take(Image2D* InputImage);

    //-----------------------------------------------------------------//

    inline bool IsEmpty() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

    inline ImageSize2D GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly) const;

    inline bool SetSize(const Image2DSize& Size);

    inline bool SetSize(int_max Lx, int_max Ly);

    inline ImageSpacing2D GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y) const;

    inline void SetSpacing(const Image2DSpacing& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y);

    inline ImageOrigin2D GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y) const;

    inline void SetOrigin(const Image2DOrigin& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

    inline ImagePhysicalSize2D GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const;

    inline int_max GetPixelNumber() const;

    template<typename PixelType_Input>
    bool CopyData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber);

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();

    inline const PixelType* GetPixelPointer() const;

    // ------------------------ Index and Position ------------------------------------------------------------------------//

    inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

    inline void TransformLinearIndexTo2DIndex(int_max LinearIndex, int_max& xIndex, int_max& yIndex) const;

    inline void TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex, double& x, double& y) const;

    inline void Transform2DIndexTo2DPhysicalPosition(int_max xIndex, int_max yIndex, double& x, double& y) const;

    inline void Transform2DPhysicalPositionTo2DIndex(double x, double y, double& xIndex, double& yIndex) const;

	//--------------------------- Get/Set Pixel      ------------------------------//

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

	inline PixelType& operator()(int_max LinearIndex);

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline PixelType& operator()(int_max xIndex, int_max yIndex);

	inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

	inline PixelType& at(int_max LinearIndex);

	inline const PixelType& at(int_max LinearIndex) const;

	inline PixelType& at(int_max xIndex, int_max yIndex);

	inline const PixelType& at(int_max xIndex, int_max yIndex) const;

	//-------------------------- Get SubImage -------------------------------//

    Image2D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const;

	//-------------------------- Pad, UnPad -------------------------------//

    Image2D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const;

    Image2D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const;

    Image2D  UnPad(int_max Pad_Lx, int_max Pad_Ly) const;

	//------------------------- Get LinearIndex In Region -------------------//

    DenseMatrix<int_max> GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
	                                                int_max yIndex_s,     int_max Region_Ly) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

};


}//end namespace mdk

#include "mdkImage2D.hpp"

#endif