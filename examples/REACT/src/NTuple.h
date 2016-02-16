// NTuple.h

#ifndef _VBI_POLYMATH_N_TUPLE_H_
#define _VBI_POLYMATH_N_TUPLE_H_

class NTuple;

typedef std::vector<NTuple> TimeSeries;
typedef TimeSeries::iterator TimeSeriesIter;

typedef std::vector<TimeSeries> TimeSeriesVector;
typedef TimeSeriesVector::iterator TimeSeriesVectorIter;

typedef std::multimap<int, TimeSeries> TimeSeriesVectorSet;
typedef TimeSeriesVectorSet::iterator TimeSeriesVectorSetIter;

// Class to hold an nTuple of assignable length
//  this class puts a thin cover over BitString
//		uses 1-based indexing, and converts to 0-based indexing for BitString
class NTuple
{
public:
	// Default constructor ( all zeros tupple of num_bits )
	NTuple( )
		: mTuple( GAParams::NumVars() )
	{
	}

	// Contruct from a TupleType
	NTuple( const TupleType& tuple )
		: mTuple( tuple )
	{
	}

	// Construct from a string of 0's and 1's
	NTuple( String bit_string );

	// Return mTuple[i] where 1 <= i <= Size()
	inline bool operator[]( size_t i ) const
	{
		return mTuple[i-1];
	}

	// Bitwise AND operator
	inline NTuple operator&( const NTuple& other ) const
	{
		return NTuple( mTuple & other.mTuple );
	}

	// Bitwise OR operator
	inline NTuple operator|( const NTuple& other ) const
	{
		return NTuple( mTuple | other.mTuple );
	}

	// Reset all bits of this tupple
	inline void Reset()
	{
		mTuple.Reset();
	}

	// Bitwise XOR operator
	inline NTuple operator^( const NTuple& other ) const
	{
		return NTuple( mTuple ^ other.mTuple );
	}

	// Equality test operator
	inline bool operator==( const NTuple& other ) const
	{
		return ( mTuple == other.mTuple );
	}

	// Count the number of 1 bits in mTuple
	inline size_t Count( ) const
	{
		// TBD - could optimize this with larger count table
		return mTuple.Count();
	}

	// Return the number of variable in this nTuple
	inline size_t Size() const
	{
		return mTuple.Size();
	}

	// Set - set bit # b of this NTuple 1 <= b <= Size()
	inline void Set( size_t b ) { mTuple.Set( b - 1 ); }

	// Reset - reset bit # b of this NTuple  1 <= b <= Size()
	inline void Reset( size_t b ) { mTuple.Reset( b - 1 ); }

	// Assign the value s to bit # b of this NTuple  1 <= b <= Size()
	inline void Assign( size_t b, bool s )
	{
		s ? mTuple.Set( b - 1 ) : mTuple.Reset( b - 1 );
	}

	// Flip bit number 'b' of this NTuple  1 <= b <= Size()
	inline void Flip( size_t b ) { mTuple.Flip( b - 1); }

	// Subtraction operator gives the Hamming distance
	inline size_t operator-( const NTuple& other ) const
	{
		// Hamming distance counts the number of 1's in the XOR of the two NTuples
		return NTuple( mTuple ^ other.mTuple ).Count();
	}

	// Randomize the contents of this NTuple with 50% ones
	void Randomize( );

	// HammingDistance computed using the subtraction operator
	inline size_t HammingDistance( const NTuple& other ) const { *this - other; };

	// Support output of a NTuple to a stream
	friend std::ostream& operator<<( std::ostream& out, const NTuple& t );

	// Compute HammingDistance between two time series
	static size_t HammingDistance( TimeSeries& t1, TimeSeries& t2 );

// private:
	BitString	mTuple;
};


#endif // _VBI_POLYMATH_N_TUPLE_H_
