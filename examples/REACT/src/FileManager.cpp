// FileManager.cpp

#include "BoolModel.h"

// Extract the next token or delimiter from the begining of string s
String FileManager::next_token( String& s, const String& delim )
{
	// Find the index of first delimiter
	size_t tp = s.find_first_of( delim );

	// If none found, return all of s
	if( tp == String::npos )
	{
		tp = s.length();
	}
	// If the first character is a delimiter, return it
	if( tp == 0 && s.length() > 0 ) tp = 1;

	// Return the substring between the beginning and the delimiter
	String token = s.substr(0,tp);

	// Remove the same substring from the beginning of s
	s = s.substr(tp);

	return token;
}

String FileManager::GetLine( std::ifstream& in_file )
{
	String l;
	char ch = (char)in_file.get();
	while( !(iscntrl(ch) && ch != '\t') && !in_file.eof() )
	{
		l.push_back( ch );
		ch = (char) in_file.get();
	}
	return l;
}


// Read in and parse the text file that controls a run of the G.A.
bool FileManager::ReadControlFile( String& control_file_path )
{
	// Extract path from control file, if any
	size_t pe = control_file_path.rfind( '\\' );
	if( pe != String::npos )
	{
		mPath = control_file_path.substr(0, pe+1);
	}
	std::ifstream cf;
	cf.open( control_file_path.c_str() );
	if( !cf.is_open() )
	{
		std::cout << "ERROR - Control File Failed to Open" << std::endl;
		return false;
	}
	String block;
	String element;
	String token;
	String delims = "=(){}\",";

	while( !cf.eof() )
	{
		// Read in a block by reading and contatinating elements until a ';' is found
		//  NOTE: this blocking removes all white space by use of operator>>
		element = ""; block = "";
		while( element.find(';') == String::npos && cf.good() )
		{
			cf >> element;
			block += element;
		}
		// If we really have a block
		if( element.find(';') != String::npos )
		{
			// Get the keyword
			token = next_token(  block, delims );
			if( token == "N" )
			{
				// =
				token = next_token( block, delims );
				if( token != "=" ) continue;
				// num_vars
				token = next_token( block, delims );
				mNumVars = atoi( token.c_str() );
			}
			if( token == "WT" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) continue;

				// Check for '{'
				token = next_token(  block, delims );
				if( token != "{" ) continue;

				// Until we reach '}'
				while( token != "}" )
				{
					// "
					token = next_token(  block, delims );
					if( token != "\"" ) break;

					// filename
					token = next_token(  block, delims );
					if( token.length() > 1 )
					{
						wt_file_list.push_back( token );
					}
					// eat the next two tokens " and , or " and ;
					token = next_token(  block, delims );
					token = next_token(  block, delims );
				}
			}
			else if( token == "KO" )
			{
				// (1, "m_out.txt"),
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) continue;

				// Check for '{'
				token = next_token(  block, delims );
				if( token != "{" ) continue;

				// Until we reach '}'
				while( token != "}" )
				{
					// (
					token = next_token(  block, delims );
					if( token != "(" ) break;

					// knockout variable number
					token = next_token(  block, delims );
					int k = atoi(token.c_str());

					// , "
					token = next_token(  block, delims );
					if( token != "," ) break;
					token = next_token(  block, delims );
					if( token != "\"" ) break;

					// filename
					token = next_token(  block, delims );
					if( token.length() > 1 )
					{
						ko_file_list.push_back( KOFileSpec(k,token) );
					}
					// eat the next three tokens "), or ")};
					token = next_token(  block, delims );
					token = next_token(  block, delims );
					token = next_token(  block, delims );
				}
			}
			else if( token == "BIO" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) break;
				// Check for {
				token = next_token(  block, delims );
				if( token != "{" ) break;
				// Check for "
				token = next_token(  block, delims );
				if( token == "}" ) continue;
				if( token != "\"" ) break;
				// Filename
				token = next_token(  block, delims );
				if( token.length() > 1 )
				{
					mBioFile = token;
				}
				// eat the next two tokens "}
				token = next_token(  block, delims );
				token = next_token(  block, delims );
			}
			else if( token == "CMPLX" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) break;
				// Check for {
				token = next_token(  block, delims );
				if( token != "{" ) break;
				// Check for "
				token = next_token(  block, delims );
				if( token == "}" ) continue;
				if( token != "\"" ) break;
				// Filename
				token = next_token(  block, delims );
				if( token.length() > 1 )
				{
					mDepGraphFile = token;
				}
				// eat the next two tokens "}
				token = next_token(  block, delims );
				token = next_token(  block, delims );
			}
			else if( token == "REV" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) break;
				// Check for {
				token = next_token(  block, delims );
				if( token != "{" ) break;
				// Check for "
				token = next_token(  block, delims );
				if( token == "}" ) continue;
				if( token != "\"" ) break;
				// Filename
				token = next_token(  block, delims );
				if( token.length() > 1 )
				{
					mRevFile = token;
				}
				// eat the next two tokens "}
				token = next_token(  block, delims );
				token = next_token(  block, delims );
			}
			else if( token == "MODEL" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) continue;

				// Check for '{'
				token = next_token(  block, delims );
				if( token != "{" ) continue;

				// Until we reach '}'
				while( token != "}" )
				{
					// "
					token = next_token(  block, delims );
					if( token != "\"" ) break;

					// filename
					token = next_token(  block, delims );
					if( token.length() > 1 )
					{
						model_file_list.push_back( token );
					}
					// eat the next two tokens " and , or " and ;
					token = next_token(  block, delims );
					token = next_token(  block, delims );
				}
			}
			else if( token == "PARAMS" )
			{
				// Check for '='
				token = next_token(  block, delims );
				if( token != "=" ) break;
				// Check for {
				token = next_token(  block, delims );
				if( token != "{" ) break;
				// Check for "
				token = next_token(  block, delims );
				if( token == "}" ) continue;
				if( token != "\"" ) break;
				// Filename
				token = next_token(  block, delims );
				if( token.length() > 1 )
				{
					mParamFile = token;
				}
				// eat the next two tokens "}
				token = next_token(  block, delims );
				token = next_token(  block, delims );
			}
		}
	}
	cf.close();
	return true;
}

