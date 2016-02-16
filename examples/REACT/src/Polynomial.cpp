// Polynomial.cpp

#include "BoolModel.h"

// Static variables for the polynomial class
size_t Polynomial::mMaxSupport = 1; 

// Curve to penalize both very simple and very complex polynomials
double ComplexityCurve[][2] =
{
	{ 0.00000,   0.10000 },
	{ 0.04960,   0.29463 },
	{ 0.09918,   0.48102 },
	{ 0.14874,   0.65094 },
	{ 0.19825,   0.79616 },
	{ 0.24772,   0.90844 },
	{ 0.29711,   0.97954 },
	{ 0.34643,   1.00136 },
	{ 0.39568,   0.97320 },
	{ 0.44493,   0.90589 },
	{ 0.49425,   0.81124 },
	{ 0.54370,   0.70108 },
	{ 0.59336,   0.58720 },
	{ 0.64329,   0.48141 },
	{ 0.69354,   0.39479 },
	{ 0.74412,   0.32953 },
	{ 0.79497,   0.28216 },
	{ 0.84604,   0.24910 },
	{ 0.89727,   0.22677 },
	{ 0.94861,   0.21159 },
	{ 1.00000,   0.20000 },
	{ 1.10000,   0.00000 }
};

// Linear interpolate f(x) where y1 = f(x1), y2 = f(x2) and x1 <= x <= x2
double Polynomial::LinearInterp( double x1, double y1, double x2, double y2, double x )
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	if( fabs( dx ) < 1e-9 ) return y1;

	return y1 + (x - x1)*dy/dx;
}

double Polynomial::ComplexityFunction( double x )
{
	// k is the number of pairs in ComplexityCurve
	size_t k = sizeof( ComplexityCurve ) / sizeof( ComplexityCurve[0] );

	// Don't extrapolate - just give the endpoint if out of range
	if( x < ComplexityCurve[0  ][0] ) return ComplexityCurve[0 ][1];
	if( x > ComplexityCurve[k-1][0] ) return ComplexityCurve[k-1][1];
	
	// Search for the first entry with coordinate larger than x
	size_t i = 1;
	while( ComplexityCurve[i][0] <= x   ) ++i;

	// When we get here ComplexityCurve[i-1][0] <= x < ComplexityCurve[i][0]
	double x1 = ComplexityCurve[i-1][0];	double y1 = ComplexityCurve[i-1][1];
	double x2 = ComplexityCurve[i  ][0];	double y2 = ComplexityCurve[i  ][1];

	return LinearInterp( x1, y1, x2, y2, x );
}

// Copy constructor
Polynomial::Polynomial( const Polynomial& poly )
{
	// Copy ALL the stuff
	mK               = poly.mK;
	mMutated		 = poly.mMutated;
	mHammingScore	 = poly.mHammingScore;
	mComplexityScore = poly.mComplexityScore;
	mRevEngScore	 = poly.mRevEngScore;
	mBioProbScore	 = poly.mBioProbScore;
	mTotalScore		 = poly.mTotalScore;

	constPolyIter poly_iter = poly.mPoly.begin();
	while( poly_iter != poly.mPoly.end() )
	{
		mPoly.insert( *poly_iter );
		++poly_iter;
	}
}


// Add a new term to this polynomial
void Polynomial::AddTerm( Monomial& term )
{
	mPoly.insert( term );
	mMutated = true;
}

// Convert this object to a random polynomial with up to max_terms 
void Polynomial::GenerateRandomPolynomial( size_t max_terms )
{
	// Clear out old contents
	mPoly.clear();

	// with prob = 0.5 create a constant term = 1 else 0
	Monomial term;
	if( ( PolyMathRand::RandInt() % 2 ) )
	{
		term.mCoeff = false;
	}
	mPoly.insert( term );

	// Choose number of terms with an exponential distribution
	int k = (int)PolyMathRand::RandIntExp( 1 + (int)(max_terms/5) );
	if( k > (int) max_terms) k = (int)max_terms;
	while( --k >= 0 )
	{
		// Generate and add a random monomial
		Monomial term;
		term.Randomize( );
		mPoly.insert( term );
	}
	mMutated = true;
}

