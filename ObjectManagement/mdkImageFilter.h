#include <memory>

#include <mdkObjectBase.h>

namespace mdk
{

class ImageFilter : public ObjectBase
{
public:
	double m_InputImage;
	double m_OutputImage;

public:

	ImageFilter();

	virtual ~ImageFilter();

	bool Initialize();

	bool SetInput(const double);
	
	bool Apply();

	bool GetOutput(double*);
	
	virtual std::string GetClassName();
};
}// end namespace mdk