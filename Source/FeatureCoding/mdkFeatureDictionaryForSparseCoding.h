#ifndef __mdkFeatureDictionaryForSparseCoding_h
#define __mdkFeatureDictionaryForSparseCoding_h

#include <atomic>

#include "mdkFileIO.h"
#include "mdkFeatureDictionary.h"
#include "mdkDenseMatrix.h"
#include "mdkFeatureCoding_Common_Type.h"

namespace mdk
{
// sparse coding based Dictionary learning
// ||X - DZ||^2 + function(Z), 
// X: Feature Vector
// D: Dictionary
// Z: sparse code
//
// about WeightMatrix:
// during training, each Feature Vector may have a weight vector (mask)
// e.g., remove surrounding stuff about an object in an image, only the object is inside the mask
// an element of the Feature Vector is set to zero if it is out of mask
// an element is out of mask : mask(element position) = 0
// an element is in mask : mask(element position) > 0 or < 0
// Therefore, after modify each Feature Vector using mask, mask is not used in Dictionary learning, only the modified Feature Vector is used
//
// WeightMatrix is useful in encoding (e.g., KNN based encoding), and is useful in classification if reconstruction error vector norm is used
// w_k=WeightMatrix(:,k), 0<= w_k <=1, X_k=w_k.*X; get differnece version of X; 
// distance between X and d_k =||w_k.*(X - d_k)|| = sqrt(sum_n(X_k(n)-w_k(n)*d_k(n)))^2/sum_n(w_k(n)))
// for KNN, find mask of each KNN Basis, then get combined mask m_com, and get X_com = m_com.*X
// to get reconstructed X: ||X_k - [m_com.*d1, m_com.*d2, ...]*Z|| 
//
// WeightMatrix may not be useful in classification if reconstruction error vector is used
// reconstruction error vector = |w_k.*(X - d_k)|, take absolute value of each element in the error vector 
// then apply SVM using 1 vs ALL, an element out of mask will be assigned a small weight
//

template<typename ScalarType>
struct DictionaryData_Of_FeatureDictionaryForSparseCoding
{
    std::string Name; // name of the dictionary

    DenseMatrix<ScalarType> BasisMatrix; // D
    // ColNumber is BasisNumber (the number of bases)
    // RowNumber is Length of Feature Data Vector

	DenseMatrix<ScalarType> WeightMatrix; // W

    //------------ basis unique ID -----------------------------------
    // row vector

    DenseMatrix<int_max> BasisID;

    std::atomic<int_max> SeedForNewBasisIDGeneration;

    //------------ constraint on basis -----------------------------------

    bool BasisPositive;
    bool BasisNormalizedWithL1Norm;
    bool BasisNormalizedWithL2Norm;

    //----------- Current "Dictionary Time " --------------------

    int_max CurrentDictionaryTime; // Measured By Total Number Of Samples In Training History;

    //------------ Age of each basis ----------------------------
    // row vector

    DenseMatrix<ScalarType> BasisAge;

    // ---------- basis  Experience on Representing Data ----
    // row vector

    DenseMatrix<ScalarType> BasisExperience;
    // the weighted total number of training data samples
    // for each basis from day 0

    //---------- Similarity and Redundancy between basis vector -----

    VectorSimilarityTypeEnum SimilarityType;

    DenseMatrix<ScalarType> SimilarityMatrix;

    ScalarType SimilarityThreshold_For_ComputeBasisRedundancy;

    DenseMatrix<ScalarType> BasisRedundancy;
    // BasisRedundancy[j] =  the number of the other bases near the basis j ( Similarity(i, j) >= SimilarityThreshold) 
    // range [0, BasisNumber-1]

    //----------------- Variance -------------------------
	// row vector

	//DenseMatrix<ScalarType> VarianceForDecoder;
	// reserved, not used yet 

    DenseMatrix<ScalarType> VarianceOfL1Distance; 
    // Length = BasisNumber
    // Variance_i_j = || X_i - D(:, j)||^2
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ScalarType> VarianceOfL2Distance;
    // Length = BasisNumber
    // Variance_i_j = || X_i - D(:, j)||^2
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ScalarType> VarianceOfKLDivergence; // not real Variance
    // Length = BasisNumber
    // Variance_i_j = KL( X_i, - D(:, j))
    // Variance(j) = sqrt(sum_i(Indicator_i_j * Variance_i_j))
    // if Basis_j is related to X_i, then  Indicator_i_j = 1, else it = 0

    DenseMatrix<ScalarType> VarianceOfReconstruction;
    // Variance_i_j = || X_i - D * Alpha||^2
    // VarianceOfReconstruction(j) = mean (ErrorNorm_i related to Basis_j (i.e., Alpha(j) > 0 ))

	//----------- Covariance between bases --------------------------
    // not used yet ----------------
    //DenseMatrix<ScalarType> BasisCovariance;  // relation between bases