// Evaluate this polynomial at the given N-tuple x
bool Polynomial::Evaluate( NTuple& x )
{
	if( mPoly.empty() )
		throw("Polynomial - attempt to evaluate uninitialized polynomial");

	PolyIter iter = mPoly.begin();
	int y = 0;
	while( iter != mPoly.end() )
	{
		// The result is the Z2 sum (XOR) of the evaluation
		// at each monomial that makes up this polynomial
		// Monomial m = *iter;
		y += iter++->Evaluate( x );
		//++iter;
	}
	// Return true if the sum is odd, equivalently if y = 1 (mod 2)
	return ( (y & 1) != 0);
}

// Return the maximum degree over all terms
size_t Polynomial::MaxDegree( )
{
	size_t d, md = 0;
	PolyIter p_iter = mPoly.begin();
	while( p_iter != mPoly.end() )
	{
		Monomial m = *p_iter;
		m.Count();
		md = std::max( d, md );	
		++p_iter;
	}
	mPoly.empty();
	return md;
}

// Make a single mutation to this polynomial
void Polynomial::Mutate( )
{
	// Choose a random monomial
	int k = PolyMathRand::RandInt( 0, (int)mPoly.size( ) );

	// If zero - add a new monomial instead
	if( k == 0 )
	{
		Monomial m;

		// Choose a random single variable to include
		int j = PolyMathRand::RandInt( 1, (int)GAParams::NumVars() );
		m.Set( j );
		AddTerm( m );
	}
	else
	{
		// Iterate up to the k'th monomial
		PolyIter p_iter = mPoly.begin();
		while( --k > 0 ) ++p_iter;

		// Choose an exponent to mutate and change it
		int e = PolyMathRand::RandInt( 1, (int) p_iter->Size() );

		Monomial m = *p_iter;
		// Get rid of the previous version of m
		mPoly.erase( p_iter );

		// Mutate bit e
		m.Flip( e );
		size_t q = m.Count();
		if( q == 0 )
		{
			m.mCoeff = PolyMathRand::RandInt( ) % 2 == 0 ? true : false;
		}
		// If maximum support exceeded, wrap around to a single variable in the monomial
		else if( q > mMaxSupport )
		{
			m.Reset();
			m.Set( e );
		}

		AddTerm(m);	// JJM Jan08 Does nothing if already there
/*
		// If the mutated version is already in the container
		PolyIter f_iter = mPoly.find( m );
		if( f_iter != mPoly.end( ) )
		{
			// the duplicates should cancel
			mPoly.erase( f_iter );

			// If empty, make it a 0 constant
			if( mPoly.empty() )
			{
				m.Reset();	m.mCoeff = false;
				AddTerm(m);
			}
		}
		else
		{
			AddTerm( m );
		}
*/
	}
	// Remember we mutated, means we need to re-compute scores
	mMutated = true;
}


void Polynomial::SetDepGraphScore( ComplexityMatrixRow& cm_row )
{
/*************************************************
	TBD - need to design a function to use these complexity ratios

	ComplexityMatrixEntry e;	// {(p1,q1), (p2,q2), ... }
	ComplexityQuotient r;		// (p,q)

	// For this function, for each variable frequency record
	ComplexityMatrixRowIter cm_row_iter = cm_row.begin();
	while( cm_row_iter != cm_row.end() )
	{
		e = *cm_row_iter++;

		// For each ratio (p,q) in this record
		ComplexityMatrixEntryIter cm_entry_iter = e.begin();
		while( cm_entry_iter != e.end() )
		{
			r = *cm_entry_iter++;

			// TBD - must replace this Dummy formula
			double x = (double) r.p / (double) r.q;
		}
	}
******************************************************/
}

// Set the complexity score for this polynomial
void Polynomial::SetComplexityScore( double complexity_ratio )
{
	// No need to evaluate if the polynomial has not changed
	if( !mMutated ) return;

	// Transform the ratio complexity_ratio based on the complexity function
	double complexity_ratio_mapped = ComplexityFunction( complexity_ratio );

	// The ratio mapped gives us the complexity score directly
	mComplexityScore =  complexity_ratio_mapped ;

}


