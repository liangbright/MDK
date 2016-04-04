#pragma once

#include <atomic>

#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename ScalarType>
struct ShapeDictionaryData
{
    std::string Name; // name of the dictionary

    ObjectArray<DenseMatrix<ScalarType>> Basis;
	//Basis[k] is the basis-k, a shape in 3D or 2D

	// row vector
    DenseMatrix<int_max> BasisID;

    std::atomic<int_max> SeedForNewBasisIDGeneration;

    //----------- Current "Dictionary Time " --------------------

    int_max CurrentDictionaryTime; // Measured By Total Number Of Samples In Training History;

    //------------ Age of each basis ----------------------------
    // row vector
	//Total Number Of Samples In Training History
    DenseMatrix<ScalarType> BasisAge;

    // ---------- basis  Experience on Representing Data ----
    // row vector    
    // the weighted number of training data samples
    // for each basis from day 0
	DenseMatrix<ScalarType> BasisExperience;

    //---------- Similarity and Redundancy between basis vector -----

    DenseMatrix<ScalarType> BasisSimilarity;
	//BasisSimilarity(i,j) is similarity between basis-i and basis-j

	ScalarType SimilarityThreshold;	//For BasisRedundancy Calculation;

    DenseMatrix<ScalarType> BasisRedundancy;
    // BasisRedundancy[j] =  the number of the other bases near the basis j ( Similarity(i, j) >= SimilarityThreshold) 
    // range [0, BasisCount-1]
	// matrix rank can also measure redundancy
};


template<typename Scalar_Type>
class ShapeDictionary : Object
{
public:
	typedef Scalar_Type ScalarType;

private:
    std::shared_ptr<ShapeDictionaryData<ScalarType>> m_Data;

public:
    ShapeDictionary();
    ShapeDictionary(const ShapeDictionary& InputDictionary);
    ShapeDictionary(ShapeDictionary&& InputDictionary);
    ~ShapeDictionary();

    void operator=(const ShapeDictionary& InputDictionary);
    void operator=(ShapeDictionary&& InputDictionary);

    void Copy(const ShapeDictionary& InputDictionary);
	void Copy(ShapeDictionary&& InputDictionary);

    void Share(ShapeDictionary& InputDictionary);
    void ForceShare(const ShapeDictionary& InputDictionary);

    void Clear();

    bool Load(const std::string& FilePathAndName);
	bool Save(const std::string& FilePathAndName) const;

	void Initialize(ObjectArray<DenseMatrix<ScalarType>> BasisData);

    ShapeDictionary<ScalarType> GetSubDictionary(const DenseMatrix<int_max>& SelectedBasisIndexList) const;

    void MergeDictionary(const ShapeDictionary<ScalarType>& InputDictionary);

    // -------------- get/set ---------------------------------------------------//

	inline std::string& Name() { return m_Data->Name; }
	inline const std::string& Name() const { return m_Data->Name; }

	inline int_max GenerateNewBasisID();
	inline int_max GetCurrentSeedForNewBasisIDGeneration() const { return m_Data->SeedForNewBasisIDGeneration.load(); }
	inline void SetCurrentSeedForNewBasisIDGeneration(int_max Seed) { m_Data->SeedForNewBasisIDGeneration = Seed; }

	inline void SetCurrentDictionaryTime(int_max DictionaryTime) {m_Data->CurrentDictionaryTime = DictionaryTime;}
	inline int_max GetCurrentDictionaryTime() const {return m_Data->CurrentDictionaryTime = DictionaryTime;	}

	inline int_max GetBasisCount() const { return  m_Data->Basis.GetLength(); }
	inline ObjectArray<DenseMatrix<ScalarType>>& Basis() { return m_Data->Basis; }
    inline const  ObjectArray<DenseMatrix<ScalarType>>& Basis() const { return m_Data->Basis; }

    inline DenseMatrix<int_max>& BasisID() { return m_Data->BasisID; }
    inline const DenseMatrix<int_max>& BasisID() const { return m_Data->BasisID; }

    inline DenseMatrix<ScalarType>& BasisAge() { return m_Data->BasisAge; }
    inline const DenseMatrix<ScalarType>& BasisAge() const { return m_Data->BasisAge; }

    inline DenseMatrix<ScalarType>& BasisExperience() { return m_Data->BasisExperience; }
    inline const DenseMatrix<ScalarType>& BasisExperience() const { return m_Data->BasisExperience; }

    inline DenseMatrix<ScalarType>& BasisRedundancy() { return m_Data->BasisRedundancy; }
    inline const DenseMatrix<ScalarType>& BasisRedundancy() const { return m_Data->BasisRedundancy; }

    inline DenseMatrix<ScalarType>& BasisSimilarity() { return m_Data->BasisSimilarity; }
    inline const DenseMatrix<ScalarType>& BasisSimilarity() const { return m_Data->BasisSimilarity; }

    inline void SetSimilarityThreshold(ScalarType SimilarityThreshold) { m_Data->SimilarityThreshold = SimilarityThreshold; }
	inline ScalarType GetSimilarityThreshold() const { return m_Data->SimilarityThreshold; }	
};


}// namespace mdk

#include "mdkShapeDictionary.hpp"
