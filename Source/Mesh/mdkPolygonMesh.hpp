#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


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
: MembraneMesh(std::forward<MembraneMesh<MeshAttributeType>&&>(InputMesh))
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
    this->MembraneMesh::operator=(std::forward<MembraneMesh<MeshAttributeType>&&>(InputMesh));
}


//------------ Construct from input data ------------------------------------//

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputPointPositionMatrix, 
                                               const ObjectArray<DenseVector<int_max>>& InputCellTable)
{
	if (InputPointPositionMatrix.IsEmpty() == true || InputCellTable.IsEmpty() == true)
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is empty @ PolygonMesh::Construct(...)")
        return false;
    }

	if (InputPointPositionMatrix.GetRowNumber() != 3 || 3 * InputCellTable.GetElementNumber() < InputPointPositionMatrix.GetColNumber())
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }
    //--------------------------------------------------------------------------------------------------

	auto PointHandleList = this->AddPointSet(InputPointPositionMatrix);

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
			MDK_Error("Somthing is wrong with PointHandleList @ PolygonMesh::Construct(...)")
            return false;
        }
    }

    for (int_max k = 0; k < InputCellTable.GetLength(); ++k)
    {        
        auto PointHandleList_k = PointHandleList.GetSubSet(InputCellTable[k]);
        this->AddCellByPoint(PointHandleList_k);
    }

    return true;
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
PolygonMesh<MeshAttributeType>::GetPointPositionMatrixAndCellTable() const
{
	std::pair<DenseMatrix<ScalarType>, ObjectArray<DenseVector<int_max>>> Output;
    this->GetPointPositionMatrixAndCellTable(Output.first, Output.second);
    return Output;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPositionMatrixAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable,
                                   ObjectArray<DenseVector<int_max>>& CellTable) const
{
    auto PointNumber = this->GetPointNumber();
    auto CellNumber = this->GetCellNumber();

    PointPositionTable.FastResize(3, PointNumber);    
    CellTable.FastResize(CellNumber);

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

    int_max CellCounter = 0;

    for(auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        auto PointHandleList = it.Cell().GetPointHandleList();
        CellTable[CellCounter].FastResize(PointHandleList.GetLength());
        for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
        {
            auto tempPointIndex = PointHandleList[k].GetIndex();   
            auto it_map = Map_PointIndex_to_OutputIndex.find(tempPointIndex);
            if (it_map != Map_PointIndex_to_OutputIndex.end())
            {
                CellTable[CellCounter][k] = it_map->second;
            }
            else
            {
                MDK_Error("tempPointIndex is invalid @ PolygonMesh::GetPointPositionTableAndCellTable(...)")
                return;
            }
        }
        CellCounter += 1;
    }
}

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const
{
    PolygonMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->MembraneMesh::GetSubMeshByCell(CellHandleList));
    return OutputMesh;
}

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> PolygonMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const
{
    PolygonMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->MembraneMesh::GetSubMeshByCell(CellIDList));
    return OutputMesh;
}

}// namespace mdk

#endif