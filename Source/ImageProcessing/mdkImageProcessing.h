#pragma once


//2D

//---------------Filter for 3D DenseImage--------------------------------------//
#include "mdkDiscreteConvolutionDenseImageFilter3D.h"
#include "mdkGenericConvolutionDenseImageFilter3D.h"
#include "mdkGradientDenseImageFilter3D.h"
#include "mdkIntegralImageBasedAverageDenseImageFilter3D.h"
#include "mdkPoolingDenseImageFilter3D.h"
// resample
#include "mdkDenseImageResampler3D.h"
//Integral Image
#include "mdkIntegralImageBuilder3D.h"

//------------------ Function -----------------------------------------------//
#include "mdkImageInterpolation3D.h"
#include "mdkDenseImageResampler3D.h"
#include "mdkHistogram.h"

//------------------------ FileIO ---------------------------------------------//
//#include "mdkDenseImage2D_FileIO.h"
#include "mdkDenseImage3D_FileIO.h"