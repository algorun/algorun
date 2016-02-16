// GAParams.h

#ifndef _VBI_POLYMATH_GA_PARAMS_H_
#define _VBI_POLYMATH_GA_PARAMS_H_

// GAParams.h
// Structure to hold a name value pair
struct NameValuePair
{
	char*	mName;
	double	mValue;
};

struct ParamValue
{
	ParamValue( double v, bool d )
		: mValue(v), mDefault(d)
	{
	}
	double	mValue;
	bool	mDefault;
};

typedef std::map<String, ParamValue, std::less<String> > ParamMap;
typedef ParamMap::iterator ParamMapIter;

// Initial (default) values for G.A. Parameters
//  The first element of each pair is the keyword used to identify
//	the parameter, the second is the default value for that parameter
static NameValuePair param_list[] =
{
	{ (char*) "GenePoolSize",			 4000.00 },
	{ (char*) "NumCandidates",			  100.00 },
	{ (char*) "NumParentsToPreserve",	   10.00 },
	{ (char*) "MaxGenerations",			15000.00 },
	{ (char*) "StableGenerationLimit",	  300.00 },
	{ (char*) "MutateProbability",		    0.75 },

	// The following set of polynomial scoring weights must sum to 1.0
	{ (char*) "HammingPolyWeight",		    0.25 },
	{ (char*) "ComplexityWeight",		    0.25 },
	{ (char*) "RevEngWeight",			    0.25 },
	{ (char*) "DepGraphWeight",				0.00 },
	{ (char*) "BioProbWeight",			    0.25 },
	// End of sum

	// The following set of model scoring weights must sum to 1.0
	{ (char*) "HammingModelWeight",			0.3 },
	{ (char*) "PolyScoreWeight",		    0.7 },
};

// Class to manage and store
class GAParams
{
public:

	static inline void SetNumVars( size_t num_vars ) { mNumVars = num_vars; }

	static inline size_t NumVars()              { return mNumVars; }
	static inline size_t GenePoolSize()         { return mGenePoolSize; }
	static inline size_t NumCandidates()        { return mNumCandidates; }
	static inline size_t NumParentsToPreserve() { return mNumParentsToPreserve; }
	static inline size_t MaxGenerations()       { return mMaxGenerations; }
	static inline size_t StableGenerationLimit(){ return mStableGenerationLimit; }
	static inline double MutateProbability()    { return mMutateProbability; }
	static inline double HammingPolyWeight()    { return mHammingPolyWeight; }
	static inline double ComplexityWeight()     { return mComplexityWeight; }
	static inline double RevEngWeight()         { return mRevEngWeight; }
	static inline double DepGraphWeight()		{ return mDepGraphWeight; }
	static inline double BioProbWeight()        { return mBioProbWeight; }
	static inline double HammingModelWeight()   { return mHammingModelWeight; }
	static inline double PolyScoreWeight()      { return mPolyScoreWeight; }

	void InitParameterTable( );

	bool ParseGAParameter( String& name_value_pair );

	// Transfer parameters from mParamMap to the static parameter variable
	void SetParameters( );

	// Test and normalize the weights so that they sum to 1.0 
	bool NormalizePolyWeights( );

	// Test and normalize the weights so that they sum to 1.0 return true if total = 1
	bool NormalizeModelWeights( );

private:

	// An STL map of parameter values keyed by parameter name
	ParamMap	mParamMap;

	static size_t		mNumVars;
	static size_t		mGenePoolSize;
	static size_t		mNumCandidates;
	static size_t		mNumParentsToPreserve;
	static size_t		mMaxGenerations;
	static size_t		mStableGenerationLimit;
	static double		mMutateProbability;
	static double		mHammingPolyWeight;
	static double		mComplexityWeight;
	static double		mRevEngWeight;
	static double		mDepGraphWeight;
	static double		mBioProbWeight;
	static double		mHammingModelWeight;
	static double		mPolyScoreWeight;

};


#endif // _VBI_POLYMATH_GA_PARAMS_H_
