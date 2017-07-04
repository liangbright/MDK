#pragma once

namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh()
: Mesh()
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const MDK_Symbol_PureEmpty&)
: Mesh(MDK_PureEmpty)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const PolygonMesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(PolygonMesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const Mesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(Mesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::~PolygonMesh()
{
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const PolygonMesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
PolygonMesh<ScalarType> PolygonMesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
	PolygonMesh<ScalarType> OutputMesh = this->Mesh::GetSubMeshByFace(FaceIndexList);
	return OutputMesh;
}

}// namespace mdk
