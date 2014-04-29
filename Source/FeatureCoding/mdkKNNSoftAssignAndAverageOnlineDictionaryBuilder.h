#ifndef __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_h
#define __mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder_h


#include <random>


#include "mdkFileIO.h"
#include "mdkString.h"
#include "mdkDataContainer.h"
#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"
#include "mdkKNNSoftAssignOnlineDictionaryBuilder.h"


namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder
{
    std::string DictionaryName;

    int_max BasisNumber;

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> ParameterOfKNNSoftAssign;

    // sort the pair i, j according to score = weigth_s * Similarity(i, j) + (1-weigth_s) * 0.5*(prob(i) + prob(j))
    ElementType weigth_s;

    // prob_basis_updated = weigth_past * prob_basis_past + (1-weigth_past) * prob_basis_data;
    ElementType weigth_past; // weight for the past experience, i.e., the initial dictionary

    // parameter for data sampling --------

    int_max NumberOfDataInEachBatch; // the number of data in each batch/thread

    int_max MaxNumberOfIteration;

    int_max MaxNumberOfThreads;

    // parameter for updating reconstruction information after the dictionary is built

    Parameter_Of_KNNReconstructionSparseEncoder ParameterOfKNNReconstruction;

//--------------------------------------------------------------------------------------------------------

    Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder() {}

    void Clear()
    {
        ParameterOfKNNSoftAssign.Clear();

        BasisNumber = -1;

        weigth_s = 0;

        weigth_past = 0;

        NumberOfDataInEachBatch = -1;
        MaxNumberOfIteration = -1;

        MaxNumberOfThreads = 1;

        ParameterOfKNNReconstruction.Clear();
    }
};


template<typename ElementType>
class KNNSoftAssignAndAverageOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
public:
    Parameter_Of_KNNSoftAssignAndAverageOnlineDictionaryBuilder<ElementType> m_Parameter;

private:

    const DenseMatrix<ElementType>* m_FeatureData;

    FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType>  m_Dictionary_SharedCopy;

    KNNSoftAssignOnlineDictionaryBuilder<ElementType> m_KNNSoftAssignDictionaryBuilder;

public:
    KNNSoftAssignAndAverageOnlineDictionaryBuilder();
    ~KNNSoftAssignAndAverageOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* InitialDictionary);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void SetupDefaultPipelineOutput();

    void UpdatePipelineOutput();

    void GenerateDictionary();

    FeatureDictionaryForSparseCoding<ElementType> GenerateDictionaryFromDataBatch(int_max BasisNumber_desired,
                                                                                  const DenseMatrix<ElementType>& FeatureData,
                                                                                  const DenseMatrix<ElementType>& ProbabilityMassFunctionOfData,
                                                                                  const FeatureDictionaryForSparseCoding<ElementType>* Dictionary_init);

};


}// namespace mdk


#include "mdkKNNSoftAssignAndAverageOnlineDictionaryBuilder.hpp"

#endif