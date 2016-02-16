// Polynomial.h

#ifndef _VBI_POLYMATH_POLYNOMIAL_H_
#define _VBI_POLYMATH_POLYNOMIAL_H_

// A polynomial is represented as an ordered STL set of monomial terms
typedef std::set< Monomial, std::less< Monomial > > Poly;
typedef std::set< Monomial, std::less< Monomial > >::iterator PolyIter;
typedef std::set< Monomial, std::less< Monomial > >::const_iterator constPolyIter;

// Class to represent a polynomial over Z2
class Polynomial
{
public:
	// Default constructor
	Polynomial( )
	{
		mK = 1;
		mHammingScore		= 0.0;
		mComplexityScore	= 0.0;
		mRevEngScore		= 0.0;
		mBioProbScore		= 0.0;
		mTotalScore			= 0.0;
		mMutated = true;
	}

	// Copy constructor
	Polynomial( const Polynomial& poly );

	// Convert this object to a random polynomial with
	void GenerateRandomPolynomial( size_t max_terms );

	// Add a monomial term to this polynomial
	void AddTerm( Monomial& term );

	// Evaluate this polynomial at the given N-tuple x
	bool Evaluate( NTuple& x );

	// Make a single mutation to this polynomial
	void Mutate( );

	// Turn off the mutated state of this polynomial
	//	this should be done only after all scores have been re-computed
	inline void UnMutate( )	{ mMutated = false;	}

	// Return true if the polynomial has changed since scores were updated
	inline bool IsMutatedQ() { return mMutated; }

	// Size returns number of monomials in this polynomial
	inline size_t Size() const { return mPoly.size(); } 

	// Complexity returns the average number of variables in the monomials
	//   of this polynomial
	size_t Cost() const;

	// Return a reference to the i'th monomial of this polynomial
	const Monomial& operator[]( size_t i );

	// Remove the i'th monomial, and return a copy of it.
	Monomial Remove( size_t i );

	// Compare this polynomial to another, return true if they are identical
	bool operator==( Polynomial& other );

	inline bool IsDefaultPolyQ() { return mPoly.size() == 0; }

	// Set the Hamming score for this polynomial
	inline void SetHammingScore( double hamming_ratio )
	{
		// Take the complement of the hammingRatio to compute the score between 0 and 1
		mHammingScore = (1 - hamming_ratio );
		mMutated = true;
	}

	// Set the complexity score for this polynomial
	void SetComplexityScore( double cs );

	// Set the Dependency Graph Score for this polnomial
	void SetDepGraphScore( ComplexityMatrixRow& cm_row );

	// Return the maximum degree over all terms
	size_t MaxDegree( );

	// Set the maximum number of variables to be included in any Monomial
	void static SetMaxSupport( size_t max_support )
	{
		mMaxSupport = max_support;
		// Push down to the Monomial class also
		Monomial::SetMaxSupport( max_support );
	}

	size_t MaxSupport() { return mMaxSupport; }

	double EvaluateWeightMatrix( WeightMatrixRow& weight_row );

	// Create a Reverse Engineering score based on a row in the Reverse Eng. Weight Matrix
	void ScoreReverseEngineeringWeights( WeightMatrixRow& weight_row );

	// Create a Biological Probability score based on row in the Biological Probability Matrix
	void ScoreBioProbabilities( WeightMatrixRow& weight_row );

	// Return the probability that we should use this polynomial based on its composite score
	double GetPolyProb( );

	// Convert this polynomial to a string in human readable form
	String ToString( bool with_star = false ) const;

	// Support output of a Monomial to a stream
	friend std::ostream& operator<<( std::ostream& out, const Polynomial& p );

// private:

	// Linear interpolate f(x) where y1 = f(x1), y2 = f(x2) and x1 <= x <= x2
	static double LinearInterp( double x1, double y1,
			double x2, double y2, double x );

	// Compute the Complexity Function of x, where 0. <= x <= 1.0
	static double ComplexityFunction( double x );

	// Maximum support for any monomial - this is static, only one copy for all polynomials
	static size_t mMaxSupport;

	// A function number for this polynomial.
	size_t	mK;

	// Single variable Hamming score for this polynomial
	double mHammingScore;

	// Complexity score for this polynomial
	double mComplexityScore;

	// Reverse Engineering Weight Matrix Score
	double mRevEngScore;

	// Biological Probability score
	double mBioProbScore;

	// A score computed as an agregate of the individual score
	double mTotalScore;

	// Set true after mutation, to force recomputation of scores
	bool mMutated;

	// The polynomial is represented as an ordered STL set of monomial terms
	Poly	mPoly;
};

// Define a vector of Polynomials
typedef std::vector<Polynomial> PolyModel;
typedef std::vector<Polynomial>::iterator PolyModelIter;

#endif // _VBI_POLYMATH_POLYNOMIAL_H_
