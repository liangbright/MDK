#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh()
: SurfaceMesh()
{
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(const PolygonMesh<MeshAttributeType>& InputMesh)
: SurfaceMesh(InputMesh)
{
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType>::PolygonMesh(PolygonMesh<MeshAttributeType>&& InputMesh)
: SurfaceMesh(std::forward<PolygonMesh<MeshAttributeType>&&>(InputMesh))
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
    this->SurfaceMesh::operator=(InputMesh);
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::operator=(PolygonMesh<MeshAttributeType>&& InputMesh)
{
    this->SurfaceMesh::operator=(std::forward<PolygonMesh<MeshAttributeType>&&>(InputMesh));
}


//------------ Construct from input data ------------------------------------//

template<typename MeshAttributeType>
bool PolygonMesh<MeshAttributeType>::Construct(const DenseMatrix<typename MeshAttributeType::ScalarType>& InputPointPositionTable, 
                                               const DataArray<DenseVector<int_max>>& InputCellTable)
{
    if (InputPointPositionTable.IsEmpty() == true || InputCellTable.IsEmpty() == true)
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is empty @ PolygonMesh::Construct(...)")
        return false;
    }

    if (InputPointPositionTable.GetRowNumber() != 3 || 3 * InputCellTable.GetElementNumber() < InputPointPositionTable.GetColNumber())
    {
        MDK_Error("InputPointPositionTable or InputPointIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }
    //--------------------------------------------------------------------------------------------------

    auto PointHandleList = this->AddPoint(InputPointPositionTable);

    for (int_max k = 0; k < PointHandleList.GetLength(); ++k)
    {
        if (this->IsValidHandle(PointHandleList[k]) == false)
        {
            return false;
        }
    }

    DenseVector<EdgeHandleType> EdgeHandleList;

    for (int_max k = 0; k < InputCellTable.GetLength(); ++k)
    {
        const DenseVector<int_max>& Cell_k = InputCellTable[k];
        
        EdgeHandleList.FastResize(Cell_k.GetElementNumber());

        for (int_max n = 0; n < Cell_k.GetElementNumber(); ++n)
        {
            auto tempIndex_a = Cell_k[n];

            int_max tempIndex_b = -1;
            if (n < Cell_k.GetElementNumber() - 1)
            {
                tempIndex_b = Cell_k[n + 1];
            }
            else
            {
                tempIndex_b = Cell_k[0];
            }

            EdgeHandleList[n] = this->AddEdge(PointHandleList[tempIndex_a], PointHandleList[tempIndex_b]);
        }

        for (int_max k = 0; k < EdgeHandleList.GetLength(); ++k)
        {
            if (this->IsValidHandle(EdgeHandleList[k]) == false)
            {
                return false;
            }
        }

        this->AddCell(EdgeHandleList);
    }

    return true;
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::Construct(SurfaceMesh<MeshAttributeType> InputSurfaceMesh)
{
    (*this) = std::move(InputSurfaceMesh);
}


template<typename MeshAttributeType>
inline
std::pair<DenseMatrix<typename MeshAttributeType::ScalarType>, DataArray<DenseVector<int_max>>>
PolygonMesh<MeshAttributeType>::GetPointPositionMatrixAndCellTable() const
{
    std::pair<DenseMatrix<ScalarType>, DataArray<DenseVector<int_max>>> Output;
    this->GetPointPositionMatrixAndCellTable(Output.first, Output.second);
    return Output;
}


template<typename MeshAttributeType>
inline
void PolygonMesh<MeshAttributeType>::
GetPointPositionMatrixAndCellTable(DenseMatrix<typename MeshAttributeType::ScalarType>& PointPositionTable,
                                   DataArray<DenseVector<int_max>>& CellTable) const
{
    auto PointNumber = this->GetPointNumber();
    auto CellNumber = this->GetCellNumber();

    PointPositionTable.FastResize(3, PointNumber);    
    CellTable.FastResize(CellNumber);

    std::unordered_map<int_max, int_max> Map_PointIndex_to_OutputIndex;

    int_max PointCounter = 0;

    auto it_v = this->GetIteratorOfPoint();
    it_v.SetToBegin();
    while (it_v.IsInRange())
    {
        auto PointHandle = it_v.GetPointHandle();
        this->Point(PointHandle).GetPosition(PointPositionTable.GetPointerOfCol(PointCounter));
        Map_PointIndex_to_OutputIndex[PointHandle.GetIndex()] = PointCounter;
        PointCounter += 1;
        ++it_v;
    }

    int_max CellCounter = 0;

    auto it = this->GetIteratorOfCell();
    it.SetToBegin();
    while (it.IsInRange())
    {
        auto CellHandle = it.GetCellHandle();
        auto PointHandleList = this->Cell(CellHandle).GetPointHandleList();
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
        ++it;
    }
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtPoint(PointHandleType PointHandle)
{
}


template<typename MeshAttributeType>
void PolygonMesh<MeshAttributeType>::UpdateNormalAtCell(CellHandleType CellHandle)
{
}


}// namespace mdk

#endif