//  Evolutionary Algorithm for Discrete Dynamical System Optimization
//
//		M. Paola Vera-Licona & John J. McGee
//		Virginia Bioinformatics Institute at Virginia Tech
//
//	Main Program with test modules
//
//		Created		 9-Nov-2005
//		Last update	30-Dec-2005

#include "BoolModel.h"
#include <cstring>
#define TEST_SEED 1234

void test_BitStringNew()
{
	BitString a(12);
	a.Set(1);
	a.Set(3);
	a.Set(7);
	std::cout << "a =       " << a << std::endl;

	BitString b(12);
	b.Set(1);
	b.Set(4);
	b.Set(6);
	std::cout << "b =       " << b << std::endl;

	std::cout << "a AND B = " << (a & b) << std::endl;
	std::cout << "a OR  B = " << (a | b) << std::endl;

	std::cout << a[0] << a[1] << a[2] << a[3] << a[4] << a[5] << a[6] << a[7] << std::endl;
}

// Test the NTuple class
void test_NTuple()
{
	GAParams::SetNumVars( 8 );
	NTuple a( "1001 0110" );
	NTuple b( "1111 0000" );

	// Test binary AND (which is multiply in F2^n)
	NTuple c = a & b;

	// Test binary XOR (which is addition in F2^n)
	NTuple d = a ^ b;

	std::cout << "a =       " << a << std::endl;
	std::cout << "b =       " << b << std::endl;
	std::cout << "a AND b = " << c << std::endl;
	std::cout << "a XOR b = " << d << std::endl;

	std::cout << "# of 1's in a = " << (int) a.Count() << std::endl;
	a.Reset(3);
	std::cout << "reset bit a_3 = " << a << std::endl;

	a.Flip(5);
	std::cout << "flip  bit a_5 = " << a << std::endl;

	// Test Hamming Distance using the subtraction operator
	std::cout << " a - b  =       " << (int)(a - b) << std::endl;
}

// Test the Monomial class
void test_Monomial()
{
	GAParams::SetNumVars( 8 );

	Monomial a("1001 0110" );
	Monomial b("1111 0000" );
	Monomial d("0000 0000" );

	NTuple c( "1011 0110" );

	d.mCoeff = 1;
	std::cout << d.Evaluate( c ) << std::endl;

	d.mCoeff = 0;
	std::cout << d.Evaluate( c ) << std::endl;

	std::cout << a.Evaluate( c ) << std::endl;
	a.mCoeff = false;

	std::cout << a.Evaluate( c ) << std::endl;
	a.mCoeff = true;
	
	std::cout << "NTuple   representation " << a.mTerm << std::endl;
	std::cout << "Monomial representation " << a.ToString(true) << std::endl;
	std::cout << a.Evaluate( c ) << std::endl;
}

//Test Polynomial class
void test_Polynomial()
{
	GAParams::SetNumVars( 8 );

	Monomial a("1001 0110" );
	Monomial b("1111 0000" );
	Monomial c("0000 0000" );

	// p(x) = 0;
	Polynomial p;
	p.AddTerm(c);
	std::cout << p.ToString( true ) << std::endl;
	std::cout << "  has max degree = " << (int) p.MaxDegree() << std::endl;

	// p(x) = a  = x1*x4*x6*x7
	p.AddTerm(a);
	// p(x) = a + b = x1*x4*x6*x7 + x1*x2*x3*x4
	p.AddTerm(b);
	// p(x) = a + b + c = x1*x4*x6*x7 + x1*x2*x3*x4 + 1

	std::cout << p.ToString( true ) << std::endl;
	std::cout << "  has max degree = " << (int) p.MaxDegree() << std::endl;

	NTuple d("1101 0110" );

	std::cout << p.Evaluate(d) << std::endl;
}

void test_BitString()
{
	try
	{
		BitString x(30);
		BitString y(30);

		x.Set(3);
		x.Set(7);
		x.Set(14);

		y.Set(3);
		y.Set(5);
		y.Set(12);

	// Test out of bounds bit number handling
	//	y.Set(32);

		std::cout << "X     = " << x << std::endl;
		std::cout << "Y     = " << y << std::endl;

		// Test bit-wise AND
		// BitString z = x & y;

		// std::cout << "X & Y = " << z << std::endl;

	// Test bit-wise OR
	//	BitString z = x | y;

	//	std::cout << "X | Y = " << z << std::endl;

	// Test bit-wise XOR
		BitString z = x^y;

		std::cout << "X ^ Y = " << z << std::endl;


	// Test bit-wise Count
		size_t c = z.Count();

		std::cout << "Count X | Y = " << (int) c << std::endl;

	}
	// Catch any simple string error messages
	catch( char* s )
	{
		std::cout << s << std::endl;
	}

}

