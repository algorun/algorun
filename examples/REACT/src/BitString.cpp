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


// Support output of a BitString to an output stream
std::ostream& operator<<( std::ostream& out, const BitString& t )
{
	int i;
	size_t j = 1;  // Initialize a bit counter

	// For each bit in the word
	ULONG x = t.mWords;
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
	return out;
}

