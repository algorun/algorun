// Monomial.h
#ifndef _VBI_POLYMATH_MONOMIAL_H_
#define _VBI_POLYMATH_MONOMIAL_H_

// Class to represent a monomial term in Z2^k
class Monomial
{
public:
	// Default constructor
	Monomial( )
		: mCoeff( true )
	{
	}

	// Set the maximum number of variables to be included in any Monomial
	void static SetMaxSupport( size_t max_support )
	{
		mMaxSupport = max_support;
	}

	// Construct from a string of 0's and 1's
	// x1 is first character, x2 is second, etc.
	Monomial( String bit_string  );

	// Return Monomial[i] where 1 <= i <= Size()
	inline bool operator[]( size_t i ) const
	{
		return mTerm[i];
	}

	// Set - set bit # b of this term  1 <= b <= Size()
	inline void Set( size_t b ) { mTerm.Set( b ); }

	// Reset - reset bit # b of this term  1 <= b <= Size()
	inline void Reset( size_t b ) { mTerm.Reset( b ); }

	// Reset - reset all bits of this term  1 <= b <= Size()
	inline void Reset( ) { mTerm.Reset( ); }

	// Flip bit number 'b'  1 <= b <= Size()
	inline void Flip( size_t b ) { mTerm.Flip( b ); }

	// Size returns the number of bits in this monomial
	inline size_t Size( ) const { return mTerm.Size(); }

	// Count the number of 1 bits in this monomial
	inline size_t Count( ){ return mTerm.Count(); }

	// Evaluate - compute the value of this monomial term at N-tuple x
	// return 1 (true) if support of mTerm is contained in the support of x
	inline bool Evaluate( NTuple& x ) const
	{
		// Return 0 if coefficient is 0, else (for the boolean case)
		//  return 1 (true) if all 1 bits in the monomial term are 1 in the N-tuple
		return mCoeff ? (mTerm & x) == mTerm : 0;
	}

	// Randomize the contents of this monomial
	void Randomize( );

	// Convert this monomial to a human readable form
	String ToString( bool with_star = false ) const;

	// A 'less than' operator is required for an object placed
	// into an ordered container, in particular so that we
	// can represent a polynomial as STL set<Monomial> of terms
	// in canonical order.
	friend bool operator<( const Monomial& a, const Monomial& b );

	// Equality test operator for elimination of duplicates
	bool operator==( const Monomial& other )
	{
		if( mCoeff != other.mCoeff ) return false;

		return ( mTerm == other.mTerm );
	}


	// Support output of a Monomial to a stream
	friend std::ostream& operator<<( std::ostream& out, const Monomial& t );

// private:
	// data member to hold a monomial term
	NTuple	mTerm; 

	// coefficient for the monomial
	bool	mCoeff;

	static size_t mMaxSupport;
};

#endif // _VBI_POLYMATH_MONOMIAL_H_
