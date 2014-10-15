#ifndef __mdkDenseImage2D_h
#define __mdkDenseImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkImageInterpolation2D.h"
#include "mdkImageInfo.h" // must be the last to include

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D DenseImage Class
// Pixel is Scalar or vector
// Lx: number of Pixel in x-direction
// Ly: number of Pixel in y-direction


//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_2DDenseImage_Operator_CheckBound
#endif
//------------------------------------------------------

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

template<typename ScalarType = double>
struct Image2DBoxRegionOf2DPhysicalPosition
{
    bool IsEmpty;

	ScalarType x0;
	ScalarType y0;

	ScalarType x1;
	ScalarType y1;

//-------------------------------------

    Image2DBoxRegionOf2DPhysicalPosition()
    {
        IsEmpty = true;
        x0 = 0;
        y0 = 0;
        x1 = 0;
        y1 = 0;
    };

	ScalarType Lx() const
    {
        return x1 - x0;
    }

	ScalarType Ly() const
    {
        return y1 - y0;
    }
};


struct Image2DInfo
{
	DenseVector<int_max, 2> Size;       // {Lx, Ly} number of Pixels in each direction
	DenseVector<double, 2>  Spacing;    // Pixel Spacing of DICOM DenseImage in world coordinate system {Sx, Sy} (unit: mm)
	DenseVector<double, 2>  Origin;     // Origin of DICOM DenseImage in world coordinate system (x,y) (unit: mm)
	DenseMatrix<double> Orientation;    // 2x2 Matrix

//-------------------------------------------
	Image2DInfo() { this->Clear(); }
	~Image2DInfo() {}

	Image2DInfo(const Image2DInfo& Info)
	{
		(*this) = Info;
	}

	void operator=(const Image2DInfo& Info)
	{
		Size = Info.Size;
		Spacing = Info.Spacing;
		Origin = Info.Origin;
		Orientation = Info.Orientation;
	}

	void Clear()
	{
		Size.Fill(0);
		Spacing.Fill(0);
		Origin.Fill(0);
		Orientation.Clear();
		Orientation.Resize(2, 2);
		Orientation.FixSize();
		Orientation.FillDiagonal(1.0);
	}
};

//===================================================================================================================//
//--------------------------------------------------- DenseImageData2D struct --------------------------------------------//

template<typename PixelType>
struct DenseImageData2D
{
	DenseVector<int_max, 2> m_Size;     // {Lx, Ly} number of Pixels in each direction

	DenseVector<double, 2> m_Spacing;   // Pixel Spacing of DICOM DenseImage in world coordinate system {Sx, Sy} (unit: mm)

	DenseVector<double, 2> m_Origin;    // Origin of DICOM DenseImage in world coordinate system (x, y) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 2x2 Matrix

    std::vector<PixelType> m_DataArray;

	PixelType m_Pixel_OutsideImage;
//----------------------------------------------------------------

    DenseImageData2D();

    ~DenseImageData2D();

    inline void Clear();

    //---------------------------------

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

    inline PixelType& operator()(int_max LinearIndex);

    inline const PixelType& operator()(int_max LinearIndex) const;
   
    inline PixelType& operator()(int_max xIndex, int_max yIndex);

    inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

    inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const;
	
	template<typename ScalarType_Position, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const;

private:
//deleted:
    DenseImageData2D(const DenseImageData2D&) = delete;
    DenseImageData2D(DenseImageData2D&&) = delete;
    void operator=(const DenseImageData2D&) = delete;
    void operator=(DenseImageData2D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class DenseImage2D : public Object
{
public:
	typedef Pixel_Type PixelType;

	typedef Option_Of_Image2DInterpolation<PixelType>    InterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation   InterpolationBoundaryOptionEnum;

protected:
    std::shared_ptr<DenseImageData2D<PixelType>> m_ImageData;

public:
    DenseImage2D();

    DenseImage2D(const DenseImage2D& InputImage);

    DenseImage2D(DenseImage2D&& InputImage);

    ~DenseImage2D();

    void operator=(const DenseImage2D& InputImage);

    void operator=(DenseImage2D&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an DenseImage from double (Type_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const DenseImage2D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	bool Copy(const DenseImage2D<PixelType_Input>* InputImage);

    inline bool Fill(const PixelType& Pixel);

    //---------------------------------------------------------//
    bool Share(DenseImage2D& InputImage);
    bool Share(DenseImage2D* InputImage);

	void ForceShare(const DenseImage2D& InputImage);
	bool ForceShare(const DenseImage2D* InputImage);

    //---------------------------------------------------------//
    void Take(DenseImage2D&& InputImage);
    void Take(DenseImage2D& InputImage);
    bool Take(DenseImage2D* InputImage);

    //-----------------------------------------------------------------//

    inline bool IsEmpty() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

	inline Image2DInfo GetInfo() const;

	inline DenseVector<int_max, 3> GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly, int_max& Lz) const;

	inline bool SetSize(const DenseVector<int_max, 3>& Size);

    inline bool SetSize(int_max Lx, int_max Ly, int_max Lz);

	inline DenseVector<double, 3> GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y, double& Spacing_z) const;

	inline void SetSpacing(const DenseVector<double, 3>& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y, double Spacing_z);

	inline DenseVector<double, 3> GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y, double& Origin_z) const;

	inline void SetOrigin(const DenseVector<double, 3>& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y, double Origin_z);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

	inline DenseVector<double, 3> GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y, double& PhysicalSize_z) const;

