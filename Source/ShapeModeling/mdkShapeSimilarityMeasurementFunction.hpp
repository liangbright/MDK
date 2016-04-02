#pragma once

namespace mdk
{

template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB, const std::string& TransformName, bool Flag_Symmetry)
{
	bool Flag_3D = false;
	bool Flag_2D = flase;
	if (ShapeA.GetRowCount == 2 && ShapeB.GetRowCount() == 2)
	{
		Flag_2D = true;
	}
	else if (ShapeA.GetRowCount == 3 && ShapeB.GetRowCount() == 3)
	{
		Flag_3D = true;
	}
	else
	{
		MDK_Error("input shape is wong @ ShapeSimilarityMeasurementFunction::ComputeSimilarityBetweenShapeWithPointCorrespondence(...)")
		return 0;
	}

	if (ShapeA.GetColCount() < 3 || ShapeB.GetColCount() < 3)
	{
		MDK_Error("input shape has less than 3 point @ ShapeSimilarityMeasurementFunction::ComputeSimilarityBetweenShapeWithPointCorrespondence(...)")
		return 0;
	}

	if (ShapeA.GetColCount() != ShapeB.GetColCount())
	{
		MDK_Error("NO PointCorrespondence @ ShapeSimilarityMeasurementFunction::ComputeSimilarityBetweenShapeWithPointCorrespondence(...)")
		return 0;
	}

	if (TransformName == "SimilarityTransform")
	{
		if (Flag_Symmetry == true)
		{
			if (Flag_2D == true)
			{
				auto Similarity_AB = ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity_BA = ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity = (Similarity_AB + Similarity_BA) / ScalarType(2);
				return Similarity;
			}

			if (Flag_3D == true)
			{
				auto Similarity_AB = ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity_BA = ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity = (Similarity_AB + Similarity_BA) / ScalarType(2);
				return Similarity;
			}
		}
		else
		{
			if (Flag_2D == true)
			{
				return ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
			}

			if (Flag_3D == true)
			{
				return ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(ShapeA, ShapeB);
			}
		}	
	}
	else if (TransformName == "RigidTransform")
	{
		if (Flag_Symmetry == true)
		{
			if (Flag_2D == true)
			{
				auto Similarity_AB = ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity_BA = ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity = (Similarity_AB + Similarity_BA) / ScalarType(2);
				return Similarity;
			}

			if (Flag_3D == true)
			{
				auto Similarity_AB = ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity_BA = ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
				auto Similarity = (Similarity_AB + Similarity_BA) / ScalarType(2);
				return Similarity;			
			}
		}
		else
		{
			if (Flag_2D == true)
			{
				return ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
			}

			if (Flag_3D == true)
			{
				return ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(ShapeA, ShapeB);
			}
		}
	}
	else
	{
		MDK_Error("TransformName is Unknown @ ShapeSimilarityMeasurementFunction::ComputeSimilarityBetweenShapeWithPointCorrespondence(...)")
		return 0;
	}
}


template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	//-------- function to center and normalize shape -----------------------//
	auto TempFunction_NormalizeShape = [](const DenseMatrix<ScalarType>& Shape)
	{
		DenseMatrix<ScalarType> Shape_new = Shape;
		auto Center = Shape.MeanOfEachRow();
		auto PointCount = Shape.GetColCount();
		ScalarType MeanDistance = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape_new(0, k);
			auto& y = Shape_new(1, k);
			x -= Center[0];
			y -= Center[1];
			MeanDistance += std::sqrt(x*x + y*y);
		}
		MeanDistance /= ScalarType(PointCount);

		for (int_max k = 0; k < 2 * PointCount; ++k)
		{
			Shape_new[k] /= MeanDistance;
		}
		return Shape_new;
	};
	//------------------------------------------------------
	// center and normalized A, B 
	auto ShapeA_new = = TempFunction_NormalizeShape(ShapeA);
	auto ShapeB_new = TempFunction_NormalizeShape(ShapeB);	
	// transfrom B to A
	SimilarityTransform2D<ScalarType> Transform;
	Transform.SetSourceLandmarkPointSet(&ShapeB_new);
	Transform.SetTargetLandmarkPointSet(&ShapeA_new);
	Transform.EstimateParameter();
	ShapeB_new = Transform.TransformPoint(ShapeB_new);
	//calculate mean distance error
	ScalarType MDE = 0;
	auto PointCount = ShapeA_new.GetColCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto xa = ShapeA_new(0, k);
		auto ya = ShapeA_new(1, k);
		auto xb = ShapeB_new(0, k);
		auto yb = ShapeB_new(1, k);
		MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb);
	}
	MDE /= ScalarType(PointCount);
	// calculate similarity 
	auto Similarity = std::exp(-MDE*MDE / ScalarType(0.2));
	return Similarity;
}