// Read in a time series from the given file, optionally verify knockout variable kov
bool FileManager::ReadTimeSeries( String& file_path, TimeSeries& ts, size_t kov )
{
//	std::cout << "file: " << file_path << std::endl;
	std::ifstream in_file;
	in_file.open( file_path.c_str() );
	if( !in_file.good() )
	{
		throw( String("FileManager::ReadTimeSeries - unable to open file ") + file_path);
	}

	String in_line;
	char ch;
	size_t num_digits = 0;
	size_t prev_num_digits = 0;

	while( !in_file.eof() )
	{
		in_file.get( ch );
		if( ch == '\n' ) 
		{
			if( num_digits > 0 )
			{
//				std::cout << "line: " << in_line << std::endl;
				NTuple t( in_line  );
//				std::cout << "tuple: " << t << std::endl;
				if( t.Size() != GAParams::NumVars() )
				{
					throw( "FileManager::ReadTimeSeries - time series n-tuple length not equal to number of variables" );
				}
				if( kov != 0 )
				{
					if( t[kov] )
					{
//						std::cout << t[kov] << std::endl;
//						std::cout << kov << std::endl;
						t.Reset( kov );
						std::cout << "FileManager::ReadTimeSeries - knockout variable not zero in time series" << std::endl;
						std::cout << "  data corrected - processing will continue" << std::endl;
					}
				}
				// Save the n-tuple in the time series
				ts.push_back( t );

				// Clear for next line
				num_digits = 0;
				in_line = "";
			}
		}
		else
		{
			in_line.push_back( ch );
			// Count only digits in the field
			if( isdigit( ch ) )
			{
				if( mFieldDigits.find( ch ) == String::npos )
				{
					throw( "FileManager::ReadTimeSeries - invalid digit in time series" );
				}
				++num_digits;
			}
		}
	}
	in_file.close();
	return true;
}

