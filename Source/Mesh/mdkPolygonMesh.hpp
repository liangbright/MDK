#ifndef mdk_PolygonMesh_hpp
#define mdk_PolygonMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh()
: MembraneMesh()
{
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(const PolygonMesh<MeshAttributeType>& InputMesh)
: MembraneMesh(InputMesh)
{
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(PolygonMesh<MeshAttributeType>&& InputMesh)
: MembraneMesh(std::move(InputMesh))
{
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::~PolygonMesh()
{
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::operator=(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    this->MembraneMesh::operator=(InputMesh);
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::operator=(PolygonMesh<MeshAttributeType>&& InputMesh)
{
    this->MembraneMesh::operator=(std::move(InputMesh));
}


//------------ Construct from input data ------------------------------------//

template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::Construct(DenseMatrix<ScalarType> InputPointPositionMatrix, const ObjectArray<DenseVector<int_max>>& InputFaceTable)
{
	if (InputPointPositionMatrix.IsEmpty() == true || InputFaceTable.IsEmpty() == true)
    {
        MDK_Error("InputPointPositionMatrix or InputFaceTable is empty @ PolygonMesh::Construct(...)")
        return;
    }

	if (InputPointPositionMatrix.GetRowCount() != 3 || 3 * InputFaceTable.GetElementCount() < InputPointPositionMatrix.GetColCount())
    {
        MDK_Error("InputPointPositionMatrix or InputFaceTable is invalid @ PolygonMesh::Construct(...)")
        return;
    }
    //--------------------------------------------------------------------------------------------------
	this->Clear();

	auto PointHandleList = this->AddPoint_batch(std::move(InputPointPositionMatrix));

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
			MDK_Error("Somthing is wrong with PointHandleList @ PolygonMesh::Construct(...)")
            return;
        }
    }

    for (int_max k = 0; k < InputFaceTable.GetLength(); ++k)
    {        
        auto PointHandleList_k = PointHandleList.GetSubSet(InputFaceTable[k]);
        this->AddFaceByPoint(PointHandleList_k);
    }
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::Construct(MembraneMesh<MeshAttributeType> InputMembraneMesh)
{
    auto InputMeshPtr = static_cast<PolygonMesh<MeshAttributeType>*>(&InputMembraneMesh);
    m_MeshData = std::move(InputMeshPtr->m_MeshData);
}


template<typename MeshAttributeType>
inline
std::pair<DenseMatrix<typename MeshAttributeType::ScalarType>, ObjectArray<DenseVector<int_max>>>
PolygonMesh<MeshAttributeType>::GetPointPositionMatrixAndFaceTable() const
{
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Output;
    this->GetPointPositionMatrixAndFaceTable(Output.first, Output.second);
    return Output;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPositionMatrixAndFaceTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable, ObjectArray<DenseVector<int_max>>& FaceTable) const
{
    auto PointCount = this->GetPointCount();
    auto FaceCount = this->GetFaceCount();

    PointPositionTable.FastResize(3, PointCount);    
    FaceTable.FastResize(FaceCount);

    // Map PointIndex (PointHandle.GetIndex()) to OutputIndex (col index) in PointPositionTable
    std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex; 

    int_max PointCounter = 0;

    for( auto it = this->GetIteratorOfPoint(); it.IsNotEnd(); ++it)
    {
        auto PointHandle = it.GetPointHandle();
        it.Point().GetPosition(PointPositionTable.GetPointerOfCol(PointCounter));
        Map_PointIndex_to_OutputIndex[PointHandle.GetIndex()] = PointCounter;
        PointCounter += 1;
    }

    int_max FaceCounter = 0;

    for(auto it = this->GetIteratorOfFace(); it.IsNotEnd(); ++it)
    {
        auto PointHandleList = it.Face().GetPointHandleList();
        FaceTable[FaceCounter].FastResize(PointHandleList.GetLength());
        for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
        {
            auto tempPointIndex = PointHandleList[k].GetIndex();   
            auto it_map = Map_PointIndex_to_OutputIndex.find(tempPointIndex);
            if (it_map != Map_PointIndex_to_OutputIndex.end())
            {
                FaceTable[FaceCounter][k] = it_map->second;
            }
            else
            {
                MDK_Error("tempPointIndex is invalid @ PolygonMesh::GetPointPositionTableAndFaceTable(...)")
                return;
            }
        }
        FaceCounter += 1;
    }
}

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetSubMeshByFace(const DenseVector<FaceHandleType>& FaceHandleList) const
{
    PolygonMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->MembraneMesh::GetSubMeshByFace(FaceHandleList));
    return OutputMesh;
}


template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
	if (this->IsEmpty() == true)
	{
		return false;
	}

	for (auto it = this->GetIteratorOfFace(); it.IsNotEnd(); ++it)
	{
		auto FaceHandle = it.GetFaceHandle();
		auto PointCount = this->Face(FaceHandle).GetPointCount();
		if (PointCount != 3)
		{
			return false;
		}
	}

	return true;
}

}// namespace mdk

#endif