// GAParams.cpp
#include "BoolModel.h"


// Static member variables of the GAParams class
//	allow access from other classes without reference to a particular GAParams object
size_t		GAParams::mNumVars;
size_t		GAParams::mGenePoolSize;
size_t		GAParams::mNumCandidates;
size_t		GAParams::mNumParentsToPreserve;
size_t		GAParams::mMaxGenerations;
size_t		GAParams::mStableGenerationLimit;
double		GAParams::mMutateProbability;
double		GAParams::mHammingPolyWeight;
double		GAParams::mComplexityWeight;
double		GAParams::mRevEngWeight;
double		GAParams::mDepGraphWeight;
double		GAParams::mBioProbWeight;
double		GAParams::mHammingModelWeight;
double		GAParams::mPolyScoreWeight;

// Initialize the parameter map to the defaults
void GAParams::InitParameterTable( )
{
	String name;
	double value;

	size_t num_params = sizeof( param_list ) / sizeof(NameValuePair);
	size_t i = 0;
	while( i < num_params )
	{
		name  = String(param_list[i].mName);
		value = param_list[i].mValue;

		mParamMap.insert( ParamMap::value_type( name, ParamValue( value, true) ) );

		++i;
	}
}

// Parse a name, value pair string to set a value in the parameter map
//   return true if the value set, false if the string fails to parse
bool GAParams::ParseGAParameter( String& name_value_pair )
{
	// Define separator characters
	String separators(" \t,:");

	// Number of characters in the given string
	size_t k = name_value_pair.size();

	// Locate the keyword start and end + 1
	size_t key_start_pos = name_value_pair.find_first_not_of( separators );
	if( key_start_pos == name_value_pair.npos )
		return false;

	size_t key_end_pos = name_value_pair.find_first_of( separators, key_start_pos );
	if( key_end_pos == name_value_pair.npos )
		return false;

	// Extract the keyword
	size_t key_length = key_end_pos - key_start_pos;
	std::string key_word = name_value_pair.substr( key_start_pos, key_length );

	// Locate the value start and end + 1
	size_t value_start_pos = name_value_pair.find_first_not_of( separators, key_end_pos );
	if( value_start_pos == name_value_pair.npos )
		return false;

	size_t value_end_pos = name_value_pair.find_first_of( separators, value_start_pos );
	if( value_end_pos == name_value_pair.npos )
		value_end_pos = k;

	// Extract the value
	size_t value_length = value_end_pos - value_start_pos;
	std::string value = name_value_pair.substr( value_start_pos, value_length );

	double x = atof( value.c_str() );

	// Insert the value into the map if the keyword is valid
	ParamMapIter iter = mParamMap.find( key_word );
	if( iter != mParamMap.end() )
	{
		iter->second.mValue = x;
		iter->second.mDefault = false;

		// Indicate success
		return true;
	}

	// Invalid keyword
	return false;
}

// Set the G.A. tuning parameters based on the contents of
//	the parameter map mParamMap.   The values in mParamMap
//  are set to the defaults by InitParameterTable()
//	Note: this function must be updated each time a new parameter is added
//		or an existing parameter is removed or changes type
void GAParams::SetParameters( )
{
	// Set parameter member variables from the parameter map
	// Note: the contents of the parameter map are validated upon entry
	//	so no checking is done here
	ParamMapIter iter;
	ParamMapIter end_iter = mParamMap.end();

	iter = mParamMap.find( "GenePoolSize" );
	if( iter != end_iter )
	{
		// If not set by the parameter file
		if( iter->second.mDefault )
		{
			mGenePoolSize = 200 * mNumVars;
		}
		else
		{
			mGenePoolSize = (size_t) iter->second.mValue;
		}
	}
	iter = mParamMap.find( "NumCandidates" );
	if( iter != end_iter )
	{
		// If not set by the parameter file
		if( iter->second.mDefault )
		{
			mNumCandidates =  2 + mGenePoolSize / 8;
		}
		else
		{
			mNumCandidates = (size_t) iter->second.mValue;
		}
	}
	iter = mParamMap.find( "NumParentsToPreserve" );
	if( iter != end_iter )
	{
		// If not set by the parameter file
		if( iter->second.mDefault )
		{
			mNumParentsToPreserve = 1 + mNumCandidates / 20;
		}
		else
		{
			mNumParentsToPreserve = (size_t) iter->second.mValue;
		}
	}
	iter = mParamMap.find( "MaxGenerations" );
	if( iter != end_iter )
	{
		mMaxGenerations = (size_t) iter->second.mValue;
	}
	iter = mParamMap.find( "StableGenerationLimit" );
	if( iter != end_iter )
	{
		mStableGenerationLimit = (size_t) iter->second.mValue;
	}
	iter = mParamMap.find( "MutateProbability" );
	if( iter != end_iter )
	{
		mMutateProbability = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "HammingPolyWeight" );
	if( iter != end_iter )
	{
		mHammingPolyWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "ComplexityWeight" );
	if( iter != end_iter )
	{
		mComplexityWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "RevEngWeight" );
	if( iter != end_iter )
	{
		mRevEngWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "DepGraphWeight" );
	if( iter != end_iter )
	{
		mDepGraphWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "BioProbWeight" );
	if( iter != end_iter )
	{
		mBioProbWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "HammingModelWeight" );
	if( iter != end_iter )
	{
		mHammingModelWeight = (double) iter->second.mValue;
	}
	iter = mParamMap.find( "PolyScoreWeight" );
	if( iter != end_iter )
	{
		mPolyScoreWeight = (double) iter->second.mValue;
	}
}

// Test and normalize the weights so that they sum to 1.0
//	return true if normalizable
bool GAParams::NormalizePolyWeights( )
{
	double total_poly_weight = mHammingPolyWeight + mComplexityWeight
			+ mRevEngWeight + mDepGraphWeight + mBioProbWeight;

	// If the total weight is close to zero
	const double epsilon = 0.001;
	if( fabs( total_poly_weight ) < epsilon )
	{
		// Force to equal weighting
		mHammingPolyWeight  = 0.25;
		mComplexityWeight   = 0.25;
		mRevEngWeight		= 0.25;
		mDepGraphWeight		= 0.00;
		mBioProbWeight      = 0.25;
		return false;
	}
	else
	{
		// Normalize the sum by dividing by the total weight
		mHammingPolyWeight	/= total_poly_weight;
		mComplexityWeight	/= total_poly_weight;
		mRevEngWeight		/= total_poly_weight;
		mDepGraphWeight		/= total_poly_weight;
		mBioProbWeight		/= total_poly_weight;
	}
	// return true if total weight very close to 1.0
	return fabs( total_poly_weight - 1.0 ) < epsilon;
} 

// Test and normalize the weights so that they sum to 1.0
//	return the original sum
bool GAParams::NormalizeModelWeights( )
{
	double total_model_weight = mHammingModelWeight + mPolyScoreWeight;

	// If the total weight is close to zero
	const double epsilon = 0.00001;
	if( fabs( total_model_weight ) < epsilon )
	{
		// Force to equal weighting
		mHammingModelWeight  = 0.5;
		mPolyScoreWeight     = 0.5;
		return false;
	}
	else
	{
		// Normalize the sum by dividing by the total weight
		mHammingModelWeight /= total_model_weight;
		mPolyScoreWeight    /= total_model_weight;
	}
	// return true if total weight very close to 1.0
	return fabs( total_model_weight - 1.0 ) < epsilon;
} 


// GAParams.cpp