// Test the DynSysModel class
void test_DynSysModel()
{
	// Make a 4 variable model
	GAParams::SetNumVars( 4 );

	DynSysModel m;

	// Create monomial x1, x2, x3, x4, x1*x2, x3*x4, x1*x2*x3*x4

	Monomial c0(   "0000" );	c0.mCoeff = false;
	Monomial c1(   "0000" );	c1.mCoeff = true;
	Monomial x4(   "0001" );
	Monomial x3(   "0010" );
	Monomial x2(   "0100" );
	Monomial x1(   "1000" );
	Monomial x34(  "0011" );
	Monomial x12(  "1100" );
	Monomial x14(  "1001" );
	Monomial x1234("1111" );

	Polynomial f;
	f.AddTerm( c0 );
	std::cout << "f = " << f.ToString( true ) << std::endl;

	Polynomial g;
	g.AddTerm( c1 );
	std::cout << "g = " << g.ToString( true ) << std::endl;

	// f1(x) = x1 + x2 + x4;
	Polynomial f1;
	f1.AddTerm( x1 );	f1.AddTerm( x2 );	f1.AddTerm( x4 );
	std::cout << "f1 = " << f1.ToString( true ) << std::endl;

	Polynomial::mMaxSupport = 2;
	m.SetFunction( 1, f );
	m.SetFunction( 2, g );
	m.SetFunction( 3, f );
	m.SetFunction( 4, f1 );

	ComplexityMatrix cmplx_mat;
	ComplexityMatrixRow row;
	cmplx_mat.assign( 4, row );

	m.SetPolyComplexities( );
	double s;
	s = m[1].mComplexityScore;
	s = m[2].mComplexityScore;
	s = m[3].mComplexityScore;
	s = m[4].mComplexityScore;


/*
	// f2(x) = x1*x2*x3*x4;
	Polynomial f2;
	f2.AddTerm( x1234 );
	std::cout << "f2 = " << f2.ToString( true ) << std::endl;

	// f3(x) = x1*x2 + x3*x4;
	Polynomial f3;
	f3.AddTerm( x12 );
	f3.AddTerm( x34 );
	std::cout << "f3 = " << f3.ToString( true ) << std::endl;

	// f3'(x) = x1*x4;
	Polynomial f3p;
	f3p.AddTerm( x14 );
	std::cout << "f3p = " << f3p.ToString( true ) << std::endl;

	// f4(x) = x3 + x4;
	Polynomial f4;
	f4.AddTerm( x3 );	f4.AddTerm( x4 );
	std::cout << "f4 = " << f4.ToString( true ) << std::endl;

	// Assign the functions to the model
	m.SetFunction( 1, f1 ); 	m.SetFunction( 2, f2 );
	m.SetFunction( 3, f3p ); 	m.SetFunction( 4, f4 );
*/

	TimeSeries t2;
//	t2.push_back( NTuple("1101" ) );
//	t2.push_back( NTuple("1011" ) );
	t2.push_back( NTuple("1100" ) );
	t2.push_back( NTuple("0010" ) );
	t2.push_back( NTuple("0001" ) );
	t2.push_back( NTuple("1001" ) );
	t2.push_back( NTuple("0001" ) );

	// Better to pass a reference to the time series for the result
	// TimeSeries t3 = m.Iterate( NTuple( "1111" ), 6 );

	// Test iteration for only the k'th variable
	size_t k = 3;
	TimeSeries t4;
	size_t h = m.Iterate( t2, k, t4 );

	TimeSeriesIter iter = t4.begin();
	while( iter != t4.end() )
	{
		std::cout << *iter++ << std::endl;
	}
}

void test_polymath_rand()
{
	PolyMathRand::Randomize( 0 );

	int i = 10;
	while( --i >= 0 )
	{
		std::cout <<	PolyMathRand::RandInt( 10, 99 )
			<< "\t" <<	PolyMathRand::RandReal( ) << std::endl;
	}
}

void test_rand_poly()
{
	// Establish the problems size
	GAParams::SetNumVars( 4 );

	// Establish the maximum support for any monomial
	Monomial::SetMaxSupport( 3 );

	size_t max_terms = 8;
	Polynomial p;

	int i;
	for( i = 25; i > 0; --i)
	{
		p.GenerateRandomPolynomial( max_terms );

		std::cout << "p = " << p.ToString( true ) << std::endl;
	}

}

void test_rand_methods()
{
	int i = 1000;
	int j;
	int hist[50] = {0};
	while( --i >= 0 )
	{
		 j =  PolyMathRand::RandIntExp( 5 );
		 if( j < 50 )
		 {
			 ++hist[j];
		 }
	}
	for( i=0; i < 50; ++i )
	{
		std::cout << i << "\t";
		for(j=0; j < hist[i]; ++j) std::cout << "*";
		std::cout << std::endl;
	}
}

