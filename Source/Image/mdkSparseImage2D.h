#ifndef __mdkSparseImage2D_h
#define __mdkSparseImage2D_h

#include <vector>
#include <memory>
#include <cstdlib>
#include <unordered_map>

#include "mdkDebugConfig.h"
#include "mdkDenseMatrix.h"
#include "mdkDenseImage2D.h"
#include "mdkImageInterpolation2D.h"

namespace mdk
{
//-------------------------------------------------------------------------------------------------------//
// 2D Sparse Image Class
// used in SingleClassObjectDetectionInSingleImage2D
//-----------------------------------------------------
#if defined MDK_DEBUG_MODE
	#define MDK_DEBUG_2DSparseImage_Operator_CheckBound
#endif
//------------------------------------------------------

//===================================================================================================================//
//--------------------------------------------------- SparseImageData2D struct --------------------------------------------//

template<typename PixelType>
struct SparseImageData2D
{
	DenseVector<int_max, 2> m_Size;     // {Lx, Ly} number of Pixels in each direction

	DenseVector<double, 2> m_Spacing;   // Pixel Spacing of DICOM image in world coordinate system {Sx, Sy} (unit: mm)

	DenseVector<double, 2> m_Origin;    // Origin of DICOM image in world coordinate system (x, y) (unit: mm)

    DenseMatrix<double> m_Orientation;  // 2x2 Matrix

	std::unordered_map<int_max, PixelType> m_DataMap; // map LinearIndex to Pixel

	PixelType m_Pixel_OutsideImage;

//----------------------------------------------------------------

	SparseImageData2D();

	~SparseImageData2D();

    inline void Clear();

    //---------------------------------

	inline const PixelType& GetPixel(int_max LinearIndex) const;
   
	inline const PixelType& GetPixel(int_max xIndex, int_max yIndex) const;

	inline void SetPixel(int_max LinearIndex, const PixelType& Pixel);

	inline void SetPixel(int_max xIndex, int_max yIndex, const PixelType& Pixel);

	//---------------------------------

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
	SparseImageData2D(const SparseImageData2D&) = delete;
	SparseImageData2D(SparseImageData2D&&) = delete;
	void operator=(const SparseImageData2D&) = delete;
	void operator=(SparseImageData2D&&) = delete;
};

//===================================================================================================================//

template<typename Pixel_Type>
class SparseImage2D : public Object
{
public:
	typedef Pixel_Type PixelType;

	typedef Option_Of_Image2DInterpolation<PixelType>    InterpolationOptionType;
	typedef MethodEnum_Of_Image2DInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image2DInterpolation   InterpolationBoundaryOptionEnum;

protected:
    std::shared_ptr<SparseImageData2D<PixelType>> m_ImageData;

public:
    SparseImage2D();

    SparseImage2D(const SparseImage2D& InputImage);

    SparseImage2D(SparseImage2D&& InputImage);

    ~SparseImage2D();

    void operator=(const SparseImage2D& InputImage);

    void operator=(SparseImage2D&& InputImage);

    //-----------------------------------------------------------------//

    void Clear();

    //---------------------------------------------------------//

    // Copy can be used to convert an image from double (PixelType_Input) to float (PixelType), etc

	template<typename PixelType_Input>
	void Copy(const SparseImage2D<PixelType_Input>& InputImage);

	template<typename PixelType_Input>
	bool Copy(const SparseImage2D<PixelType_Input>* InputImage);

	template<typename PixelType_Input>
	void CopyPixelData(const SparseImage2D<PixelType_Input>& InputImage);

    //---------------------------------------------------------//
    bool Share(SparseImage2D& InputImage);
    bool Share(SparseImage2D* InputImage);

    void ForceShare(const SparseImage2D& InputImage);
    bool ForceShare(const SparseImage2D* InputImage);

    //---------------------------------------------------------//
    void Take(SparseImage2D&& InputImage);
    void Take(SparseImage2D& InputImage);
    bool Take(SparseImage2D* InputImage);

    //-----------------------------------------------------------------//

    inline bool IsEmpty() const;

    //--------------------------- Get/Set Info and Data ------------------------------//

	inline Image2DInfo GetInfo() const;

