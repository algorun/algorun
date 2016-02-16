/*===========================================================================*
*	NTuple.cpp																 *
*																			 *
*	Contains some of the function definitions of NTuple.h					 *
*	                                                                         *
*   Uses 1 based indexing                                                    *
*============================================================================*/

// NTuple.cpp
#include "BoolModel.h"

// Construct an NTuple from a string of 0's and 1's
NTuple::NTuple( String bit_string )
	: mTuple( GAParams::NumVars() )
{
//	std::cout << bit_string << std::endl;
	size_t i = 0;
	String::const_iterator iter = bit_string.begin();
	while( iter != bit_string.end( ) )
	{
		if( *iter == '1' )
		{
			mTuple.Set(i);
		} 
		// Ignore all but 0's and 1's
		if( *iter == '1'|| *iter == '0' )
		{
			++i;
		}
		++iter;
	}
}

// Randomize the contents of this NTuple with 50% 1's
void NTuple::Randomize( )
{
	size_t i = Size();
	while( --i >= 0 )
	{
		// Flip a coin to set each bit
		if( PolyMathRand::RandInt() % 2 )
		{
			mTuple.Set(i);
		}
	}
}


// Compute HammingDistance between two time series
size_t NTuple::HammingDistance( TimeSeries& t1, TimeSeries& t2 )
{
	size_t h = 0;
	TimeSeriesIter iter1 = t1.begin();
	TimeSeriesIter iter2 = t2.begin();
	while( iter1 != t1.end() && iter2 != t2.end() )
	{
		// Accumulate the Hamming distance for each NTuple
		h += *iter1++ - *iter2++;
	}
	return h;
}


// Support output of a NTuple to a stream
std::ostream& operator<<( std::ostream& out, const NTuple& t )
{
	size_t j;  // Initialize a bit counter
	for( j = 0; j < t.mTuple.Size(); )
	{
		// Output the appropriate character 1 or 0
		out << t.mTuple[j] ? '1' : '0';

		// Separator between each block of 8 bits
		if( ++j % 8 == 0 ) out << ' ';
	}
	return out;
}