// A common function to evaluate a polynomial with respect to a weight matrix
double Polynomial::EvaluateWeightMatrix( WeightMatrixRow& weight_row )
{
	double p;

	// Create an nTuple u which has 1 in bit k iff at least one monomial includes variable k
	NTuple u;
	u.Reset();
	PolyIter p_iter = mPoly.begin();
	while( p_iter != mPoly.end() )
	{
		u = u | (p_iter->mTerm);
		++p_iter;
	}

	// total score
	double s = 0;

	// For each variable's probability
	size_t k = 1;
	WeightMatrixRowIter wm_iter = weight_row.begin();
	while( wm_iter != weight_row.end() && k <= GAParams::NumVars() )
	{
		// Grab the probability
		p = *wm_iter++;

		// Compute like expected value 
		s += u[k] ? p : (1-p);
		++k;
	}
	// Average over number of variables so 0.0 <= s <= 1.0
	s = s / GAParams::NumVars();
	return s;
}

// Create a Reverse Engineering score based on a row in the Reverse Eng. Weight Matrix
void Polynomial::ScoreReverseEngineeringWeights( WeightMatrixRow& weight_row )
{
	// No need to evaluate if the polynomial has not changed
	if( !mMutated ) return;

	mRevEngScore = EvaluateWeightMatrix( weight_row );
}

// Create a Biological Probability score based on row in the Biological Probability Matrix
void Polynomial::ScoreBioProbabilities( WeightMatrixRow& weight_row )
{
	// No need to evaluate if the polynomial has not changed
	if( !mMutated ) return;

	mBioProbScore = EvaluateWeightMatrix( weight_row );
}


// Return the probability that we should use this polynomial based on its composite score
double Polynomial::GetPolyProb( )
{
	if( mMutated )
	{
		// Single variable Hamming score for this polynomial
		double score = 0.0;

		// range (0.01 to 1.0)
		score += GAParams::HammingPolyWeight() * mHammingScore;

		// range (0.01  to 1.0 for term order <= max support )
		score += GAParams::ComplexityWeight()  * mComplexityScore;

		score += GAParams::RevEngWeight()  * mRevEngScore;
		score += GAParams::BioProbWeight() * mBioProbScore;
		
		mTotalScore = score;
	}
	return mTotalScore;
}

// Convert this polynomial to a string in human readable form
String Polynomial::ToString( bool with_star ) const
{
	bool first = true;
	String s;
	constPolyIter iter = mPoly.end();
	while( iter != mPoly.begin() )
	{
		--iter;
		if( !first )
		{
			s += " + ";
		}
		s += (*iter).ToString( with_star );
		first = false;
	}
	return s;
}

// Return a reference to the i'th monomial of this polynomial 1 <= i <= num terms
const Monomial& Polynomial::operator[]( size_t i )
{
	// Check for valid index
	if( i > mPoly.size() )
	{
		throw( "Polynomial::operator[] - invalid index" );
	}
	PolyIter iter = mPoly.begin();
	while( iter != mPoly.end() && i > 1 )
	{
		++iter;
		--i;
	}
	return *iter;
}

// Compare this polynomial to another, return true if they are identical
bool Polynomial::operator==( Polynomial& other )
{
	// Can't be the same if they have different number of monomials
	if( mPoly.size() != other.mPoly.size() ) return false;

	PolyIter ita = mPoly.begin();
	PolyIter itb = other.mPoly.begin();
	while( ita != mPoly.end() && itb != other.mPoly.end() )
	{
		// not identical if we encounter distinct monomials
		// This works because mPoly is a set, hence the monomials are sorted
		Monomial a = *ita++;   Monomial b = *itb++;
		if( !(a == b)  ) return false;
	}
	// If we get here the polynomials are identical
	return true;
}

// Remove the i'th monomial in this polynomial
//   and return a copy of it
Monomial Polynomial::Remove( size_t i )
{
	// Check for valid index
	if( i > mPoly.size() || i == 0 )
	{
		throw( "Polynomial::Remove - invalid index" );
	}
	PolyIter iter = mPoly.begin();
	while( iter != mPoly.end() && i > 1 )
	{
		++iter;
		--i;
	}
	Monomial mono = *iter;
	mPoly.erase( iter );

	mMutated = true;

	return mono;
}

// Support output of a Polynomial to a stream
std::ostream& operator<<( std::ostream& out, const Polynomial& p )
{
	bool first = true;
	constPolyIter iter = p.mPoly.begin();
	while( iter != p.mPoly.end() )
	{
		if( first )
		{
			out << "P = ";
			first = false;
		}
		else
		{
			out << "  + ";
		}
		out << *iter << std::endl;
		++iter;
	}
	return out;
}

