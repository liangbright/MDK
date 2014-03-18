#ifndef __mdkFeatureCodebookBuilder_h
#define __mdkFeatureCodebookBuilder_h

#include <vector>
#include <string>

#include "mdkFeatureCodebook.h"

namespace mdk
{

template<typename ElementType>
class mdkFeatureCodebookBuilder : public mdkObject
{
protected:

    mdkFeatureCodebook<ElementType> m_Codebook;

    int64 m_CodebookLength;

    mdkFeatureEncoder<ElementType>* m_GenericEncoder;

    mdkMatrix<ElementType> m_FeatureData;

public:

    mdkFeatureCodebookBuilder();

    mdkFeatureCodebookBuilder(mdkFeatureCodebookBuilder&&);

    ~mdkFeatureCodebookBuilder();

    void operator=(mdkFeatureCodebookBuilder&&);

    //---------------------------------------------------//

    void Reset();

    void Clear();

    //---------------------------------------------------//

    void SetInitialCodebook(const mdkMatrix<ElementType>& InitialCodebook);

    bool LoadCodebook(const std::string& FilePathAndName);

    bool SaveCodebook(const std::string& FilePathAndName);

    bool SetOutputCodebook(mdkFeatureCodebook<ElementType>& Codebook);

    bool SetCodebookLength(int64 Length);

    //---------------------------------------------------//

    void SetGenericEncoder(const mdkFeatureEncoder<ElementType>* FeatureEncoder);

    //---------------------------------------------------//

    bool SetInputFeatureData(const mdkMatrix<ElementType>& InputFeatureData);

    bool LoadFeatureData(const std::string& FilePathAndName);

    //----------------------------------------------------//

    virtual bool Run();

    virtual bool GenerateCodebook();

    //----------------------------------------------------//

    mdkFeatureCodebook<ElementType>& GetOutputCodebook();


private:
//deleted
    mdkFeatureCodebookBuilder(const mdkFeatureCodebookBuilder&) = delete;

    void operator=(const mdkFeatureCodebookBuilder&) = delete;

};

}// namespace mdk


#include "mdkFeatureCodebookBuilder.hpp"

#endif