#ifndef __mdkFeatureDictionaryForDenseCoding_h
#define __mdkFeatureDictionaryForDenseCoding_h

#include "mdkFeatureDictionary.h"

namespace mdk
{

template<typename ElementType>
struct DictionaryData_Of_FeatureDictionaryForDenseCoding
{
	std::string Name;

	DenseMatrix<ScalarType> BasisMatrix; // D

	DenseMatrix<ScalarType> MeanVector; //col vector, sample mean in PCA

	DenseMatrix<ScalarType> BasisCovariance;
	// relation between bases

	DenseMatrix<ScalarType> Variance;
	// Variance(j) = sum_i(Prob(i,j)*(FeatureData_i - D(:,j))^2)
	
};

template<typename Scalar_Type>
class FeatureDictionaryForDenseCoding : public FeatureDictionary<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

private:
	std::shared_ptr<DictionaryData_Of_FeatureDictionaryForDenseCoding<ScalarType>> m_DictionaryData;
   
public:

    FeatureDictionaryForDenseCoding();

    FeatureDictionaryForDenseCoding(const FeatureDictionaryForDenseCoding& InputDictionary);

    FeatureDictionaryForDenseCoding(FeatureDictionaryForDenseCoding&& InputDictionary);

    ~FeatureDictionaryForDenseCoding();

    void operator=(const FeatureDictionaryForDenseCoding& InputDictionary);

    void operator=(FeatureDictionaryForDenseCoding&& InputDictionary);

    bool Copy(const FeatureDictionaryForDenseCoding& InputDictionary);

    bool Copy(const FeatureDictionaryForDenseCoding* InputDictionary);

    bool Share(FeatureDictionaryForDenseCoding& InputDictionary);

    bool Share(FeatureDictionaryForDenseCoding* InputDictionary);

    void ForceShare(const FeatureDictionaryForDenseCoding& InputDictionary);

    bool ForceShare(const FeatureDictionaryForDenseCoding* InputDictionary);

    void Clear();

    bool IsEmpty() const;

    MatrixSize GetSize() const;

    bool Load(const CharString& FilePathAndName);

    bool Save(const CharString& FilePathAndName) const;

    std::string& Name();

    const std::string& Name() const;

    DenseMatrix<ScalarType>& BasisMatrix();

    const DenseMatrix<ScalarType>& BasisMatrix() const;

};


}// namespace mdk

#include "mdkFeatureDictionaryForDenseCoding.hpp"


#endif