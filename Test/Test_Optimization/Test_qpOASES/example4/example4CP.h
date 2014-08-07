/*
 *	This file is part of qpOASES.
 *
 *	qpOASES -- An Implementation of the Online Active Set Strategy.
 *	Copyright (C) 2007-2014 by Hans Joachim Ferreau, Andreas Potschka,
 *	Christian Kirches et al. All rights reserved.
 *
 *	qpOASES is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation; either
 *	version 2.1 of the License, or (at your option) any later version.
 *
 *	qpOASES is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *	See the GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with qpOASES; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


/**
 *	\file examples/example4CP.cpp
 *	\author Hans Joachim Ferreau
 *	\version 3.0beta
 *	\date 2009-2014
 *
 *	Sample implementation of the ConstraintProduct class tailored for Example4.
 */


BEGIN_NAMESPACE_QPOASES


/** 
 *	\brief Example illustrating the use of the \a ConstraintProduct class.
 *
 *	Example illustrating the use of the \a ConstraintProduct class.
 *
 *	\author Hans Joachim Ferreau
 *	\version 3.0beta
 *	\date 2007-2014
 */
template<typename ScalarType>
class MyConstraintProduct : public ConstraintProduct<ScalarType>
{
	public:
		/** Default constructor. */
		MyConstraintProduct( ) {};

		/** Constructor. */
		MyConstraintProduct(	int_max _nV,
								int_max _nC,
								ScalarType* _A
								)
		{
			nV = _nV;
			nC = _nC;
			A  = _A;
		};

		/** Copy constructor (flat copy). */
		MyConstraintProduct(	const MyConstraintProduct& rhs
								)
		{
			nV = rhs.nV;
			nC = rhs.nC;
			A  = rhs.A;
		};

		/** Destructor. */
		virtual ~MyConstraintProduct( ) {};
		
		/** Assignment operator (flat copy). */
		MyConstraintProduct& operator=(	const MyConstraintProduct& rhs
										)
		{
			if ( this != &rhs )
			{
				nV = rhs.nV;
				nC = rhs.nC;
				A  = rhs.A;
			}
			else
				return *this;
		}

		virtual int_max operator() (	int_max constrIndex,
									const ScalarType* const x,
									ScalarType* const constrValue
									) const
		{
			constrValue[0] = 1.0 * x[(constrIndex/10)+2];

			for( int_max i=0; i<2; ++i )
				constrValue[0] += A[constrIndex*nV + i] * x[i];

			return 0;
		};

	protected:
		int_max nV;			/**< Number of variables. */
		int_max nC;			/**< Number of constraints. */
		ScalarType* A;		/**< Pointer to full constraint matrix (typically not needed!). */
};


END_NAMESPACE_QPOASES

