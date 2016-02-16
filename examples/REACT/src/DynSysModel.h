// DynSysModel.h

#ifndef _VBI_POLYMATH_DYN_SYS_MODEL_H_
#define _VBI_POLYMATH_DYN_SYS_MODEL_H_

// Class to hold a single discrete dynamical system model
class DynSysModel
{
public:
	DynSysModel( )
	{
		// Make a default (zero) polynomial
		Polynomial p;

		// Fill the model with num_variables copies of p
		size_t i;
		for( i = GAParams::NumVars(); i > 0; --i )
		{
			// Append a copy of p to the model
			mModel.push_back( p );
		}

		// Ensure scores are initialize
		mHammingScore  = 0.0;
        mModelScore    = 0.0;

		// Start off mutated
		mMutated = true;
	}

	// Copy constructor
	DynSysModel( const DynSysModel& other )
	{
		// Copy everything
		mState			= other.mState;
		mMutated		= other.mMutated;
		mHammingScore	= other.mHammingScore;
		mModelScore		= other.mModelScore;

		// TBD - make sure this is the fastest copy of the polynomial vector
		mModel   = other.mModel;
	}

	// Set the k'th polynomial
	void SetFunction( size_t k, Polynomial& poly )
	{
		// Verify that index k is valid 1 <= k < num_vars
		if( k > GAParams::NumVars() || k == 0 )
		{
			throw( "DynSysModel::SetFunction - index out of bound" );
		}
		// Assign the k'th polynomial
		mModel[k-1] = poly;

		// Model is mutated
		mMutated = true;
	}

	// Set the Hamming score for the k'th polynomial
	void SetPolyHamming( size_t k, double hamming_ratio )
	{
		// Verify that index k is valid 1 <= k < num_vars
		if( k > GAParams::NumVars() || k == 0 )
		{
			throw( "DynSysModel::SetFunction - index out of bound" );
		}
		// Assign the k'th polynomial
		mModel[k-1].SetHammingScore( hamming_ratio );

		mMutated = true;
	}

	// Set the complexity score for each polynomial
	void SetPolyComplexities( );

	// Return a reference to the k'th polynomial of this model
	Polynomial& operator[]( size_t k );

	// Compare two models for equality based on their polynomials
	bool operator==( DynSysModel& other )
	{
		PolyModelIter ita = mModel.begin();
		PolyModelIter itb = other.mModel.begin();
		while( ita != mModel.end() && itb != other.mModel.end() )
		{
			// They are different if they have two distinct functions for the same variable
			if( !(*ita++ == *itb++ ) ) return false;
		}
		// If we get here the models are identical
		return true;
	}

	bool IsDefaultPolyQ( size_t k )
	{
		// Verify that index k is valid 1 <= k < num_vars
		if( k > GAParams::NumVars() || k == 0 )
		{
			throw( "DynSysModel::SetFunction - index out of bound" );
		}
		return mModel[k-1].IsDefaultPolyQ();
	}

	// Model is complete if there is one function for each variable
	bool IsCompleteModelQ()
	{
		int i = (int) GAParams::NumVars() - 1;
		while( i >= 0 )
		{
			if( mModel[i--].IsDefaultPolyQ()) return false;
		}
		return true;
	}

	// Mutate a random polynomial of this model
	void Mutate()
	{
		int k = PolyMathRand::RandInt(1, (int) GAParams::NumVars() );
		mModel[k-1].Mutate();

		mMutated = true;
	}

	// Commit that the current scores in the model and its polynomials are correct
	//	for this generation
	void UnMutate()
	{
		// Turn of the mutate state in each polynomial and in the model itself
		PolyModelIter pm_iter = mModel.begin();
		while( pm_iter != mModel.end() )
		{
			pm_iter->UnMutate();
			++pm_iter;
		}
		mMutated = false;
	}

	// Compute the score for each function in the model with respect to the
	//	Reverse Engineering Weight Matrix
	void ScoreReverseEngineeringWeights( WeightMatrix& rev_weight_matrix )
	{
		// No need to compute if the model has not changed
		if( !mMutated ) return;

		// Each row of the weight matrix corresponds to a function
		WeightMatrixIter wm_iter = rev_weight_matrix.begin();
		int k = 1;
		while( wm_iter != rev_weight_matrix.end() )
		{
			mModel[k-1].ScoreReverseEngineeringWeights( *wm_iter );

			++wm_iter; ++k;
		}
	}

	void ScoreBioProbabilities( WeightMatrix& bio_prob_matrix )
	{
		// No need to compute if the model has not changed
		if( !mMutated ) return;

		// Each row of the weight matrix corresponds to a function
		WeightMatrixIter wm_iter = bio_prob_matrix.begin();
		int k = 1;
		while( wm_iter != bio_prob_matrix.end() )
		{
			mModel[k-1].ScoreBioProbabilities( *wm_iter );

			++wm_iter; ++k;
		}
	}

	void DumpModel( std::ofstream& out_file )
	{
		int k = 1;
		PolyModelIter pm_iter = mModel.begin();
		while( pm_iter != mModel.end() )
		{
			String ps = pm_iter->ToString( true );
			out_file << "  f" << k << " = " << ps << std::endl;
				out_file << "\t\tH: " << pm_iter->mHammingScore;
				out_file << "\tC: "   << pm_iter->mComplexityScore;
				out_file << "\tR: "   << pm_iter->mRevEngScore;
				out_file << "\tB: "   << pm_iter->mBioProbScore;
				out_file << std::endl;
			++k;
			++pm_iter;
		}
		out_file << "\tH: " << mHammingScore << " \t T: " << mModelScore << std::endl;
	}

	// Get the average score of the polynomials in this model
	double GetAvgPolyScore( );

	// GetScore - return the score for this model
	double GetScore( );

	// GetPolyProb - return the probability that we should use polynomial k
	//	based on its composite score
	double GetPolyProb( size_t k );

	// Set the Hamming score for the model
	inline void SetHamming( double hamming_ratio )
	{
		mHammingScore = (1 - hamming_ratio);
		mMutated = true;
	}
	
	inline double HammingScore() const
	{
		return mHammingScore;
	}

	inline bool IsMutatedQ( ) { return mMutated; }

	// Provided for insurance - should not be needed
	inline void SetMutated( ) { mMutated = true; }

	// Iterate from an initial state
	void Iterate( NTuple& initial_state, size_t length, TimeSeries& result );

	// Iterate only the k'th variable with respect to the given reference time series
	size_t Iterate( TimeSeries& ref_series, size_t k, TimeSeries& result, size_t kov = 0 );

	// Iterate from an initial state with the k'th function knocked out
	void KoIterate( NTuple& initial_state, size_t length, TimeSeries& result, size_t kov );

private:
	bool		mMutated;		// True if any part of this model has been mutated.

	double		mHammingScore;	// Overall Hamming distance score for this model

	double		mModelScore;	// Total score for this model

	NTuple		mState;			// Current state during iteration

	PolyModel	mModel;			// The vector of polynomials that comprise the model

};

// Define a vector of DynSysModels
typedef std::vector<DynSysModel>	DynSysModelVector;
typedef DynSysModelVector::iterator	DynSysModelVectorIter;

#endif //_VBI_POLYMATH_DYN_SYS_MODEL_H_

