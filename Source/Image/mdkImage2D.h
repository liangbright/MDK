#ifndef __mdkImage2D_h
#define __mdkImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>


#include "mdkImageConfig.h"
#include "mdkDenseMatrix.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D Image Class
// --------------------------------------------------------------------------------------------------------//

struct Image2DSize
{
	int_max Lx;
	int_max Ly;
};

struct Image2DPixelSpacing
{
    double Sx;
    double Sy;
};

struct Image2DPhysicalSize
{
    double Lx;
    double Ly;
};

struct Image2DPhysicalOrigin
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
//--------------------------------------------------- Image2DData struct --------------------------------------------//

template<typename PixelType>
struct Image2DData
{
    int_max m_Size[2];                  // {Lx, Ly} number of Pixels in each direction

    double m_PixelSpacing[2];           // i.e., Spacing in DICOM image (ITK, VTK)

    double m_PhysicalOrigin[2];         // i.e., Origin in DICOM image (ITK, VTK) {x0, y0, z0} in world coordinate system (x,y,z) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 2x2 Matrix

    std::vector<PixelType> m_DataArray;

    PixelType m_NaNPixel;
//-----------------------------------------------------------

    Image2DData();

    ~Image2DData();

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

    inline void Transform2DPhysicalPositionToContinuous2DIndex(double x, double y, double& xIndex, double& yIndex) const;

private:
//deleted:
    Image2DData(const Image2DData&) = delete;
    Image2DData(Image2DData&&) = delete;
    void operator=(const Image2DData&) = delete;
    void operator=(Image2DData&&) = delete;
};

//===================================================================================================================//

template<typename PixelType>
class Image2D: public Object
{
private:

    std::shared_ptr<Image2DData<PixelType>> m_ImageData;

    PixelType* m_PixelPointer; // keep tracking m_ImageData->m_DataArray.data();
	
protected:

    Image2D();

    Image2D(Image2D&& InputImage);

    ~Image2D();

    void operator=(Image2D&& InputImage);

public:

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

    inline Image2DSize GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly) const;

    inline bool SetSize(const Image2DSize& Size);

    inline bool SetSize(int_max Lx, int_max Ly);

    inline Image2DPixelSpacing GetPixelSpacing() const;

    inline void GetPixelSpacing(double& Spacing_x, double& Spacing_y) const;

    inline void SetPixelSpacing(const Image2DPixelSpacing& Spacing);

    inline void SetPixelSpacing(double Spacing_x, double Spacing_y);

    inline Image2DPhysicalOrigin GetPhysicalOrigin() const;

    inline void GetPhysicalOrigin(double& Origin_x, double& Origin_y) const;

    inline void SetPhysicalOrigin(const Image2DPhysicalOrigin& Origin);

    inline void SetPhysicalOrigin(double Origin_x, double Origin_y);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

    inline Image2DPhysicalSize GetPhysicalSize() const;

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

    inline void Transform2DPhysicalPositionToContinuous2DIndex(double x, double y, double& xIndex, double& yIndex) const;

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

private:
    Image2D(const Image2D& InputImage) = delete;
    void operator=(const Image2D& InputImage) = delete;

};


}//end namespace mdk

#include "mdkImage2D.hpp"

#endif