template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence2D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	//-------- function to center shape --------------------------------//
	auto TempFunction_CenterShape = [](DenseMatrix<ScalarType>& Shape)
	{
		ScalarType MeanRadius = 0;
		auto Center = Shape.MeanOfEachRow();
		auto PointCount = Shape.GetColCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape(0, k);
			auto& y = Shape(1, k);
			auto& z = Shape(2, k);
			x -= Center[0];
			y -= Center[1];
			z -= Center[2];
			MeanRadius += std::sqrt(x*x + y*y + z*z);
		}
		MeanRadius /= ScalarType(PointCount);
		return MeanRadius;
	};

	//----------------------------------------------------------------
	auto PointCount = ShapeA.GetColCount();
	// center A and calculate mean radius
	auto MeanRadiusA = TempFunction_CenterShape(ShapeA);
	// transfrom B to A
	RigidTransform2D<ScalarType> Transform;
	Transform.SetSourceLandmarkPointSet(&ShapeB);
	Transform.SetTargetLandmarkPointSet(&ShapeA_new);
	Transform.EstimateParameter();
	auto ShapeB_new = Transform.TransformPoint(ShapeB);
	//calculate mean radius of Shape B
	ScalarType MeanRadiusB = 0;
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto x = ShapeB_new(0, k);
		auto y = ShapeB_new(1, k);
		auto z = ShapeB_new(2, k);
		MeanRadiusB += std::sqrt(x*x + y*y + z*z);
	}
	MeanRadiusB /= ScalarType(PointCount);

	auto MeanRadius = (MeanRadiusA + MeanRadiusB) / ScalarType(2);

	// calculate mean distance error 
	ScalarType MDE = 0;
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto xa = ShapeA_new(0, k);
		auto ya = ShapeA_new(1, k);
		auto za = ShapeA_new(2, k);
		auto xb = ShapeB_new(0, k);
		auto yb = ShapeB_new(1, k);
		auto zb = ShapeB_new(2, k);
		MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb) + (za - zb)*(za - zb));
	}
	MDE /= ScalarType(PointCount);
	MDE /= MeanRadius;

	// calculate similarity 
	auto Similarity = std::exp(-MDE*MDE / ScalarType(0.2));
	return Similarity;
}


template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_SimilarityTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	//-------- function to center and normalize shape -----------------------//
	auto TempFunction_NormalizeShape = [](const DenseMatrix<ScalarType>& Shape)
	{
		DenseMatrix<ScalarType> Shape_new = Shape;
		auto Center = Shape.MeanOfEachRow();
		auto PointCount = Shape.GetColCount();
		ScalarType MeanDistance = 0;
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape_new(0, k);
			auto& y = Shape_new(1, k);
			auto& z = Shape_new(2, k);
			x -= Center[0];
			y -= Center[1];
			z -= Center[2];
			MeanDistance += std::sqrt(x*x + y*y + z*z);
		}
		MeanDistance /= ScalarType(PointCount);

		for (int_max k = 0; k < 3 * PointCount; ++k)
		{
			Shape_new[k] /= MeanDistance;
		}
		return Shape_new;
	};
	
	//------------------------------------------------
	auto ShapeA_new = TempFunction_NormalizeShape(ShapeA);
	auto ShapeB_new = TempFunction_NormalizeShape(ShapeB);	
	// transfrom B to A
	SimilarityTransform3D<ScalarType> Transform;
	Transform.SetSourceLandmarkPointSet(&ShapeB_new);
	Transform.SetTargetLandmarkPointSet(&ShapeA_new);
	Transform.EstimateParameter();
	ShapeB_new = Transform.TransformPoint(ShapeB_new);
	//calculate mean distance error
	ScalarType MDE = 0;
	auto PointCount = ShapeA_new.GetColCount();
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto xa = ShapeA_new(0, k);
		auto ya = ShapeA_new(1, k);
		auto za = ShapeA_new(2, k);
		auto xb = ShapeB_new(0, k);
		auto yb = ShapeB_new(1, k);
		auto zb = ShapeB_new(2, k);
		MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb) + (za - zb)*(za - zb));
	}
	MDE /= ScalarType(PointCount);
	// calculate similarity 
	auto Similarity = std::exp(-MDE*MDE / ScalarType(0.2));
	return Similarity;
}


