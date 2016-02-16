// FileManager.h
//
// This class is responsible for parsing the Control File, and storing its data.
// It provides methods for reading the various files indicated by the Control File
// and filling the appropriate objects such as time series, models, 
// and dependency matrices

#ifndef _VBI_POLYMATH_GA_FILE_MANAGER_H_
#define _VBI_POLYMATH_GA_FILE_MANAGER_H_

// KOFileSpec
//		small class to hold a knockout filename along with the
//		variable number which was knocked out.
struct KOFileSpec
{
	KOFileSpec( int k, const String& filename )
	{
		mK = k;
		mFilename = filename;
	}

	int			mK;				// Variable number
	String mFilename;		// Filename
};


typedef std::vector<KOFileSpec>  KOFileSpecList;
typedef KOFileSpecList::iterator KOFileSpecListIter;

class FileManager
{
public:
	FileManager( )
	{
		// Fix to binary for now
		mFieldDigits = "01";

		// Initialize all built-in types
		mNumVars = 0;
		mMaxWtTsLength = 0;
		mMaxKoTsLength = 0;
	}

	// next_token( s, delim) 
	//		Return the next token or delimiter in s, and remove it from the front of s.
	static String next_token( String& s, const String& delim );

	// Read in and parse the text file that controls a run of the G.A.
	bool ReadControlFile( String& control_file_path );

	// Read in a single time series from a given path
	bool ReadTimeSeries( String& fullpath, TimeSeries& ts, size_t kov = 0 );

	// Read in all of the wild-type time-series, one for each file in wt_file_list
	bool ReadWildTimeSeries( TimeSeriesVector& wt_series );

	// Read in all of the knockout time series, one set for each variable
	bool ReadKnockOutTimeSeries( TimeSeriesVectorSet& ko_series_set );

	// Read in a file containing frequence of use counts for each variable in each function
	bool ReadReverseEngWeightMatrix( WeightMatrix& weight_matrix );

	// Read in a file containing a biological dependency probability matrix
	bool ReadBioDependencyMatrix( WeightMatrix& prob_matrix );

	// Read in a file containing a matrix complexity vectors
	bool ReadComplexityMatrix( ComplexityMatrix& cmplx_mat );

	// Given a string representation of a polynomial in x,
	//	create the corresponding Polynomial object
	bool ParseFunction( String& s, Polynomial& poly );

	// Read in all of the given models
	bool ReadDiscreteModels( DynSysModelVector& models );

	bool ReadParamFile( GAParams& param_obj );

	// Number of variables as determined by the control file
	size_t			mNumVars;

	size_t			mMaxWtTsLength;
	size_t			mMaxKoTsLength;

private:
	ComplexityMatrixEntry ReadCmplxEntry( std::ifstream& in_file );
	void ProcessRow( WeightMatrixRow& row );
	String GetLine( std::ifstream& in_file );

	String			mFieldDigits;
	String			mPath;
	StringVector	wt_file_list;
	KOFileSpecList  ko_file_list;
	StringVector	model_file_list;
	String			mBioFile;		
	String			mRevFile;
	String			mDepGraphFile;
	String			mParamFile;
};

#endif // _VBI_POLYMATH_GA_FILE_MANAGER_H_
