#pragma once


//2D

//---------------Filter for 3D Scalar DenseImage--------------------------------------//
#include "mdkScalarDenseImageAverageFilter3D.h"
#include "mdkIntegralImageBasedImageAverageFilter3D.h"
#include "mdkScalarDenseImageGaussianFilter3D.h"
#include "mdkScalarDenseImageIsotropicLaplacianOfGaussianFilter3D.h"
#include "mdkScalarDenseImageGradientFilter3D.h"
#include "mdkScalarDenseImageGradientMagnitudeFilter3D.h"
// pooling
#include "mdkScalarDenseImageMaxPoolingFilter3D.h"
#include "mdkScalarDenseImageMaxAbsPoolingFilter3D.h"
#include "mdkScalarDenseImageMinPoolingFilter3D.h"
#include "mdkScalarDenseImageMinAbsPoolingFilter3D.h"
// resample
#include "mdkDenseImageResampler3D.h"
#include "mdkDenseImageResamplerWithCoordinateTransform3D.h"
//Integral Image
#include "mdkIntegralImageBuilder3D.h"
//---------------------------------------------------------------------------//

//#include "mdkScalarrDenseImageToVectorDenseImageConvolutionFilter3D.h"

//------------------ Function -----------------------------------------------//
#include "mdkImageInterpolation3D.h"
#include "mdkDenseImageResampler3D.h"
#include "mdkHistogram.h"



//------------------------ FileIO ---------------------------------------------//
//#include "mdkDenseImage2D_FileIO.h"
#include "mdkDenseImage3D_FileIO.h"