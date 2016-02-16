// Monomial.cpp

#include "BoolModel.h"

size_t Monomial::mMaxSupport = 1; 

// Construct from a string of 0's and 1's
Monomial::Monomial( String bit_string )
{
	size_t i = 1;
	String::const_iterator iter = bit_string.begin();
	while( iter != bit_string.end( ) && i < mTerm.Size() )
	{
		if( *iter == '1' )
		{
			mTerm.Set(i);
		}
		// ignore blanks
		if( *iter != ' ' )
		{
			++i;
		}
		iter++;
	}
}

// Convert this monomial to a human readable form
//	e.g. x1*x2*x5 + x3*x4 + 1
String Monomial::ToString( bool with_star ) const
{
	// Allocate 6 characters to represent the name of one variable i.e x9999
	char buf[6];
	if( mTerm.Size() > 9999 )
	{
		throw( "Monomial::ToString - more than 9999 variables" );
	}

	// The string 's' will contain the full character representation
	//   collection of all the variable names in the monomial
	String s;
	size_t i = 1;
	bool all_zeros = true;

	// Handle case of all zero's which behaves as constant 1 or 0
	if( mTerm.mTuple.Count() == 0)
	{
		s = mCoeff ? "1" : "0";
	}
	else
	{
		while( i <= mTerm.Size() )
		{
			if( mTerm[i] )
			{
				// Don't output a 'times' until after the first variable
				if( with_star && !all_zeros )
				{
					s += "*";
				}
				// Construct the variable name "x9999"
				sprintf( buf, "x%zd", i );

				// append this variable name to the string 's'
				s += buf;
				all_zeros = false;
			}
			++i;
		}
	}
	return s;
}

// Randomize the contents of this monomial
void Monomial::Randomize( )
{
	size_t k;
	// Include up to mMaxSupport variables in this monomial
	int i = (int) mMaxSupport;
	while( --i >= 0 )
	{
		// With 50% probability
		if( PolyMathRand::RandInt() % 2 )
		{
			// Choose a random variable to include
			k = PolyMathRand::RandInt( 1, (int) GAParams::NumVars() );
			// but don't count duplicates
			if( !mTerm[k] )
			{
				mTerm.Set( k );
			}
			else
			{
				++i;
			}
		}
	}
}

// 'less than' operator required for an object placed
// into an ordered container, in particular so that we
// can represent a polynomial as STL set<Monomial> of terms
// in canonical order. Must return false for equal monomials
// to prevent duplication in a set (enforces reverse lex order)
bool operator<( const Monomial& a, const Monomial& b )
{
	int k = (int) a.Size();
	int i = 1;
  	while( (a.mTerm[i]) == (b.mTerm[i]) )
	{
		if( ++i >= k ) break;
	}
	// if all but last exponents of a and b are the same, check the last one
	if(( i == k ) && ( a.mTerm[k] == b.mTerm[k] ))
		return false;
	return b.mTerm[i];
}

// Support output of a Monomial to a stream
std::ostream& operator<<( std::ostream& out, const Monomial& t )
{
	out << t.mTerm;
	return out;
}

// End Monomial.cpp

