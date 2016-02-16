// GAModel.h
#ifndef _VBI_POLYMATH_GA_MODEL_H_
#define _VBI_POLYMATH_GA_MODEL_H_

typedef std::vector<DynSysModel> ModelList;
typedef ModelList::iterator ModelListIter;

typedef std::vector<DynSysModel> GenePool;
typedef GenePool::iterator GenePoolIter;


class GAModel
{
public:
	// Constructor
	GAModel( )
	{
		// Maximum number of terms = square of number of variables - for now TBD
		mMaxTerms = GAParams::NumVars()*GAParams::NumVars();

		// Initialize state variables
		mMinPolyScore	= 0.0;
		mMaxPolyScore	= 0.0;
		mMinModelScore	= 0.0;
		mMaxModelScore	= 0.0;
		mPrevBestScore	= 0.0;

		mNumMutations	= 0;
		num_stable_generations = 0;

	}
	
	// Destructor
	~GAModel();

	void InitializeGA( String& control_file_path );

	// Fill the gene pool with mutated copies of models in model_list
	//   or withh purely random models if the model_list is empty
	void FillGenePool( size_t num_models );

	// Iterate functions in each model in the gene pool to develop a set of scores
	void IterateModels( );

	// Create a candidate set from the best polynomials in the current gene pool
	void CreateCandidateSet();

	// Re-populate the gene pool via inheritance, crossover and mutation
	//   using the candidate set as parents
	void CreateNextGeneration();

	void Randomize( int seed );

	// Run - run the G.A. from the initial state until convergence or max_iter
	// MES: added: parameter: filename is the output which had been hardcoded as "BestModels.txt"
	void Run( char *filename );

	// Dump the contents of the gene pool to the given open file in human-readable format
	void DumpGenePool( std::ofstream& out_file, size_t max_models = 0 );

	// Dump the contents of the complexity matrix to the given open file
	void DumpComplexityMatrix( std::ofstream& out_file );

private:

	// Maximum number of terms (monomials) for any polynomial
	size_t		mMaxTerms;

	// The maximum score over all the polynomials currently in the gene pool
	double	mMinPolyScore;
	double	mMaxPolyScore;

	double	mMinModelScore;
	double	mMaxModelScore;
	double	mPrevBestScore;

	size_t	mNumMutations;

	size_t	num_stable_generations;

	FileManager FileMgr;

	// The wild type time series set
	TimeSeriesVector	mWtSeries;

	// The knockout time series set
	TimeSeriesVectorSet mKoSeriesSet;

	// The reverse engineering weight matrix
	WeightMatrix		mRevWeightMatrix;

	// The biological dependency matrix
	WeightMatrix		mBioDepMatrix;

	// The complexity matrix, each term is ( p1/q1 + p2/q2 + ... )
	ComplexityMatrix	mCmplxMat;

	// Input models from reverse engineering
	DynSysModelVector	mModelSet;

	// The candidates to start the next generation
	DynSysModelVector	mCandidates;

	// The gene pool consists of an array of polynomial models
	GenePool	mGenePool;
};

#endif // _VBI_POLYMATH_GA_MODEL_H_