// Read in all of the wild-type time-series, one for each file in wt_file_list
bool FileManager::ReadWildTimeSeries( TimeSeriesVector& wt_series )
{
	wt_series.clear();

	mMaxWtTsLength = 0;
	// For each file in wt_file_list
	StringVectorIter iter = wt_file_list.begin();
	while( iter != wt_file_list.end() )
	{
		String fullpath = mPath + *iter;
		TimeSeries ts;
		if( ReadTimeSeries( fullpath, ts ) )
		{
			if( ts.size() > mMaxWtTsLength ) mMaxWtTsLength = ts.size();
			wt_series.push_back( ts );
		}
		++iter;
	}
	return true;
}

// Read in all of the knockout time series, one set for each variable
bool FileManager::ReadKnockOutTimeSeries( TimeSeriesVectorSet& ko_series_set )
{
	mMaxKoTsLength = 0;
	// ko_series_set is a multimap with key = knockout variable number
	//	and data = a time series for that variable
	KOFileSpecListIter iter = ko_file_list.begin();
	while( iter != ko_file_list.end() )
	{
		// Build the path from the filename
		String fullpath = mPath + iter->mFilename;

		// Read in the time series from that file
		TimeSeries ts;
		if( ReadTimeSeries( fullpath, ts, iter->mK ) )
		{
			if( ts.size() > mMaxKoTsLength ) mMaxKoTsLength = ts.size();
			// Insert the time series into the map, keyed by knockout variable number
			ko_series_set.insert(TimeSeriesVectorSet::value_type( iter->mK, ts ));
		}
		++iter;
	}
	return true;
}


// Process a single row of the Reverse Engineering Matrix
void FileManager::ProcessRow( WeightMatrixRow& row )
{
	// Divide thru by the last entry and then drop it
	int	n = (int) floor(row.back());
	WeightMatrixRowIter row_iter = row.begin();
	while( row_iter != row.end() )
	{
		if( n > 0 && *row_iter <= n )
		{
			*row_iter /= n;
		}
		else
		{
			if( n == 0 && *row_iter != 0 )
			{
				throw( "FileManager::ReadReverseEngWeightMatrix - nonzero entry with total count = 0" );
			}
			else if ( n < *row_iter )
			{
				throw( "FileManager::ReadReverseEngWeightMatrix - entry greater than total count" );
			}
		}
		++row_iter;
	}
	// remove the last entry and then save the row
	row.pop_back( );
}

// Read in a file containing frequence of use counts for each variable in each function
bool FileManager::ReadReverseEngWeightMatrix( WeightMatrix& weight_matrix )
{
	// If no file name do not complain
	if( mRevFile.length() == 0 ) return true;

	String file_path = mPath + mRevFile;
	std::ifstream in_file;
	in_file.open( file_path.c_str() );
	if( !in_file.good() )
	{
		throw( String("FileManager::ReadReverseEngWeightMatrix - unable to open file") + file_path);
	}

	char ch;
	int n;
	double x;
	int var_num;
	String ns; // to collect a string of digits representing a number

	// fill the weight matrix with mNumVar rows
	WeightMatrixRow row;
	for( n = (int) GAParams::NumVars(); n > 0; --n )
	{
		weight_matrix.push_back( row );
	}

	bool in_num = false;
	bool in_row = false;
	bool still_blank = true;
	bool first_num = true;
	while( !in_file.eof() )
	{
		in_file.get( ch );
		if( in_row )
		{
			if( in_num )
			{
				if( isdigit( ch ) || ch == '.' )
				{
					// accumulate digits
					ns.push_back( ch );
				}
				else
				{
					if( ns.length() > 0 )
					{
						if( first_num )
						{
							// The first number after 'F' identifies the variable
							var_num = atoi( ns.c_str());
							if( var_num > (int)weight_matrix.size() )
							{
								throw( "FileManager::ReadReverseEngWeightMatrix - invalid variable number after F" );
							}
							first_num = false;
						}
						else
						{
							x = atof( ns.c_str() );
							// Append the number to the current row
							row.push_back( x );
						}
					}
					if( ch == '\n' )
					{
						if( row.size() !=  GAParams::NumVars() + 1 )
						{
							throw( "FileManager::ReadReverseEngWeightMatrix - row length does not match number of variables");
						}
						// Normalize the values using the last column
						ProcessRow( row );
						weight_matrix[ var_num - 1 ] = row;
						row.clear();
						still_blank = true; first_num = true;
					}
					// Leave the 'in number' state
					in_num = false;
				}
			}
			else // we are outside of a number
			{
				if( isdigit( ch ) || ch == '.' )
				{
					// accumulate the first digit
					ns.clear();
					if( ch == '.' ) ns.push_back( '0' );
					ns.push_back( ch );
					in_num = true;
				}
				else
				{
					if( ch == '\n' )
					{
						if( row.size() > 0 )
						{
							// Normalize the values using the last column
							if( row.size() !=  GAParams::NumVars() + 1 )
							{
								throw( "FileManager::ReadReverseEngWeightMatrix - row length does not match number of variables");
							}
							ProcessRow( row );
							weight_matrix[ var_num - 1 ] = row;
						}
						row.clear();
						still_blank = true; first_num = true;
					}
				}
			}
		}
		else
		{
			if( ch == 'F' && still_blank )
			{
				in_row = true;
			}
			if( isalnum( ch ))  still_blank = false;
			if( ch == '\n' )
			{
				still_blank = true; first_num = true;
			}
		}
	}
	in_file.close();
	return true;
}


