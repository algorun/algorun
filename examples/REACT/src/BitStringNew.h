// BitString.h
//
// Uses 0-based indexing

#ifndef _VBI_POLYMATH_N_BITSTRING_H_
#define _VBI_POLYMATH_N_BITSTRING_H_

// TO DO

// typedef std::vector<ULONG> WordArray;
// typedef std::vector<ULONG>::iterator WordArrayIter;

extern USHORT BitsPerWord;

// NumOnes[k] contains the number of 1's in the binary representation of the integer k
extern const int NumOnes[];

// Count the number of 1's in a single ULONG
size_t BitCount( ULONG x );

class BitString
{
public:
	// Default constructor ( all zeros bitstring of num_bits )
	BitString( size_t num_bits )
	{
		mN = num_bits;

		if( num_bits > 0 )
		{
			// Compute the number of words we need to hold mN bits
			USHORT k = (USHORT) ( 1 + ( (num_bits - 1) / BitsPerWord ));

			// Insert k words of value 0
			mWords.assign( k, 0 );
		}
	}

	// Copy Contruct from another BitString
	BitString( const BitString& other )
		: mWords( other.mWords ), mN( other.mN )
	{
	}

	// Return true if bit number i = 1 (can not be used for assignment)
	bool operator[]( size_t i ) const;

	// Bitwise AND operator (Performs * in F_2^n)
	BitString operator&( const BitString& other ) const;

	// Bitwise XOR operator (Performs + in F_2^n)
	BitString operator^( const BitString& other ) const;

	// Bitwise OR operator
	BitString operator|( const BitString& other ) const;

	// Reset all bits of this bitstring
	inline void Reset()
	{
		mWords.clear();
		// Compute the number of words we need to hold mN bits
		size_t k = 1 + ( (mN - 1) / BitsPerWord );
		mWords.assign( k, 0 );
	}

	// Equality test operator
	bool operator==( const BitString& other ) const;

	// Count the number of 1 bits in this BitString
	size_t Count( ) const;

	// Return the number of bits in this BitString
	inline size_t Size() const
	{
		return mN;
	}

	// Set - set the b'th bit of this term
	void Set( size_t b );

	// Reset - reset the b'th bit of this term
	void Reset( size_t b );

	// Flip the b'th bit of this term
	void Flip( size_t b );

	// Support output of a BitString to a stream
	friend std::ostream& operator<<( std::ostream& out, const BitString& t );

private:
	// Number of bits
	size_t		mN;
	WordArray	mWords;
};

#endif // _VBI_POLYMATH_N_BITSTRING_H_
