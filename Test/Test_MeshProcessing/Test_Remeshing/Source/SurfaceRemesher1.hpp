#pragma once

namespace mdk
{
template<typename ScalarType>
SurfaceRemesher1<ScalarType>::SurfaceRemesher1()
{
	this->Clear();
}


template<typename ScalarType>
SurfaceRemesher1<ScalarType>::~SurfaceRemesher1()
{
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::Clear()
{
	m_InputMesh.Clear();
	this->ClearInternalData();
	m_OutputMesh.Clear();
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::ClearInternalData()
{
	m_MiddlePointOfEdgeOfInputMesh.Clear();
	m_QuadCandidateList.Clear();
	m_QuadCandidateScoreList.Clear();
	m_QuadCandidateFlagList.Clear();
	m_QuadCandiateIndexList_on_each_triangle.Clear();
}

template<typename ScalarType>
bool SurfaceRemesher1<ScalarType>::CheckInput()
{
	if (m_InputMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("m_InputMesh DataStructure is NOT Clean @ SurfaceRemesher1::CheckInput()")
		return false;
	}

	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		if (it.Point().GetAdjacentPointCount() >= 8)
		{
			MDK_Error("AdjacentPointCount >= 8 @ SurfaceRemesher1::CheckInput()")
			return false;
		}
	}
	return true;
}


template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return;
	}
	this->ClearInternalData();
	this->FindMiddlePointOfEdgeOfInputMesh();
	this->GenerateQuadCandidate();
	this->EvaluateQuadCandidate();
	this->SelectQuadCandidate();
	this->BuildOutputMesh();
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::FindMiddlePointOfEdgeOfInputMesh()
{// add middle point of each edge
	m_MiddlePointOfEdgeOfInputMesh.Clear();
	m_MiddlePointOfEdgeOfInputMesh.SetCapacity(m_InputMesh.GetEdgeCount());

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto PointHandleList = it.Edge().GetPointHandleList();
		auto Point0 = m_InputMesh.GetPointPosition(PointHandleList[0]);
		auto Point1 = m_InputMesh.GetPointPosition(PointHandleList[1]);
		auto PointM = (Point0 + Point1) / ScalarType(2);
		m_MiddlePointOfEdgeOfInputMesh.Append(m_InputMesh.AddPoint(PointM));
	}
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::GenerateQuadCandidate()
{
	m_QuadCandidateList.Clear();
	m_QuadCandidateList.SetCapacity(m_InputMesh.GetEdgeCount());

	m_QuadCandiateIndexList_on_each_triangle.Clear();
	m_QuadCandiateIndexList_on_each_triangle.Resize(m_InputMesh.GetFaceCount());
	for (int_max k = 0; k < m_QuadCandiateIndexList_on_each_triangle.GetLength(); ++k)
	{
		m_QuadCandiateIndexList_on_each_triangle.SetCapacity(3);
	}

	for (auto it = m_InputMesh.GetIteratorOfEdge(); it.IsNotEnd(); ++it)
	{
		auto AdjacentFaceHandleList = it.Edge().GetAdjacentFaceHandleList();
		for (int_max k = 0; k < AdjacentFaceHandleList.GetLength(); ++k)
		{
			//----------------------------	
			// P3: middle point of edge e3
			// P4: middle point of edge e4
			//
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2          
			//----------------------------

			EdgeHandleType EdgeH0, EdgeH1, EdgeH2, EdgeH3, EdgeH4;
			PointHandleType PointH0, PointH1, PointH2, PointH3, PointH4;

			EdgeH0 = it.GetEdgeHandle();

			auto FaceH_middle = AdjacentFaceHandleList[k];
			{
				auto tempPointHandleList = m_InputMesh.Face(FaceH_middle).GetPointHandleList();
				auto tempEdgeHandleList = m_InputMesh.Face(FaceH_middle).GetEdgeHandleList();
				if (EdgeH0 == tempEdgeHandleList[0])
				{
					PointH0 = tempPointHandleList[2];
					PointH1 = tempPointHandleList[0];
					PointH2 = tempPointHandleList[1];

					EdgeH1 = tempEdgeHandleList[1];
					EdgeH2 = tempEdgeHandleList[2];
				}
				else if (EdgeH0 == tempEdgeHandleList[1])
				{
					PointH0 = tempPointHandleList[0];
					PointH1 = tempPointHandleList[1];
					PointH2 = tempPointHandleList[2];

					EdgeH1 = tempEdgeHandleList[2];
					EdgeH2 = tempEdgeHandleList[0];
				}
				else//(EdgeH0 == tempEdgeHandleList[2])
				{
					PointH0 = tempPointHandleList[1];
					PointH1 = tempPointHandleList[2];
					PointH2 = tempPointHandleList[0];

					EdgeH1 = tempEdgeHandleList[0];
					EdgeH2 = tempEdgeHandleList[1];
				}
			}

			FaceHandleType FaceH_left;
			{
				auto tempAdjacentFaceHandleList = m_InputMesh.Edge(EdgeH2).GetAdjacentFaceHandleList();
				if (tempAdjacentFaceHandleList[0] != FaceH_middle)
				{
					FaceH_left = tempAdjacentFaceHandleList[0];
				}
				else
				{
					FaceH_left = tempAdjacentFaceHandleList[1];
				}
			}

			FaceHandleType FaceH_right;
			{
				auto tempAdjacentFaceHandleList = m_InputMesh.Edge(EdgeH1).GetAdjacentFaceHandleList();
				if (tempAdjacentFaceHandleList[0] != FaceH_middle)
				{
					FaceH_right = tempAdjacentFaceHandleList[0];
				}
				else
				{
					FaceH_right = tempAdjacentFaceHandleList[1];
				}
			}

			//get EdgeH3 and EdgeH4
			{// Known: P0, P1, e1, e2
				auto tempEdgeHandleList = m_InputMesh.Point(PointH0).GetAdjacentEdgeHandleList();
				for (int_max m = 0; m < tempEdgeHandleList.GetLength(); ++m)
				{
					bool Flag_e3 = false;
					bool Flag_e4 = false;
					auto tempFaceHandleList = m_InputMesh.Edge(tempEdgeHandleList[m]).GetAdjacentFaceHandleList();
					for (int_max n = 0; n < tempFaceHandleList.GetLength(); ++n)
					{
						if (tempFaceHandleList[n] == FaceH_left)
						{
							if (tempFaceHandleList[n] != EdgeH2)
							{
								EdgeH4 = tempEdgeHandleList[m];
								Flag_e4 = true;
							}
						}
						else if (tempFaceHandleList[n] == FaceH_right)
						{
							if (tempFaceHandleList[n] != EdgeH1)
							{
								EdgeH3 = tempEdgeHandleList[m];
								Flag_e3 = true;
							}
						}
					}

					if (Flag_e3 == true && Flag_e4 == true)
					{
						break;
					}
				}
			}

			//Get PointH3, PointH4
			PointH3 = m_MiddlePointOfEdgeOfInputMesh[EdgeH3.GetIndex()];
			PointH4 = m_MiddlePointOfEdgeOfInputMesh[EdgeH4.GetIndex()];

			//----------------------------	
			//       P4          P3   
			//   ----e4--- P0 ---e3------
			//    \   |  /    \   |   /
			//     \  | /e2  e1\  |  /
			//      \ |/        \ |/
			//       P1 ---e0----P2          
			//----------------------------

			if (m_InputMesh.Point(PointH0).GetAdjacentPointCount() == 6)
			{ //add quad {P1, P2, P3, P4}
				DenseVector<PointHandleType, 4> PointHandleList_quad = { PointH1, PointH2, PointH3, PointH4 };
				m_QuadCandidateList.Append(PointHandleList_quad);
				m_QuadCandiateIndexList_on_each_triangle[FaceH_middle.GetIndex()].Append(m_QuadCandidateList.GetLength() - 1);
			}
			else
			{ // add quad {P1, P2, P3, P0} and quad {P1, P2, P0, P4}
				DenseVector<PointHandleType, 4> PointHandleList_quad_a = { PointH1, PointH2, PointH3, PointH0 };
				DenseVector<PointHandleType, 4> PointHandleList_quad_b = { PointH1, PointH2, PointH0, PointH4 };
				m_QuadCandidateList.Append(PointHandleList_quad_a);
				m_QuadCandiateIndexList_on_each_triangle[FaceH_middle.GetIndex()].Append(m_QuadCandidateList.GetLength() - 1);
				m_QuadCandidateList.Append(PointHandleList_quad_b);
				m_QuadCandiateIndexList_on_each_triangle[FaceH_middle.GetIndex()].Append(m_QuadCandidateList.GetLength() - 1);
			}
		}
	}
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::EvaluateQuadCandidate()
{
	m_QuadCandidateScoreList.Clear();
	m_QuadCandidateScoreList.Resize(m_QuadCandidateList.GetLength());
	for (int_max k = 0; k < m_QuadCandidateList.GetLength(); ++k)
	{
		auto Point0 = m_InputMesh.GetPointPosition(m_QuadCandidateList[k][0]);
		auto Point1 = m_InputMesh.GetPointPosition(m_QuadCandidateList[k][1]);
		auto Point2 = m_InputMesh.GetPointPosition(m_QuadCandidateList[k][2]);
		auto Point3 = m_InputMesh.GetPointPosition(m_QuadCandidateList[k][3]);
		m_QuadCandidateScoreList[k] = this->EvaluateQuad(Point0, Point1, Point2, Point3);
	}
}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::SelectQuadCandidate()
{// call Gurobi 

}

template<typename ScalarType>
void SurfaceRemesher1<ScalarType>::BuildOutputMesh()
{
	m_OutputMesh.Clear();
	m_OutputMesh.SetCapacity(m_InputMesh.GetPointCount(), m_InputMesh.GetEdgeCount(), m_InputMesh.GetFaceCount());
	for (auto it = m_InputMesh.GetIteratorOfPoint(); it.IsNotEnd(); ++it)
	{
		auto PointHandle = m_OutputMesh.AddPoint(it.Point().GetPosition());
		m_OutputMesh.Point(PointHandle).SetID(it.Point().GetID());
	}

	for (int_max k = 0; k < m_QuadCandidateFlagList.GetLength(); ++k)
	{
		if (m_QuadCandidateFlagList[k] == 1)
		{
			m_OutputMesh.AddFaceByPoint(m_QuadCandidateList[k]);
		}
	}
}

template<typename ScalarType>
ScalarType SurfaceRemesher1<ScalarType>::EvaluateQuad(const DenseVector<ScalarType, 3>& Point0, const DenseVector<ScalarType, 3>& Point1, const DenseVector<ScalarType, 3>& Point2, const DenseVector<ScalarType, 3>& Point3)
{
	//================
	//  P3 -  P2 
	//  |     |   
	//  P0 -  P1 
	//================

	auto EPS = std::numeric_limits<ScalarType>::epsilon();

	//warp angle
	ScalarType WarpAngleScore = 0;
	{
		auto Normal_023 = ComputeTriangleNormalIn3D(Point0, Point2, Point3);
		auto Normal_012 = ComputeTriangleNormalIn3D(Point0, Point1, Point2);
		auto CosAngle_023_012 = std::abs(ComputeVectorDotProductIn3D(Normal_012, Normal_023));

		auto Normal_013 = ComputeTriangleNormalIn3D(Point0, Point1, Point3);
		auto Normal_123 = ComputeTriangleNormalIn3D(Point1, Point2, Point3);
		auto CosAngle_013_123 = std::abs(ComputeVectorDotProductIn3D(Normal_013, Normal_123));

		WarpAngleScore = std::min(CosAngle_023_012, CosAngle_013_123);
	}

	auto P0P1 = Point1 - Point0;
	auto P1P2 = Point2 - Point1;
	auto P2P3 = Point3 - Point2;
	auto P3P0 = Point0 - Point3;
	auto P0P2 = Point2 - Point0;
	auto P1P3 = Point3 - Point1;
	auto Direction01 = P0P1 / (P0P1.L2Norm() + EPS);
	auto Direction12 = P1P2 / (P1P2.L2Norm() + EPS);
	auto Direction23 = P2P3 / (P2P3.L2Norm() + EPS);
	auto Direction30 = P3P0 / (P3P0.L2Norm() + EPS);

	ScalarType EdgeAngleScore = 0;
	DenseVector<ScalarType, 4> CosEdgeAngleList;
	CosEdgeAngleList[0] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction30));
	CosEdgeAngleList[1] = std::abs(ComputeVectorDotProductIn3D(Direction01, Direction12));
	CosEdgeAngleList[2] = std::abs(ComputeVectorDotProductIn3D(Direction12, Direction23));
	CosEdgeAngleList[3] = std::abs(ComputeVectorDotProductIn3D(Direction23, Direction30));
	EdgeAngleScore = ScalarType(1) - CosEdgeAngleList.Max();

	ScalarType Aspect = 0;
	DenseVector<ScalarType, 6> DistanceList;
	DistanceList[0] = P0P1.L2Norm();
	DistanceList[1] = P1P2.L2Norm();
	DistanceList[2] = P2P3.L2Norm();
	DistanceList[3] = P3P0.L2Norm();
	DistanceList[4] = P0P2.L2Norm() / ScalarType(1.414);
	DistanceList[5] = P1P3.L2Norm() / ScalarType(1.414);
	auto Distance_min = DistanceList.Min();
	auto Distance_max = DistanceList.Max();
	Aspect = Distance_min / (Distance_max + EPS);

	auto Score = WarpAngleScore + ScalarType(2)*EdgeAngleScore + Aspect;

	return Score;
}

}//namespace