bool FileManager::ReadBioDependencyMatrix( WeightMatrix& prob_matrix )
{
	// If no file name do not complain
	if( mBioFile.length() == 0 ) return true;

	String file_path = mPath + mBioFile;
	std::ifstream in_file;
	in_file.open( file_path.c_str() );
	if( !in_file.good() )
	{
		throw( String("FileManager::ReadBioDependencyMatrix - unable to open file") + file_path);
	}

	char ch;
	int n;
	int var_num;
	String ns; // to collect a string of digits representing a number

	// fill the weight matrix with mNumVar rows
	WeightMatrixRow row;
	for( n = (int) GAParams::NumVars(); n > 0; --n )
	{
		prob_matrix.push_back( row );
	}

	bool in_num = false;
	bool in_row = false;
	bool still_blank = true;
	bool first_num = true;
	double x;
	while( !in_file.eof() )
	{
		in_file.get( ch );
		if( in_row )
		{
			if( in_num )
			{
				if( isdigit( ch ) || ch == '.' )
				{
					// accumulate digits
					ns.push_back( ch );
				}
				else
				{
					if( ns.length() > 0 )
					{
						if( first_num )
						{
							// The first number after 'F' identifies the variable
							var_num = atoi( ns.c_str() );
							if( var_num > (int)prob_matrix.size() )
							{
								throw( "FileManager::ReadBioDependencyMatrix - invalid variable number after F" );
							}
							first_num = false;
						}
						else
						{
							x = atof( ns.c_str() );
							if( 0.0 <= x && x <= 1.0 )
							{
								// Append the number to the current row
								row.push_back( x );
							}
							else
							{
								throw( "FileManager::ReadBioDependencyMatrix - probability value not in [0.0, 1.0]" );
							}
						}
					}
					if( ch == '\n' )
					{
						if( row.size() !=  GAParams::NumVars()  )
						{
							throw( "FileManager::ReadBioDependencyMatrix - row length does not match number of variables");
						}
						prob_matrix[ var_num - 1 ] = row;
						row.clear();
						still_blank = true; first_num = true;
					}
					// Leave the 'in number' state
					in_num = false;
				}
			}
			else // we are outside of a number
			{
				if( isdigit( ch ) || ch == '.' )
				{
					// accumulate the first digit
					ns.clear();
					if( ch == '.' ) ns.push_back( '0' );
					ns.push_back( ch );
					in_num = true;
				}
				else
				{
					if( ch == '\n' )
					{
						if( row.size() > 0 )
						{
							// Normalize the values using the last column
							if( row.size() !=  GAParams::NumVars() )
							{
								throw( "FileManager::ReadBioDependencyMatrix - row length does not match number of variables");
							}
							prob_matrix[ var_num - 1 ] = row;
						}
						row.clear();
						still_blank = true; first_num = true;
					}
				}
			}
		}
		else
		{
			if( ch == 'F' && still_blank )
			{
				in_row = true;
			}
			if( isalnum( ch ))  still_blank = false;
			if( ch == '\n' )
			{
				still_blank = true; first_num = true;
			}
		}
	}
	in_file.close();
	return true;
}