    //---------------------------------------------------------------------//
};


template<typename Scalar_Type>
class FeatureDictionaryForSparseCoding : public FeatureDictionary<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
    std::shared_ptr<DictionaryData_Of_FeatureDictionaryForSparseCoding<ScalarType>> m_DictionaryData;

public:
    FeatureDictionaryForSparseCoding();

    FeatureDictionaryForSparseCoding(const FeatureDictionaryForSparseCoding& InputDictionary);

    FeatureDictionaryForSparseCoding(FeatureDictionaryForSparseCoding&& InputDictionary);

    ~FeatureDictionaryForSparseCoding();

    void operator=(const FeatureDictionaryForSparseCoding& InputDictionary);

    void operator=(FeatureDictionaryForSparseCoding&& InputDictionary);

    void Copy(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool Copy(const FeatureDictionaryForSparseCoding* InputDictionary);

    void Share(FeatureDictionaryForSparseCoding& InputDictionary);

    bool Share(FeatureDictionaryForSparseCoding* InputDictionary);

    void ForceShare(const FeatureDictionaryForSparseCoding& InputDictionary);

    bool ForceShare(const FeatureDictionaryForSparseCoding* InputDictionary);

    void Take(FeatureDictionaryForSparseCoding& InputDictionary);

    void Take(FeatureDictionaryForSparseCoding* InputDictionary);

    void Clear();

    bool IsEmpty() const;

	MatrixSize GetSize() const;

    bool Load(const std::string& FilePathAndName);

	bool Save(const std::string& FilePathAndName) const;

    FeatureDictionaryForSparseCoding<ScalarType> GetSubDictionary(const DenseMatrix<int_max>& BasisIndexList_to_keep) const;

    void CombineDictionary(const FeatureDictionaryForSparseCoding<ScalarType>& InputDictionary);

    // -------------- get/set ---------------------------------------------------//

	inline const std::string& GetName() const;

	inline void SetName(const std::string& Name);

    inline DenseMatrix<ScalarType>& BasisMatrix();

    inline const DenseMatrix<ScalarType>& BasisMatrix() const;

	inline DenseMatrix<ScalarType>& WeightMatrix();

	inline const DenseMatrix<ScalarType>& WeightMatrix() const;

    inline int_max GenerateNewBasisID();

    inline int_max GetCurrentSeedForNewBasisIDGeneration() const;

    inline void SetCurrentSeedForNewBasisIDGeneration(int_max Seed);

    inline DenseMatrix<int_max>& BasisID();

    inline const DenseMatrix<int_max>& BasisID() const;

    inline void SetProperty_BasisPositive(bool YesNO);
    inline void SetProperty_BasisNormalizedWithL1Norm(bool YesNO);
    inline void SetProperty_BasisNormalizedWithL2Norm(bool YesNO);

    inline bool GetProperty_BasisPositive() const;
    inline bool GetProperty_BasisNormalizedWithL1Norm() const;
    inline bool GetProperty_BasisNormalizedWithL2Norm() const;

    inline void SetCurrentDictionaryTime(int_max DictionaryTime);
    inline int_max GetCurrentDictionaryTime() const;

    inline DenseMatrix<ScalarType>& BasisAge();

    inline const DenseMatrix<ScalarType>& BasisAge() const;

    inline DenseMatrix<ScalarType>& BasisExperience();
    inline const DenseMatrix<ScalarType>& BasisExperience() const;

    inline DenseMatrix<ScalarType>& BasisRedundancy();
    inline const DenseMatrix<ScalarType>& BasisRedundancy() const;

    inline void SetProperty_SimilarityType(VectorSimilarityTypeEnum SimilarityType);
    inline VectorSimilarityTypeEnum GetProperty_SimilarityType() const;

    inline DenseMatrix<ScalarType>& SimilarityMatrix();
    inline const DenseMatrix<ScalarType>& SimilarityMatrix() const;

    inline void SetProperty_SimilarityThresholdForComputeBasisRedundancy(ScalarType SimilarityThreshold);
    inline ScalarType GetProperty_SimilarityThresholdForComputeBasisRedundancy() const;

    inline DenseMatrix<ScalarType>& VarianceOfL1Distance();
    inline const DenseMatrix<ScalarType>& VarianceOfL1Distance() const;

    inline DenseMatrix<ScalarType>& VarianceOfL2Distance();
    inline const DenseMatrix<ScalarType>& VarianceOfL2Distance() const;

    inline DenseMatrix<ScalarType>& VarianceOfKLDivergence();
    inline const DenseMatrix<ScalarType>& VarianceOfKLDivergence() const;

    inline DenseMatrix<ScalarType>& VarianceOfReconstruction();
    inline const DenseMatrix<ScalarType>& VarianceOfReconstruction() const;
};


}// namespace mdk

#include "mdkFeatureDictionaryForSparseCoding.hpp"


#endif