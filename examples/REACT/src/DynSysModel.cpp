// DynSysModel.cpp

#include "BoolModel.h"

// Iterate from an initial state
void DynSysModel::Iterate( NTuple& initial_state, size_t length, TimeSeries& result )
{
	// Make sure the result is empty
	result.clear();

	// Save the initial state as the first time series entry
	result.push_back( initial_state );

	mState = initial_state;
	NTuple next_state;
	PolyModelIter iter;

	size_t i,k;
	for( i = length-1; i > 0; --i )
	{
		k = 1;
		// For each polynomial;
		next_state.Reset();
		iter = mModel.begin();
		while( iter != mModel.end() )
		{
			// Evaluate the k'th polynomial at the current state
			//   to produce a new value for the k'th variable
			next_state.Assign( k, mModel[k-1].Evaluate( mState ) );

			// next polynomial
			++iter; ++k;
		}
		// Update the current state to be the newly compute state
		mState = next_state;
		result.push_back( next_state );
	}
}

// Iterate from an initial state with the k'th function knocked out
void DynSysModel::KoIterate( NTuple& initial_state, size_t length, TimeSeries& result, size_t kov )
{
	// Make sure the result is empty
	result.clear();

	// Force k'th entry to zero in the initial state -  no longer needed, corrected when file is read in
	// NTuple state1 = initial_state;
	// state1.Reset( kov );

	// Save the initial state as the first time series entry
	result.push_back( initial_state );

	mState = initial_state;
	NTuple next_state;
	PolyModelIter iter;

	size_t i,k;
	for( i = length-1; i > 0; --i )
	{
		k = 1;
		// For each polynomial
		next_state.Reset();
		iter = mModel.begin();
		while( iter != mModel.end() )
		{
			// Force the knockout function result to zero
			if( k == kov )
			{
				next_state.Assign( k, 0 );
			}
			else
			{
				// Evaluate the k'th polynomial at the current state
				//   to produce a new value for the k'th variable
				next_state.Assign( k, mModel[k-1].Evaluate( mState ) );
			}

			// next polynomial
			++iter; ++k;
		}
		// Update the current state to be the newly compute state
		mState = next_state;
		result.push_back( next_state );
	}
}


// Iterate only the k'th variable with respect to the given reference time series
size_t DynSysModel::Iterate( TimeSeries& ref_series, size_t k, TimeSeries& result, size_t kov )
{
	// Make sure the result is empty
	result.clear();

	// Hamming distance for bit k
	size_t h = 0;

	TimeSeriesIter t_iter = ref_series.begin();

	// Empty series in => empty series out
	if( t_iter == ref_series.end() ) return h;

	// Save the initial state as the first state in the result
	mState = *t_iter;

	// Force the knockout variable to zero - no longer needed, corrected when file is read in
	// if( kov != 0 ) mState.Reset(kov);
	result.push_back( mState );

	bool f;
	while( true )
	{
		// Merge the next state from the time series with the function evaluation
		// (a) increment iterator and check for end of time series
		++t_iter;
		if( t_iter == ref_series.end() ) break;

		// Evaluate the k'th polynomial at the current state
		//   to produce a new value for the k'th variable for time i+1
		f = mModel[k-1].Evaluate( mState );

		// (b) assign value pointed to by iterator to mState (t[i+1])
		mState = *t_iter;

		// Force the knockout variable to zero - no longer needed, corrected when file is read in
		// if( kov != 0 ) mState.Reset(kov);

		// Accumulate Hamming Distance =  t'[i+1][k] - t[i+1][k]
		h += (mState[k] ^ f) ? 1 : 0;

		// (c) create t'[i+1] = t[i+1] with k'th element replaced by f(t'[i])
		mState.Assign( k, f );

		// Save t'[i+1]
		result.push_back( mState );
	}
	return h;
}

// Return a reference to the k'th polynomial of this model
Polynomial& DynSysModel::operator[]( size_t k )
{
	// Verify that index k is valid 1 <= k < num_vars
	if( k > GAParams::NumVars() || k == 0 )
	{
		throw( "DynSysModel::[k] index out of bound" );
	}
	return mModel[k-1];
}


// Set the complexity score for each polynomial
void DynSysModel::SetPolyComplexities(  )
{
	// No need to compute if the model has not changed
	if( !mMutated ) return;

	int k = (int) mModel.size();
	double complexity_ratio;
	while( --k >= 0 )
	{
		complexity_ratio = (double) mModel[k].MaxDegree() / (double) mModel[k].MaxSupport();
		mModel[k].SetComplexityScore( complexity_ratio );
	}
}

double DynSysModel::GetAvgPolyScore( )
{
	double s = 0.0;
	double avg_poly_score = 0.0;

	// For each polynomial in the model
	PolyModelIter p_iter = mModel.begin();
	while( p_iter != mModel.end() )
	{
		// sum the polynomial scores
		avg_poly_score += p_iter->GetPolyProb();
		++p_iter;
	}
	// Compute the average polynomial score
	avg_poly_score /= GAParams::NumVars();
	return avg_poly_score;
}

// GetScore - return the score for this model
double DynSysModel::GetScore( )
{
	if( mMutated )
	{
		mModelScore = GAParams::HammingModelWeight() * mHammingScore
			+ GAParams::PolyScoreWeight() * GetAvgPolyScore( ); 
	}
	return mModelScore;
}

// GetPolyProb - return the probability that we should use polynomial k
//	based on its composite score
double DynSysModel::GetPolyProb( size_t k )
{
	// Verify that index k is valid 1 <= k < num_vars
	if( k > GAParams::NumVars() || k == 0 )
	{
		throw( "DynSysModel::SetFunction - index out of bound" );
	}
	return mModel[k-1].GetPolyProb();

}

// DynSysModel.cpp
