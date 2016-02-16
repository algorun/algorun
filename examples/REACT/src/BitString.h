// BitString.h
//
// SPECIAL VERSION THAT WORKS ONLY FOR STRINGS OF LENGTH LESS THAN 32
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
	BitString( size_t num_bits ) : mWords(0), mN( num_bits )
	{
	}

	// Copy Contruct from another BitString
	BitString( const BitString& other )
		: mWords( other.mWords ), mN( other.mN )
	{
	}

	// Construct from UNLONG and # of bits
	BitString( ULONG x, size_t n ) : mWords( x ), mN( n )
	{
	}

	// Return true if bit number i = 1 (can not be used for assignment)
	inline bool operator[]( size_t b ) const { return (((1 << b) & mWords) != 0); }

	// Bitwise AND operator (Performs * in F_2^n)
	inline BitString operator&( const BitString& other ) const { return BitString(mWords & other.mWords, mN );}

	// Bitwise XOR operator (Performs + in F_2^n)
	inline BitString operator^( const BitString& other ) const { return BitString(mWords ^ other.mWords, mN ); }

	// Bitwise OR operator
	BitString operator|( const BitString& other ) const { return BitString(mWords | other.mWords, mN ); }

	// Reset all bits of this bitstring
	inline void Reset()
	{
		mWords = 0;
	}

	// Equality test operator
	inline bool operator==( const BitString& other ) const { return mWords == other.mWords; }

	// Count the number of 1 bits in this BitString
	size_t Count( ) const { return BitCount( mWords ); }

	// Return the number of bits in this BitString
	inline size_t Size() const
	{
		return mN;
	}

	// Set - set the b'th bit of this term
	inline void Set( size_t b ) { mWords |= (1 << b); }

	// Reset - reset the b'th bit of this term
	inline void Reset( size_t b ) { mWords &= ~(1 << b); }

	// Flip the b'th bit of this term
	inline void Flip( size_t b ) { mWords ^= (1 << b); }

	// Support output of a BitString to a stream
	friend std::ostream& operator<<( std::ostream& out, const BitString& t );

private:
	// Number of bits
	size_t		mN;
	ULONG		mWords;
};

#endif // _VBI_POLYMATH_N_BITSTRING_H_