// Enter this function after a '(' encountered
//   it will return a ComplexityMatrixEntry when the ')' is encountered
ComplexityMatrixEntry FileManager::ReadCmplxEntry( std::ifstream& in_file )
{
	int p,q;
	char ch;
	String num_str;
	ComplexityMatrixEntry c;
	ComplexityQuotient x;
	bool in_numerator = true;

	while( !in_file.eof() )
	{
		in_file.get( ch );

		if( in_numerator )
		{
			if( isdigit( ch ) )
			{
				num_str.push_back( ch );
			}
			else if( ch == '/' )
			{
				in_numerator = false;
				p = atoi( num_str.c_str() );
				num_str.clear();
			}
		}
		else // in denominator
		{
			if( isdigit( ch ) )
			{
				num_str.push_back( ch );
			}
			else if( ch == '+' || ch == ')' )
			{
				in_numerator = true;
				q = atoi( num_str.c_str() );
				num_str.clear();

				if( q == 0 && p != 0 )
				{
					throw( "illegal zero denominator" );
				}
				// Form the complexity quotient and append it to the entry
				x.p = p;	x.q = q;
				c.push_back( x );
				if( ch == ')' ) return c;
			}
		}
	}
	throw( "closing ) not found" );
	return c;
}

bool FileManager::ReadComplexityMatrix( ComplexityMatrix& cmplx_mat )
{
	// Open the file
	// For each non-blank line
	//	extract Fnn
	//	for mNumVars entries
	//		read each entry as ( p1/q1 + p2/q2 + ... )
	// 

	// If no file name do not complain
	if( mDepGraphFile.length() == 0 ) return true;

	String file_path = mPath + mDepGraphFile;
	std::ifstream in_file;
	in_file.open( file_path.c_str() );
	if( !in_file.good() )
	{
		throw( String("FileManager::ReadComplexityMatrix - unable to open file") + file_path);
	}

	char ch;
	int n;
	int var_num;
	String ns; // to collect a string of digits representing a number

	// fill the complexity matrix with mNumVar rows
	ComplexityMatrixRow row;
	for( n = (int) GAParams::NumVars(); n > 0; --n )
	{
		cmplx_mat.push_back( row );
	}

	bool in_num = false;
	bool in_row = false;
	bool still_blank = true;
	bool first_num = true;
	while( !in_file.eof() )
	{
		in_file.get( ch );
		if( in_row )
		{
			if( in_num )
			{
				if( isdigit( ch ) ) // accumulate the digit as a number
				{
					ns.push_back( ch );
				}
				else
				{ // non-digit found to end the number
					if( ns.length() > 0 )
					{
						if( first_num )
						{
							// The first number after 'F' identifies the variable
							var_num = atoi( ns.c_str() );
							if( var_num > (int)GAParams::NumVars()  )
							{
								throw( "FileManager::ReadComplexityMatrix - invalid variable number after F" );
							}
							first_num = false;
						}
						else
						{
							n = atoi( ns.c_str() );
							if( n == 0  || row.size() == GAParams::NumVars() )
							{
								// Dummy up ( (0,0 ) ) for a single 0 entry
								// Or ( (n,n) ) for the last entry which is total appearances
								ComplexityQuotient x;
								x.p = n; x.q = n;
								ComplexityMatrixEntry cp;
								cp.push_back( x );
								row.push_back( cp );
							}
							else
							{
								throw( "FileManager::ReadComplexityMatrix - invalid complexity entry" );
							}
						}
					}
					if( ch == '\n' )
					{
						if( row.size() !=  GAParams::NumVars() + 1  )
						{
							throw( "FileManager::ReadBioDependencyMatrix - row length does not match number of variables");
						}
						cmplx_mat[ var_num - 1 ] = row;
						row.clear();
						still_blank = true; first_num = true;
					}
					// Leave the 'in number' state
					in_num = false;
				}
			}
			else // we are outside of a number
			{
				if( isdigit( ch ) )
				{
					// accumulate the first digit
					ns.clear();
					ns.push_back( ch );
					in_num = true;
				}
				else if( ch == '(' ) // Start of a Complexity entry ( p1/q1 + p2/q2 + ... )
				{
					ComplexityMatrixEntry cp = ReadCmplxEntry( in_file );
					row.push_back( cp );
				}
				else if( ch == '\n' )
				{
					if( row.size() > 0 )
					{
						if( row.size() !=  GAParams::NumVars() + 1 )
						{
							throw( "FileManager::ReadBioDependencyMatrix - row length does not match number of variables");
						}
						cmplx_mat[ var_num - 1 ] = row;
					}
					row.clear();
					still_blank = true; first_num = true;
				}
			}
		}
		else
		{
			if( ch == 'F' && still_blank )
			{
				in_row = true;
			}
			if( isalnum( ch ))  still_blank = false;
			if( ch == '\n' )
			{
				still_blank = true; first_num = true;
			}
		}
	}
	in_file.close();
	return true;
}


