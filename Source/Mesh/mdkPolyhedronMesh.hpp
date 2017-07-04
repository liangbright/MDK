#pragma once

namespace mdk
{

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh()
: Mesh()
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh(const MDK_Symbol_PureEmpty&)
: Mesh(MDK_PureEmpty)
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh(const PolyhedronMesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh(PolyhedronMesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh(const Mesh<ScalarType>& InputMesh)
: Mesh(InputMesh)
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::PolyhedronMesh(Mesh<ScalarType>&& InputMesh)
: Mesh(std::move(InputMesh))
{
}

template<typename ScalarType>
PolyhedronMesh<ScalarType>::~PolyhedronMesh()
{
}

template<typename ScalarType>
inline
void PolyhedronMesh<ScalarType>::operator=(const PolyhedronMesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolyhedronMesh<ScalarType>::operator=(PolyhedronMesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void PolyhedronMesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
	this->Mesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void PolyhedronMesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->Mesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
PolygonMesh<ScalarType> PolyhedronMesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
	PolygonMesh<ScalarType> OutputMesh = this->Mesh::GetSubMeshByFace(FaceIndexList);
	return OutputMesh;
}

template<typename ScalarType>
PolyhedronMesh<ScalarType> PolyhedronMesh<ScalarType>::GetSubMeshByCell(const DenseVector<int_max>& CellIndexList) const
{
	PolyhedronMesh<ScalarType> OutputMesh = this->Mesh::GetSubMeshByCell(CellIndexList);
	return OutputMesh;
}

}// namespace mdk
