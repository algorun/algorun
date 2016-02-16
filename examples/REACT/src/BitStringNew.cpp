// BitString.cpp

#include "BoolModel.h"

// NumOnes[k] contains the number of 1's in the binary representation of the integer k
const int NumOnes[256] = 
{0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 
 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 
 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 
 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 
 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 
 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 
 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 
 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4,
 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

USHORT BitsPerWord = sizeof(ULONG)*8;

// Count the number of 1's in a single ULONG
size_t BitCount( ULONG x )
{
	size_t count = 0;
	unsigned int i;
	while( x != 0 )
	{
		// Look at the next 8 bits
		// 0 <= i < 256
		i = x & 0xFF;

		// Count how many 1's in this byte by table lookup
		count += NumOnes[i];

		// x = x / 256;
		x >>= 8;
	}
	return count;
}

// Return true if bit number b = 1 (can not be used for assignment)
bool BitString::operator[]( size_t b ) const
{
	// Ensure that index is valid
	if( b >= mN ) throw( "BitString::operator[] - index out of bounds ");

	// k is word number
	USHORT k = (USHORT)(b / BitsPerWord);

	// m is a single bit mask for the selected  bit within that word
	ULONG m = 1 << (b % BitsPerWord);

	// return true if the selected bit is set
	return ( (mWords[k] & m) != 0 );
}

// Bitwise AND operator (Performs multiply in F_2^n)
BitString BitString::operator&( const BitString& other ) const
{
	// Make an empty result of the shorter length
	BitString result( 0 );
	result.mN = std::min( mN, other.mN );

	WordArrayConstIter itr1 = mWords.begin();
	WordArrayConstIter itr2 = other.mWords.begin();
	ULONG x;
	while( itr1 != mWords.end( ) && itr2 != other.mWords.end() )
	{
		// AND this and the other, word by word
		x = (*itr1++) & (*itr2++);
		result.mWords.push_back( x );
	}
	return result;
}

// Bitwise XOR operator (Performs addition in F_2^n)
BitString BitString::operator^( const BitString& other ) const
{
	// Make an empty result of the shorter length
	BitString result( 0 );
	result.mN = std::min( mN, other.mN );

	WordArrayConstIter itr1 = mWords.begin();
	WordArrayConstIter itr2 = other.mWords.begin();
	ULONG x;
	while( itr1 != mWords.end( ) && itr2 != other.mWords.end() )
	{
		// XOR this and the other, word by word
		x = (*itr1++) ^ (*itr2++);
		result.mWords.push_back( x );
	}
	return result;
}

// Bitwise OR operator between two BitStrings
BitString BitString::operator|( const BitString& other ) const
{
	// Make an empty result of the shorter length
	BitString result( 0 );
	result.mN = std::min( mN, other.mN );

	WordArrayConstIter itr1 = mWords.begin();
	WordArrayConstIter itr2 = other.mWords.begin();
	ULONG x;
	while( itr1 != mWords.end( ) && itr2 != other.mWords.end() )
	{
		// OR this and the other, word by word
		x = (*itr1++) | (*itr2++);
		result.mWords.push_back( x );
	}
	return result;
}


// Equality test operator between two BitString's
bool BitString::operator==( const BitString& other ) const
{
	// They can not be equal if they are not of the same length
	if( this->mN != other.mN ) return false;

	// compare word by word
	//	note: this is slightly faster than XOR since no result is created
	WordArrayConstIter itr1 = mWords.begin();
	WordArrayConstIter itr2 = other.mWords.begin();
	while( itr1 != mWords.end( ) && itr2 != other.mWords.end() )
	{
		if( (*itr1++) != (*itr2++) ) return false;
	}
	// If we get here all bits are the same.
	return true;
}


// Count the number of 1 bits in this BitString
size_t BitString::Count( ) const
{
	WordArrayConstIter itr1 = mWords.begin();
	size_t count = 0;
	while( itr1 != mWords.end( )  )
	{
		// Count the number of 1's in each word
		count += BitCount( *itr1++ );
	}
	return count;
}

// Set - set bit # b of this BitString
void BitString::Set( size_t b )
{
	// Ensure that index is valid
	if( b >= mN ) throw( "BitString::Set - bit number out of bounds ");

	// k is word number
	USHORT k = (USHORT)(b / BitsPerWord);

	// m is a single bit mask for the selected  bit within that word
	ULONG m = ((ULONG) 1) << (b % BitsPerWord);
//	std::cout << "b: " << b << std::endl;
//	std::cout << "k: " << k << std::endl;
//	std::cout << "m: " << m << std::endl;

	// Set the selected bit using a bitwise OR with the single bit mask
	mWords[k] |= m;
}

// Reset - reset bit # b of this BitString
void BitString::Reset( size_t b )
{
	// Ensure that index is valid
	if( b >= mN ) throw( "BitString::Reset - bit number out of bounds ");

	// k is word number
	USHORT k = (USHORT)(b / BitsPerWord);

	// m is a single bit mask for the selected  bit within that word
	ULONG m = 1 << (b % BitsPerWord);

	// Clear the selected bit using a bitwise AND with the
	//	logical complement of the single bit mask
	mWords[k] &= ~m;
}

// Flip bit number 'b' of this BitString
void BitString::Flip( size_t b )
{
	// Ensure that index is valid
	if( b >= mN ) throw( "BitString::Flip - bit number out of bounds ");

	// k is word number
	USHORT k = (USHORT)(b / BitsPerWord);

	// m is a single bit mask for the selected bit within that word
	ULONG m = 1 << (b % BitsPerWord);

	// Flip the selected bit using a bitwise XOR with the single bit mask
	mWords[k] ^= m;
}

// Support output of a BitString to an output stream
std::ostream& operator<<( std::ostream& out, const BitString& t )
{
	int i;
	size_t j = 1;  // Initialize a bit counter

	// For each word in the BitString
	WordArrayConstIter itr1 = t.mWords.begin();
	while( itr1 != t.mWords.end( ) )
	{
		// For each bit in the word
		ULONG x = *itr1++;
		for( i = BitsPerWord; i != 0; --i )
		{
			// Output the appropriate character 1 or 0
			out << (x & 1) ? '1' : '0';
			x >>= 1;

			// Separator between each block of 8 bits
			if( j % 8 == 0 ) out << ' ';

			// Only output the actual number of bits in the BitString
			if( ++j > t.mN ) break;
		}
	}
	return out;
}