bool FileManager::ReadDiscreteModels( DynSysModelVector& models )
{
	StringVectorIter mf_iter = model_file_list.begin();
	while( mf_iter != model_file_list.end() )
	{
		String file_path = mPath + *mf_iter;
		std::ifstream in_file;
		in_file.open( file_path.c_str() );
		if( !in_file.good() )
		{
			throw( String("FileManager::ReadDiscreteModels - unable to open file") + file_path );
		}

		DynSysModel m;
		while( !in_file.eof() )
		{
			String in_line = GetLine( in_file );

			// Ignore lines that are too short to hold a function definition
			if( in_line.length() < 4 ) continue;

			Polynomial  p;
			if( ParseFunction( in_line, p ) )
			{
				// Set function number k
				if( m.IsDefaultPolyQ( p.mK ) )
				{
					m.SetFunction( p.mK, p );
				}
				else
				{
					throw( "FileManager::ReadDiscreteModels - duplicate function number");
				}
			}
		}

		// Check that we have one polynomial for each variable in the model
		if( !m.IsCompleteModelQ() )
		{
			throw( "FileManager::ReadDiscreteModels - insufficient number of functions to construct a complete model" );
		}
		models.push_back( m );
		in_file.close();
		++mf_iter;
	}
	return true;
}

// Parse a string of the form: f1 = x1*x2 + x3 + 1
// to fill in the Polynomial object poly
// return true if sucessful, false if the string is invalid
bool FileManager::ParseFunction( String& s, Polynomial& poly )
{
	String digits( "0123456789" );

	Monomial m;

	bool got_something = false;

	// function starts with Fnn
	size_t i = s.find_first_of( String("fF") );
	if( i == s.npos ) return false;	// throw( "ParseFunction - missing 'F' for function" );

	// get the function number
	size_t j = s.find_first_not_of( digits, i+1 );
	if( j == s.npos )	return false;	//	throw( "ParseFunction - missing function number" );
	if( j <= i+1 )		return false;	//	throw( "ParseFunction - missing function number" );

	// the function number is found in s[i+1] to s[j-1]
	String fn = s.substr( i+1, j-i-1 );

	// Save the function number in the polynomial
	poly.mK = atoi( fn.c_str() );

	// Make sure the equal sign is there
	i = s.find( '=', j);
	if( i == s.npos ) throw( "ParseFunction - missing equal sign" );

	// TBD - for general case must read optional coefficient first

	// Try to find the first element Xnn
	j = s.find_first_of( String("-+01xX"), i+1 );
	if( j == s.npos )
	{
		throw( "ParseFunction - no terms after equal sign" );
	}
	else
	{
		if( s[j] == '+' || s[j] == '-' )
		{
			j = s.find_first_of( String("01xX"), j+1 );
			if( j == s.npos ) throw( "ParseFunction - missing term after +/-" );
		}
	}

	size_t k;
	// Now capture each monomial in turn
	while( j != s.npos )
	{
		i = j;
		// Handle the constant term
		if( s[j] == '0' || s[j] == '1' )
		{
			m.Reset();
			if( s[j] == '0' ) m.mCoeff = false;
			// poly.AddTerm( m ) will be done after +/- or end

			got_something = true;
			k = s.find_first_not_of( digits, j+1 );
		}
		else
		{
			String xn;
			k = s.find_first_not_of( digits, j+1 );
			if( k == j+1 ) throw( "ParseFunction - missing variable number" );

			// If there were no non-digits after the variable number
			if( k == s.npos )
			{
				// the digits are the remaining elements of the string
				xn = s.substr( j+1 );
			}
			else
			{
				// the variable number runs from s[j+1] to s[k-1]
				xn = s.substr( j+1, k-j-1 );
			}

			// Here we have captured Xxn, set bit xn in the current monomial
			size_t b = atoi( xn.c_str() );		// ASCII to Integer
			if( b > GAParams::NumVars() ) throw( "ParseFunction - illegal variable number" );

			got_something = true;
			m.Set( b );
		}
		// Does a + or - come before the next X, then the monomial is done
		j = s.find_first_of( "+-xX" , k );
		if( j != s.npos )
		{
			i = j;
			if( s[j] == '+' || s[j] == '-' )
			{
				// we have the entire monomial, add it to the polynomial

				if( m.mCoeff )
				{
					poly.AddTerm( m );
				}

				// Clear the monomial for the next time
				m.Reset();
				got_something = false;


				// Find the next element Xnn
				j = s.find_first_of( "01xX" , j );
				if( j == s.npos ) throw( "ParseFunction - term after +/-" );
			}
		}
	}
	// If we have a partial term when we reach the end of the string
	//	( there will be no trailing '+/-' )
	if( got_something )
	{
		// Add the last term
		poly.AddTerm( m );
	}
	// If here, we have created a valid polynomial
	return true;
}


