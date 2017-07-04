#pragma once

namespace mdk
{

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh()
: PolyhedronMesh()
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(const MDK_Symbol_PureEmpty&)
: PolyhedronMesh(MDK_PureEmpty)
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(const TetrahedronMesh<ScalarType>& InputMesh)
: PolyhedronMesh(InputMesh)
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(TetrahedronMesh<ScalarType>&& InputMesh)
: PolyhedronMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(const PolyhedronMesh<ScalarType>& InputMesh)
: PolyhedronMesh(InputMesh)
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(PolyhedronMesh<ScalarType>&& InputMesh)
: PolyhedronMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(const Mesh<ScalarType>& InputMesh)
: PolyhedronMesh(InputMesh)
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::TetrahedronMesh(Mesh<ScalarType>&& InputMesh)
: PolyhedronMesh(std::move(InputMesh))
{
}

template<typename ScalarType>
TetrahedronMesh<ScalarType>::~TetrahedronMesh()
{
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(const TetrahedronMesh<ScalarType>& InputMesh)
{
	this->PolyhedronMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(TetrahedronMesh<ScalarType>&& InputMesh)
{
	this->PolyhedronMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(const PolyhedronMesh<ScalarType>& InputMesh)
{
	this->PolyhedronMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(PolyhedronMesh<ScalarType>&& InputMesh)
{
	this->PolyhedronMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(const Mesh<ScalarType>& InputMesh)
{
	this->PolyhedronMesh::operator=(InputMesh);
}

template<typename ScalarType>
inline
void TetrahedronMesh<ScalarType>::operator=(Mesh<ScalarType>&& InputMesh)
{
	this->PolyhedronMesh::operator=(std::move(InputMesh));
}

template<typename ScalarType>
TriangleMesh<ScalarType> TetrahedronMesh<ScalarType>::GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const
{
	TriangleMesh<ScalarType> OutputMesh = this->PolyhedronMesh::GetSubMeshByFace(FaceIndexList);
	return OutputMesh;
}

template<typename ScalarType>
TetrahedronMesh<ScalarType> TetrahedronMesh<ScalarType>::GetSubMeshByCell(const DenseVector<int_max>& CellIndexList) const
{
	TetrahedronMesh<ScalarType> OutputMesh = this->PolyhedronMesh::GetSubMeshByCell(CellIndexList);
	return OutputMesh;
}

}// namespace mdk
