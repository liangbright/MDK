#pragma once

namespace mdk
{
	//---------------------- Get zero pixel ---------------------------------//
template<typename PixelType>
inline PixelType  GetZeroPixel() { return PixelType(0); }
//specialization
template<>
inline double  GetZeroPixel() { return 0; }

template<>
inline float   GetZeroPixel() { return 0; }

template<>
inline int8    GetZeroPixel() { return 0; }

template<>
inline int16   GetZeroPixel() { return 0; }

template<>
inline int32   GetZeroPixel() { return 0; }

template<>
inline int64   GetZeroPixel() { return 0; }

template<>
inline uint8   GetZeroPixel() { return 0; }

template<>
inline uint16  GetZeroPixel() { return 0; }

template<>
inline uint32  GetZeroPixel() { return 0; }

template<>
inline uint64  GetZeroPixel() { return 0; }

template<typename ScalarType, int_max Length>
inline DenseVector<ScalarType, Length>  GetZeroPixel() { DenseVector<ScalarType, Length> ZeroPixel = 0; return ZeroPixel; }

}//namespace mdk