template<typename ScalarType>
ScalarType ComputeSimilarityBetweenShapeWithPointCorrespondence3D_RigidTransform_NonSymmetry(const DenseMatrix<ScalarType>& ShapeA, const DenseMatrix<ScalarType>& ShapeB)
{
	//-------- function to center shape --------------------------------//
	auto TempFunction_CenterShape = [](DenseMatrix<ScalarType>& Shape)
	{		
		ScalarType MeanRadius = 0;
		auto Center = Shape.MeanOfEachRow();
		auto PointCount = Shape.GetColCount();
		for (int_max k = 0; k < PointCount; ++k)
		{
			auto& x = Shape(0, k);
			auto& y = Shape(1, k);
			auto& z = Shape(2, k);
			x -= Center[0];
			y -= Center[1];
			z -= Center[2];
			MeanRadius += std::sqrt(x*x + y*y + z*z);
		}
		MeanRadius /= ScalarType(PointCount);
		return MeanRadius;
	};

	//----------------------------------------------------------------
	auto PointCount = ShapeA.GetColCount();
	// center A and calculate mean radius
	auto MeanRadiusA = TempFunction_CenterShape(ShapeA);
	// transfrom B to A
	RigidTransform3D<ScalarType> Transform;
	Transform.SetSourceLandmarkPointSet(&ShapeB);
	Transform.SetTargetLandmarkPointSet(&ShapeA_new);
	Transform.EstimateParameter();
	auto ShapeB_new = Transform.TransformPoint(ShapeB);
	//calculate mean radius of Shape B
	ScalarType MeanRadiusB = 0;	
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto x = ShapeB_new(0, k);
		auto y = ShapeB_new(1, k);
		auto z = ShapeB_new(2, k);
		MeanRadiusB += std::sqrt(x*x + y*y + z*z);
	}
	MeanRadiusB /= ScalarType(PointCount);

	auto MeanRadius =(MeanRadiusA + MeanRadiusB)/ScalarType(2);

	// calculate mean distance error 
	ScalarType MDE = 0;
	for (int_max k = 0; k < PointCount; ++k)
	{
		auto xa = ShapeA_new(0, k);
		auto ya = ShapeA_new(1, k);
		auto za = ShapeA_new(2, k);
		auto xb = ShapeB_new(0, k);
		auto yb = ShapeB_new(1, k);
		auto zb = ShapeB_new(2, k);
		MDE += std::sqrt((xa - xb)*(xa - xb) + (ya - yb)*(ya - yb) + (za - zb)*(za - zb));
	}
	MDE /= ScalarType(PointCount);
	MDE /= MeanRadius;

	// calculate similarity 
	auto Similarity = std::exp(-MDE*MDE / ScalarType(0.2));
	return Similarity;
}


template<typename ScalarType>
DenseMatrix<ScalarType> ComputeSimilarityBetweenShapeWithPointCorrespondence(const ObjectArray<DenseMatrix<ScalarType>>& ShapeList, const std::string& TransformName, bool Flag_Symmetry, int_max MaxThreadCount)
{
	DenseMatrix<ScalarType> SimilarityMatrix;
	int_max ShapeCount = ShapeList.GetLength();
	if (ShapeCount == 0)
	{
		return SimilarityTable;
	}

	SimilarityMatrix.Resize(ShapeCount, ShapeCount);

	//for (int_max n = 0; n <= ShapeCount-2; ++n)
	auto TempFunction = [&](int_max n)
	{
		SimilarityTable[n].Resize(ShapeCount);
		for (int_max m = n + 1; m < ShapeCount; ++m)
		{
			auto Similarity = ComputeSimilarityBetweenShapeWithPointCorrespondence(ShapeList[n], ShapeList[m], TransformName, Flag_Symmetry);
			SimilarityMatrix(n, m) = Similarity;
			SimilarityMatrix(m, n) = Similarity;
		}
	};
	ParallelForLoop(TempFunction, 0, ShapeCount - 2, MaxThreadCount);
	
	for (int_max n = 0; n < ShapeCount; ++n)
	{
		SimilarityMatrix(n, n) = 1;
	}
	return SimilarityMatrix;
}

}//namespace mdk