void test_poly_mutate()
{
	GAParams::SetNumVars( 8 );

	Monomial a("1000 0000" );
	Monomial c("0000 0000" );
	c.mCoeff = true;

	// p(x) = 1;
	Polynomial p;
	p.AddTerm(c);
	std::cout << p.ToString( true ) << std::endl;

	p.AddTerm(a);
	std::cout << p.ToString( true ) << std::endl;

	// Attempt to add a duplicate term
	// a.Reset(1);
	// p.AddTerm(a);

	PolyIter p_iter = p.mPoly.find( c );
	if( p_iter != p.mPoly.end( ) )
	{
		p.mPoly.erase( p_iter );
	}

	std::cout << p.ToString( true ) << std::endl;
    
}

void test_all( )
{
	try
	{
		// test_rand_methods();
		// Test the NTuple class
		// test_NTuple();

		// Test the Monomial class
		// test_Monomial();

		// Test the Polynomial()
		// test_Polynomial();

		// Test the BitString class
		//test_BitString();

		// Test the DynSysModel class
		// test_DynSysModel();

		// test_polymath_rand();

		// test_rand_poly();

		// Establish the problems size


		
		GAParams::SetNumVars( 8 );
		GAModel ga;
		int num_models = 100;
		ga.FillGenePool( num_models );

		std::ofstream gp_dump("\\JJM\\Fall2005\\EA1_0\\test\\genepooldump.txt");
		ga.DumpGenePool( gp_dump );
		gp_dump.close();

		FileManager fm;
		String control_file_path = "\\JJM\\Fall2005\\EA1_0\\test\\fileman.txt";
		fm.ReadControlFile( control_file_path );

		TimeSeriesVector wt_series;
		fm.ReadWildTimeSeries( wt_series );

		TimeSeriesVectorSet ko_series;
		fm.ReadKnockOutTimeSeries( ko_series );

		TimeSeriesVectorSetIter ko_iter = ko_series.begin();
		while( ko_iter != ko_series.end() )
		{
			std::cout << "KO: " << ko_iter->first << std::endl;
			TimeSeriesIter ts_iter = ko_iter->second.begin();
			while( ts_iter != ko_iter->second.end() )
			{
				std::cout << "\t" << *ts_iter << std::endl;
				++ts_iter;
			}
			++ko_iter;
		}

		WeightMatrix weight_matrix;
		fm.ReadReverseEngWeightMatrix( weight_matrix );
		WeightMatrixIter wm_iter = weight_matrix.begin();
		char buf[16];
		while( wm_iter != weight_matrix.end() )
		{
			WeightMatrixRowIter wm_row_iter = wm_iter->begin();
			while( wm_row_iter != wm_iter->end() )
			{
				sprintf( buf, "%7.2f", *wm_row_iter );
				std::cout << buf;
				++wm_row_iter;
			}
			std::cout << std::endl;
			++wm_iter;
		}

		// Test the ability to read models
		DynSysModelVector models;
		fm.ReadDiscreteModels( models );

	}
	catch( const char* msg )
	{
		std::cout << msg << std::endl;
	}
	catch( const String& msg )
	{
		std::cout << msg << std::endl;
	}

}

// define the main program
int main( int num_args, char* cmd_line_args[] )
{
//	test_BitStringNew();
//	test_NTuple();
//	test_Monomial();
//	test_poly_mutate();

//	test_polymath_rand();


	try
	{
		String control_file_path = "GaControlFile.txt";
		char *best_models_filename = (char*) "BestModels.txt";
		int seed = time(0);
		if (num_args < 2)
		{
			std::cout << "Use React with the parameters:" << std::endl;
			std::cout << "./React controlfilepath (usually fileman.txt) outputfilename [--test]" << std::endl; 
			std::cout << "Note: Appending --test will run React with a pre-defined random seed, use it ONLY for testing purposes, ignore otherwise." << std::endl; 
			return 0;
		}
		if( num_args >= 2 )
		{
			control_file_path = String( cmd_line_args[1] );
		}
		if (num_args >= 3)
		{
			best_models_filename = cmd_line_args[2];
		}
		if (num_args >= 4 && !strcmp(cmd_line_args[3],"--test"))
		{
			seed=TEST_SEED;
			std::cout << "Running React in test configuration. DO NOT USE RESULTS FOR ANYTHING OTHER THAN TESTING PURPOSES." << std::endl;
		}
		std::cout << "Opening: " << control_file_path << std::endl;

		// Initialize the run based on the control file
		GAModel ga;
		ga.InitializeGA( control_file_path );

		// Run the GA until convergence
		ga.Randomize( seed );
		ga.Run( best_models_filename );

	}
	catch( const char* msg )
	{
		std::cout << msg << std::endl;
	}
	catch( const String& msg )
	{
		std::cout << msg << std::endl;
	}

	// All ok
	return 0;

}

// End
