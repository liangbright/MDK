#ifndef mdk_DenseImage3DSequence_h
#define mdk_DenseImage3DSequence_h

#include "mdkImageInterpolation3DSequence.h"
#include "mdkDenseImage3D.h"

namespace mdk
{
// 3D Image Time Sequence
template<typename Pixel_Type>
class DenseImage3DSequence : public Object
{
public:
	typedef	Pixel_Type PixelType;
	typedef Option_Of_Image3DSequenceInterpolation               InterpolationOptionType;
	typedef MethodEnum_Of_Image3DSequenceInterpolation           InterpolationMethodEnum;
	typedef BoundaryOptionEnum_Of_Image3DSequenceInterpolation   InterpolationBoundaryOptionEnum;

private:
	ObjectArray<DenseImage3D<PixelType>> m_ImageData;
	DenseVector<double> m_TimePointList;

public:			
	DenseImage3DSequence();
	DenseImage3DSequence(const DenseImage3DSequence& Input);
	DenseImage3DSequence(const DenseImage3DSequence&& Input);
	~DenseImage3DSequence();

	void operator=(const DenseImage3DSequence& Input);
	void operator=(const DenseImage3DSequence&& Input);

	//------------------------------------------------------------------------------------------//
	void Clear();

	//------------------------------ Initialization -------------------------------------------//
	void SetTimePointList(DenseVector<double> TPList);
	DenseVector<double> GetTimePointList() const;

	void SetFileOf3DImageAtTimeIndex(int_max tIndex, const String& InputPath);
	void Load3DImageAtTimeIndex(int_max tIndex);
	void LoadALLImageData();

	//-------------------------------- Ref Image ----------------------------------------------//
	DenseImage3D<PixelType>& Image3DAtTimeIndex(int_max tIndex);
	const DenseImage3D<PixelType>& Image3DAtTimeIndex(int_max tIndex) const;
	
	//-------------------------------- Pixel Operator ----------------------------------------//
	PixelType& operator(int_max LinearIndex, int_max tIndex);
	const PixelType& operator(int_max LinearIndex, int_max tIndex) const;

	PixelType& operator(int_max xIndex, int_max yIndex, int_max zIndex, int_max tIndex);
	const PixelType& operator(int_max xIndex, int_max yIndex, int_max zIndex, int_max tIndex) const;

	//-------------------------------- Get Pixel (no interpolation) -----------------------------//
	PixelType GetPixelAt3DIndexAndTimeIndex(int_max xIndex, int_max yIndex, int_max zIndex, int_max tIndex) const;

	//-------------------------------- Get Pixel (interpolation) --------------------------------//
	template<typename ScalarType>
	PixelType GetPixelAt3DIndexAndTimeIndex(ScalarType xIndex, ScalarType yIndex, ScalarType zIndex, int_max tIndex, 
		                                    const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DPositionAndTimeIndex(ScalarType x, ScalarType y, ScalarType z, int_max tIndex,
		                                       const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DPositionAndTime(ScalarType x, ScalarType y, ScalarType z, ScalarType t,
		                                  const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DWorldPositionAndTimeIndex(ScalarType x, ScalarType y, ScalarType z, int_max tIndex,
		                                            const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

	template<typename ScalarType>
	PixelType GetPixelAt3DWorldPositionAndTime(ScalarType x, ScalarType y, ScalarType z, ScalarType t,
		                                       const InterpolationOptionType& Option, bool EnableBoundCheck = true) const;

};

}//end namespace mdk

#include "mdkDenseImage3DSequence.hpp"

#endif