    inline int_max GetPixelNumber() const;

	template<typename PixelType_Input>
	bool CopyPixelData(const PixelType_Input* InputPixelPointer, int_max InputPixelNumber);

    //--------------------------- Get Pixel Pointer ------------------------------//

    inline PixelType* GetPixelPointer();
    inline const PixelType* GetPixelPointer() const;

	inline PixelType* begin();
	inline const PixelType* begin() const;

	inline PixelType* end();
	inline const PixelType* end() const;

    //------------------------ LinearIndex, 2DIndex and 2DPhyscialPosition ------------------------------------------------------------------------//
	// 2DIndex can be continuous or discrete

    inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex, int_max zIndex) const;

	inline int_max Transform2DIndexToLinearIndex(const DenseVector<int_max, 3>& Index2D) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 3> TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex, ScalarType_Index zIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 3> Transform2DIndexTo2DPhysicalPosition(const DenseVector<ScalarType_Index, 3>& Index2D) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 3> Transform2DPhysicalPositionTo2DIndex(const DenseVector<ScalarType, 3>& Position) const;

	//--------------------------- Get/Set Pixel ------------------------------//

    inline PixelType& operator[](int_max LinearIndex);

    inline const PixelType& operator[](int_max LinearIndex) const;

	inline PixelType& operator()(int_max LinearIndex);

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex);

	inline const PixelType& operator()(int_max xIndex, int_max yIndex, int_max zIndex) const;

	//------------------ get nearest pixel --------------------//

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(const DenseVector<ScalarType, 3>& Index2D) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPhysicalPosition(const DenseVector<ScalarType, 3>& Position) const;

	//------------------ get Pixel by using interpolation method --------------------//

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, const InterpolationOptionType& Option) const;

	const PixelType& GetPixelAt2DIndex(int_max xIndex, int_max yIndex, int_max zIndex, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(int xIndex, int yIndex, int zIndex, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(long xIndex, long yIndex, long zIndex, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(const DenseVector<ScalarType, 3>& Index2D, const InterpolationOptionType& Option) const;

	const PixelType& GetPixelAt2DIndex(const DenseVector<int_max, 3>& Index2D, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(const DenseVector<int, 3>& Index2D, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(const DenseVector<long, 3>& Index2D, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPhysicalPosition(ScalarType x, ScalarType y, ScalarType z, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPhysicalPosition(const DenseVector<ScalarType, 3>& Position, const InterpolationOptionType& Option) const;

	//------------------------- Get LinearIndex In Region -------------------//

    DenseMatrix<int_max> GetLinearIndexListOfRegion(int_max xIndex_s,     int_max Region_Lx,
	                                                int_max yIndex_s,     int_max Region_Ly) const;
    
    //-------------------------- Get SubImage -------------------------------//

    DenseImage2D GetSubImage(int_max xIndex_s, int_max xIndex_e, int_max yIndex_s, int_max yIndex_e) const;

    //-------------------------- Pad, UnPad -------------------------------//

    DenseImage2D  Pad(const std::string& Option, int_max Pad_Lx, int_max Pad_Ly) const;

    DenseImage2D  Pad(PixelType Pixel, int_max Pad_Lx, int_max Pad_Ly) const;

    DenseImage2D  UnPad(int_max Pad_Lx, int_max Pad_Ly) const;

    //-------------------------- Sum, Mean, Max, Min -------------------------------//

    PixelType Sum() const;

    PixelType Mean() const;

    PixelType Max() const;

    PixelType Min() const;

};


}//end namespace mdk

#include "mdkDenseImage2D.hpp"

#endif