#pragma once


namespace mdk
{

template<typename ScalarType>
ShapeDictionary<ScalarType>::ShapeDictionary()
{
    m_Data = std::make_shared<ShapeDictionaryData<ScalarType>>();
    this->Clear();
}


template<typename ScalarType>
ShapeDictionary<ScalarType>::ShapeDictionary(const ShapeDictionary<ScalarType>& InputDictionary)
{
    m_Data = std::make_shared<ShapeDictionaryData<ScalarType>>();
    this->Copy(InputDictionary);
}


template<typename ScalarType>
ShapeDictionary<ScalarType>::ShapeDictionary(ShapeDictionary<ScalarType>&& InputDictionary)
{
	(*this) = std::move(InputDictionary);
}


template<typename ScalarType>
ShapeDictionary<ScalarType>::~ShapeDictionary()
{
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::operator=(const ShapeDictionary<ScalarType>& InputDictionary)
{
    this->Copy(InputDictionary);
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::operator=(ShapeDictionary<ScalarType>&& InputDictionary)
{
	this->Copy(std::move(InputDictionary));
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::Copy(const ShapeDictionary<ScalarType>& InputDictionary)
{
    if (m_Data == InputDictionary.m_Data)
    {
        return;
    }

    m_Data->Name = InputDictionary.m_Data->Name;
    m_Data->Basis = InputDictionary.m_Data->Basis;
    m_Data->SeedForNewBasisIDGeneration = InputDictionary.m_Data->SeedForNewBasisIDGeneration.load();
    m_Data->BasisID = InputDictionary.m_Data->BasisID;
    m_Data->CurrentDictionaryTime = InputDictionary.m_Data->CurrentDictionaryTime;
    m_Data->BasisAge = InputDictionary.m_Data->BasisAge;
    m_Data->BasisExperience = InputDictionary.m_Data->BasisExperience;
    m_Data->BasisSimilarity = InputDictionary.m_Data->BasisSimilarity;
    m_Data->BasisRedundancy = InputDictionary.m_Data->BasisRedundancy;
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::Copy(ShapeDictionary<ScalarType>&& InputDictionary)
{
    m_Data->Name = std::move(InputDictionary.m_Data->Name);
    m_Data->Basis = std::move(InputDictionary.m_Data->Basis);	
    m_Data->SeedForNewBasisIDGeneration = InputDictionary.m_Data->SeedForNewBasisIDGeneration.load();
    m_Data->BasisID = std::move(InputDictionary.m_Data->BasisID);
    m_Data->CurrentDictionaryTime = InputDictionary.m_Data->CurrentDictionaryTime;
    m_Data->BasisAge = std::move(InputDictionary.m_Data->BasisAge);
    m_Data->BasisExperience = std::move(InputDictionary.m_Data->BasisExperience);
    m_Data->BasisSimilarity = std::move(InputDictionary.m_Data->BasisSimilarity);
    m_Data->SimilarityThreshold = InputDictionary.m_Data->SimilarityThreshold;
    m_Data->BasisRedundancy = std::move(InputDictionary.m_Data->BasisRedundancy);

    InputDictionary.Clear();
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::Share(ShapeDictionary<ScalarType>& InputDictionary)
{
	if (this == &InputDictionary)
	{
		return;
	}

	m_Data = InputDictionary.m_Data;
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::ForceShare(const ShapeDictionary<ScalarType>& InputDictionary)
{
	if (this == &InputDictionary)
	{
		return;
	}

	m_Data = InputDictionary.m_Data;
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::Clear()
{
	if (!m_Data)
	{
		return;
	}

	m_Data->Name = "";
    m_Data->Basis.Clear();
    m_Data->SeedForNewBasisIDGeneration = 0;
    m_Data->BasisID.Clear();
    m_Data->CurrentDictionaryTime = 0;
    m_Data->BasisAge.Clear();
    m_Data->BasisExperience.Clear();
    m_Data->BasisSimilarity.Clear();
    m_Data->SimilarityThreshold = 0;
    m_Data->BasisRedundancy.Clear();
}


template<typename ScalarType>
bool ShapeDictionary<ScalarType>::Load(const std::string& FilePathAndName)
{
    //auto temp = LoadShapeDictionaryFromJsonDataFile<ScalarType>(FilePathAndName);
    //if (temp.IsEmpty() == false)
    //{
    //    this->Take(temp);
    //    return true;
    //}

    return false;
}


template<typename ScalarType>
bool ShapeDictionary<ScalarType>::Save(const std::string& FilePathAndName) const
{
    //return SaveShapeDictionaryAsJsonDataFile(*this, FilePathAndName);
	return false;
}


template<typename ScalarType>
ShapeDictionary<ScalarType>
ShapeDictionary<ScalarType>::GetSubDictionary(const DenseMatrix<int_max>& SelectedBasisIndexList) const
{
    ShapeDictionary<ScalarType> SubDictionary;

    if (SelectedBasisIndexList.IsVector() == false)
    {
        MDK_Error("Input is invalid @ ShapeDictionary::GetSubDictionary(...)")
        return SubDictionary;
    }

    int_max BasisCount = m_Data->Basis.GetColCount();

    // check index range
    for (int_max k = 0; k < SelectedBasisIndexList.GetElementNumber(); ++k)
    {
        auto tempIndex = SelectedBasisIndexList[k];

        if (tempIndex < 0 || tempIndex >= BasisCount)
        {
            MDK_Error("Input Index is invalid @ ShapeDictionary::GetSubDictionary(...)")
            return SubDictionary;
        }
    }

    // get data
    SubDictionary.m_Data->Basis = m_Data->Basis.GetSubMatrix(ALL, SelectedBasisIndexList);
    SubDictionary.m_Data->BasisID = m_Data->BasisID.GetSubMatrix(SelectedBasisIndexList);
    SubDictionary.m_Data->SeedForNewBasisIDGeneration = m_Data->SeedForNewBasisIDGeneration.load();
    SubDictionary.m_Data->CurrentDictionaryTime = m_Data->CurrentDictionaryTime;//???????
    SubDictionary.m_Data->BasisAge = m_Data->BasisAge.GetSubMatrix(SelectedBasisIndexList);
    SubDictionary.m_Data->BasisExperience = m_Data->BasisExperience.GetSubMatrix(SelectedBasisIndexList);
    SubDictionary.m_Data->BasisSimilarity = m_Data->BasisSimilarity.GetSubMatrix(SelectedBasisIndexList, SelectedBasisIndexList);
    SubDictionary.m_Data->SimilarityThreshold = m_Data->SimilarityThreshold;
    SubDictionary.m_Data->BasisRedundancy = m_Data->BasisRedundancy.GetSubMatrix(SelectedBasisIndexList);
    //------------------------------------------------------
    return SubDictionary;
}


template<typename ScalarType>
void ShapeDictionary<ScalarType>::MergeDictionary(const ShapeDictionary<ScalarType>& InputDictionary)
{
    if (InputDictionary.IsEmpty() == true)
    {
        MDK_Warning("InputDictionary is empty @ ShapeDictionary::CombineDictionary(...)")
        return;
    }

    if (this->IsEmpty() == true)
    {
        this->Copy(InputDictionary);
        return;
    }
	
	//----------------------- start to merge ---------------------------------------------------------------

    int_max BasisCount_input = InputDictionary.m_Data->Basis.GetLength();

    int_max BasisCount_self = m_Data->Basis.GetLength();

    int_max BasisCount_combined = BasisCount_self + BasisCount_input;

    // update SeedForNewBasisIDGeneration
    m_Data->SeedForNewBasisIDGeneration = std::max(m_Data->SeedForNewBasisIDGeneration.load(), InputDictionary.m_Data->SeedForNewBasisIDGeneration.load());

    // update Dictionary Time
    m_Data->CurrentDictionaryTime = m_Data->CurrentDictionaryTime + InputDictionary.m_Data->CurrentDictionaryTime;

    // update other

    m_Data->Basis = { &m_Data->Basis, &InputDictionary.m_Data->Basis };

    m_Data->BasisID = { &m_Data->BasisID, &InputDictionary.m_Data->BasisID };

    m_Data->BasisID = { &m_Data->BasisID, &InputDictionary.m_Data->BasisID };

    m_Data->BasisAge = { &m_Data->BasisAge, &InputDictionary.m_Data->BasisAge };

    m_Data->BasisExperience = { &m_Data->BasisExperience, &InputDictionary.m_Data->BasisExperience };

    // update SimilarityMatrix : fill 0 to unknown Similarity

    DenseMatrix<ScalarType> SimilarityMatrix_self = m_Data->BasisSimilarity;

    m_Data->BasisSimilarity.FastResize(BasisCount_combined, BasisCount_combined);
    m_Data->BasisSimilarity.Fill(0);

    auto IndexRange_self = span(0, BasisCount_self - 1);
    m_Data->BasisSimilarity(IndexRange_self, IndexRange_self) = SimilarityMatrix_self;

    auto IndexRange_input = span(BasisCount_self, BasisCount_combined - 1);
    m_Data->BasisSimilarity(IndexRange_input, IndexRange_input) = InputDictionary.m_Data->BasisSimilarity;

    // update BasisRedundancy
    m_Data->BasisRedundancy = { &m_Data->BasisRedundancy, &InputDictionary.m_Data->BasisRedundancy };
}


template<typename ScalarType>
inline
int_max ShapeDictionary<ScalarType>::GenerateNewBasisID()
{
    // overflow detection ?

    m_Data->SeedForNewBasisIDGeneration += 1;

    return m_Data->SeedForNewBasisIDGeneration;
}


}// namespace mdk
