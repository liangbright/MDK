#ifndef __mdkCombinatorialMath_Function_Part0_hpp
#define __mdkCombinatorialMath_Function_Part0_hpp

namespace mdk
{

int_max	ComputeBinomialCoefficient(int_max N, int_max K)
{
	double Coef = 1;
	for (int_max i = 1; i <= K; ++i)
	{
		Coef *= double(N - K + i) / double(i);
	}

	return int_max(Coef);
}

DataArray<DenseVector<int_max>> NChooseK(int_max N, int_max K)
{
// from http://stackoverflow.com/questions/19327847/n-choose-k-for-large-n-and-k

	DataArray<DenseVector<int_max>> SubSetList;
	SubSetList.ReserveCapacity(N*K);

	DenseVector<int_max> CurrentSubSet;
	CurrentSubSet.Resize(K);

	for (int_max i = 0; i < K; i++)
	{	
		CurrentSubSet[i] = i;
	}
	CurrentSubSet[K-1] = K - 1 - 1; // fill initial combination is real first combination -1 for last number, as we will increase it in loop

	do
	{
		if (CurrentSubSet[K - 1] == (N - 1)) // if last number is just before overwhelm
		{
			int_max i = K - 1 - 1;
			while (CurrentSubSet[i] == (N - K + i))	{ i--; }

			CurrentSubSet[i]+=1;

			for (int_max j = i + 1; j < K; j++)
			{
				CurrentSubSet[j] = CurrentSubSet[i] + j - i;
			}
		}
		else
		{
			CurrentSubSet[K-1]+=1;
		}

		SubSetList.Append(CurrentSubSet);

	} while (!((CurrentSubSet[0] == (N - 1 - K + 1)) && (CurrentSubSet[K - 1] == (N - 1))));

	return SubSetList;
}

}//namespace mdk


#endif