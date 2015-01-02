#ifndef __mdkType_h
#define __mdkType_h

#include <typeinfo>

#include "mdkOSPlatformConfig.h"
#include "mdkDebugConfig.h"

// Attention:
// must include mdkCommonType.h before mdkString.h
#include "mdkCommonType.h"
#include "mdkString.h"

namespace mdk
{

template<typename ScalarType>
inline ScalarTypeEnum GetScalarType(const ScalarType& Scalar);

template<typename ScalarType>
inline String GetScalarTypeName(const ScalarType& Scalar);

template<typename ScalarType>
inline int_max GetByteNumberOfScalar(const ScalarType& Scalar);
//----------------------------------------------------------------//
// DenseMatrix<double> => DenseMatirx
template<typename TemplateClassType>
String GetTemplateClassName(const TemplateClassType& TemplateClassObject);

}//end namespace mdk

#include "mdkType.hpp"

#endif