	inline DenseVector<int_max, 2> GetSize() const;

    inline void GetSize(int_max& Lx, int_max& Ly) const;

	inline bool SetSize(const DenseVector<int_max, 2>& Size);

    inline bool SetSize(int_max Lx, int_max Ly);

	inline DenseVector<double, 2> GetSpacing() const;

    inline void GetSpacing(double& Spacing_x, double& Spacing_y) const;

	inline void SetSpacing(const DenseVector<double, 2>& Spacing);

    inline void SetSpacing(double Spacing_x, double Spacing_y);

	inline DenseVector<double, 2> GetOrigin() const;

    inline void GetOrigin(double& Origin_x, double& Origin_y) const;

	inline void SetOrigin(const DenseVector<double, 2>& Origin);

    inline void SetOrigin(double Origin_x, double Origin_y);

    inline const DenseMatrix<double>& GetOrientation() const;

    inline void SetOrientation(const DenseMatrix<double>& Orientation);

	inline DenseVector<double, 2> GetPhysicalSize() const;

    inline void GetPhysicalSize(double& PhysicalSize_x, double& PhysicalSize_y) const;

    inline int_max GetRecordedPixelNumber() const;

    //------------------------ LinearIndex, 2DIndex and 2DPhyscialPosition ------------------------------------------------------------------------//
	// 2DIndex can be continuous or discrete

    inline int_max Transform2DIndexToLinearIndex(int_max xIndex, int_max yIndex) const;

	inline int_max Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const;

	template<typename ScalarType = int_max>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DIndex(int_max LinearIndex) const;
	
	template<typename ScalarType = double>
	inline DenseVector<ScalarType, 2> TransformLinearIndexTo2DPhysicalPosition(int_max LinearIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPhysicalPosition(ScalarType_Index xIndex, ScalarType_Index yIndex) const;

	template<typename ScalarType_Position = double, typename ScalarType_Index>
	inline DenseVector<ScalarType_Position, 2> Transform2DIndexTo2DPhysicalPosition(const DenseVector<ScalarType_Index, 2>& Index2D) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPhysicalPositionTo2DIndex(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline DenseVector<ScalarType, 2> Transform2DPhysicalPositionTo2DIndex(const DenseVector<ScalarType, 2>& Position) const;

	//--------------------------- Get Pixel ------------------------------//

	inline const PixelType& operator[](int_max LinearIndex) const;

	inline const PixelType& operator()(int_max LinearIndex) const;

	inline const PixelType& operator()(int_max xIndex, int_max yIndex) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(ScalarType xIndex, ScalarType yIndex) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DIndex(const DenseVector<ScalarType, 2>& Index2D) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPhysicalPosition(ScalarType x, ScalarType y) const;

	template<typename ScalarType>
	inline const PixelType& GetPixelNearestTo2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position) const;

	//------------------ get Pixel by using interpolation method --------------------//

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(ScalarType xIndex, ScalarType yIndex, const InterpolationOptionType& Option) const;

	const PixelType& GetPixelAt2DIndex(int_max xIndex, int_max yIndex, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(int xIndex, int yIndex, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(long xIndex, long yIndex, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DIndex(const DenseVector<ScalarType, 2>& Index2D, const InterpolationOptionType& Option) const;

	const PixelType& GetPixelAt2DIndex(const DenseVector<int_max, 2>& Index2D, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(const DenseVector<int, 2>& Index2D, const InterpolationOptionType& Option) const;
	const PixelType& GetPixelAt2DIndex(const DenseVector<long, 2>& Index2D, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPhysicalPosition(ScalarType x, ScalarType y, const InterpolationOptionType& Option) const;

	template<typename OutputPixelType = PixelType, typename ScalarType>
	OutputPixelType GetPixelAt2DPhysicalPosition(const DenseVector<ScalarType, 2>& Position, const InterpolationOptionType& Option) const;

	//--------------------------- Set Pixel ------------------------------//

	inline bool SetPixelAtLinearIndex(int_max LinearIndex, const PixelType& Pixel);

	inline bool SetPixelAt2DIndex(int_max xIndex, int_max yIndex, const PixelType& Pixel);

};


}//end namespace mdk

#include "mdkSparseImage2D.hpp"

#endif