bool FileManager::ReadParamFile( GAParams& param_obj )
{
	// Set the default parameters
	param_obj.SetParameters( );

	// If no file name do not complain
	if( mParamFile.length() == 0 ) return true;

	// Build the path from the filename
	String fullpath = mPath + mParamFile;

	std::ifstream param_file( fullpath.c_str() );
	if( param_file )
	{
		while( !param_file.eof() )
		{
			String name_value_pair = GetLine( param_file );
			if( name_value_pair.length() > 1 )
			{
				// captures the name-value pair from the string
				param_obj.ParseGAParameter( name_value_pair );
			}
		}

		param_file.close( );

		// transfers the name-value pairs to the parameter member variables of GAParams
		param_obj.SetParameters( );

		// Verify that the user's choice of parameters is consistent
		if( mDepGraphFile.length() == 0 && GAParams::DepGraphWeight() != 0.0 )
		{
			throw( "FileManager::ReadParamFile - missing dependency graph weight file even though the corresponding score weight is not zero" );
		}


		if( mRevFile.length() == 0 && GAParams::RevEngWeight() != 0.0 )
		{
			throw( "FileManager::ReadParamFile - missing Reverse engineering weight file even though the corresponding score weight is not zero" );
		}


		if( mBioFile.length() == 0 && GAParams::BioProbWeight() != 0.0 )
		{
			throw( "FileManager::ReadParamFile - missing Biological Probability file even though the corresponding score weight is not zero" );
		}


		if( !param_obj.NormalizePolyWeights() )
		{
			std::cout << "Polynomial score weights did not sum to 1.0 - weights have been re-scaled" << std::endl;
		}
		if( !param_obj.NormalizeModelWeights() )
		{
			std::cout << "Model score weights did not sum to 1.0 - weights have been re-scaled" << std::endl;
		}
		return true;
	}
	return false;
}

// FileManager.cpp
