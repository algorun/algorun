// PolyMathRand

#ifndef _VBI_POLYMATH_RAND_H_
#define _VBI_POLYMATH_RAND_H_

class PolyMathRand
{
public:
	// Constructor
	PolyMathRand()
	{
	}

	// Seed the random number generator from the current time
	static void Randomize( int seed )
	{
		if (seed==0)
		{
			seed=time(0);
		}
		unsigned int s = static_cast<unsigned int>( seed );
		srand( s );
	}

	// RandInt - return an integer in the range [0, RAND_MAX]
	static int RandInt()
	{
		return rand();
	}

	// RandInt - return an integer uniformly distributed in the range [min, max]
	static int RandInt( int min, int max )
	{
		return( min + rand( ) % (max - min + 1) );
	}

	// RandInt - return an integer 'exponentially' distributed with mean lambda
	static int RandIntExp( int lambda )
	{
		int k =  1 + lambda/2;
		int x = RandInt( 0, k );
		while( RandInt() % 3 != 0 )
		{
			x += RandInt( 0, k );
		}
		return( x );
	}

	// RandReal - return a double in interval [0,1)
	static double RandReal( )
	{
		return fabs( (1.0*rand()) /(RAND_MAX + 1ul) );
	}

	// RandReal - return a double in the interval [min, max)
	static double RandReal( const double& min, const double& max)
	{
		return( min + RandReal()*(max - min) );
	}

	// RandGauss() - Generate a pseudo-Normal random sample
	//	with mean = 0, variance = 1.0
	static double RandGauss( )
	{
		double x = 0;
		int i = 12;
		while( --i >=0 )
		{
			x += RandReal();
		}
		return (x - 6.0) / 3.6;
	}


};
#endif // _VBI_POLYMATH_RAND_H_
