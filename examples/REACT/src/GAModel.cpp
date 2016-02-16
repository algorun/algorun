// class GAModel

// The GAModel class contains the methods which control the highest level operation
//	of the Evolutionary Algorithm, such as filling the gene pool, evaluating each model
//	in the gene pool, and forming the next generation using the best genes.

#include "BoolModel.h"

// Comparator function for sorting gene pool into ascending order
bool IsFirstGreater( const DynSysModel& first, const DynSysModel& second )
{
	return const_cast<DynSysModel&>(first).GetScore() > const_cast<DynSysModel&>(second).GetScore();
}

bool IsFirstGreaterPoly( const Polynomial& first, const Polynomial& second )
{
	return const_cast<Polynomial&>(first).GetPolyProb() > const_cast<Polynomial&>(second).GetPolyProb();
}

// Destructor
GAModel::~GAModel()
{
}

void GAModel::InitializeGA( String& control_file_path )
{
	if( !FileMgr.ReadControlFile( control_file_path ) )
	{
		throw( "GAModel::InitializeGA - unable to process control file" );
	}
	GAParams::SetNumVars( FileMgr.mNumVars );

	// Maximum number of terms = square of number of variables - for now TBD
	mMaxTerms = GAParams::NumVars() * GAParams::NumVars();

	if( !FileMgr.ReadWildTimeSeries( mWtSeries ) )
	{
		throw( "GAModel::InitializeGA - unable to read wild type time series" );
	}
	if( !FileMgr.ReadKnockOutTimeSeries( mKoSeriesSet ) )
	{
		throw( "GAModel::InitializeGA - unable to read knockout time series set" );
	}
	if( mWtSeries.size() + mKoSeriesSet.size() == 0 )
	{
		throw( "GAModel::InitializeGA - no time series were found" );
	}

	// Compute maximum support based on the maximum time series lengths
	size_t max_ts_length = std::max( FileMgr.mMaxKoTsLength, FileMgr.mMaxWtTsLength );

	// max_support = log base 2 of the maximum time series length
	// TBD - must refine for fields other than F2
	size_t max_support = (size_t) std::ceil( log( (double) max_ts_length ) / log( 2.0 ));
	Polynomial::SetMaxSupport( max_support );

	if( !FileMgr.ReadReverseEngWeightMatrix( mRevWeightMatrix ) )
	{
		throw( "GAModel::InitializeGA - unable to read reverse engineering matrix" );
	}
	if( !FileMgr.ReadBioDependencyMatrix( mBioDepMatrix ) )
	{
		throw( "GAModel::InitializeGA - unable to read BioDependencyMatrix" );
	}
	if( !FileMgr.ReadComplexityMatrix( mCmplxMat ) )
	{
		throw( "GAModel::InitializeGA - unable to read Complexity Matrix" );
	}
	if( !FileMgr.ReadDiscreteModels( mModelSet ) )
	{
		throw( "GAModel::InitializeGA - unable to read models" );
	}
	// Create a Parameter object and initialize it with default values
	GAParams ga_param_obj;
	ga_param_obj.InitParameterTable( );

	if( !FileMgr.ReadParamFile( ga_param_obj ) )
	{
		throw( "GAModel::InitializeGA - unable to read parameter file" );
	}
}


// Fill the gene pool with random models
//	Use input models as a basis if given
void GAModel::FillGenePool( size_t num_models )
{
	int i,j,k,z;

	int model_set_size =  (int) mModelSet.size();
	if(  model_set_size == 0 )
	{
		for( i = (int) num_models; i > 0; --i )
		{
			DynSysModel m;

			Polynomial p;
			for( k = 1; k <= (int)GAParams::NumVars(); ++k )
			{
				p.GenerateRandomPolynomial( mMaxTerms );
				m.SetFunction( k, p );
			}
			mGenePool.push_back( m );
		}
	}
	else
	{
		for( i = model_set_size; i > 0; --i )
		{
			mGenePool.push_back( mModelSet[i-1] );
		}
		// 90% as mutated copies of the originals
		for( i = (int)(9*num_models/10); i > 0; --i )
		{
			// Choose a random input model as a starting point
			j = PolyMathRand::RandInt(1, model_set_size);

			DynSysModel m = mModelSet[j-1];

			// Choose a random number of mutations
			z = PolyMathRand::RandIntExp( 3 );

			// For each mutation
			for( k = z; k > 0; --k )
			{
				// Perform one mutation on the model
				m.Mutate();
			}
			mGenePool.push_back( m );
		}
		// 10% as totally random models
		for( i = (int)(num_models/10); i > 0; --i )
		{
			DynSysModel m;

			Polynomial p;
			for( k = 1; k <= (int)GAParams::NumVars(); ++k )
			{
				p.GenerateRandomPolynomial( mMaxTerms );
				m.SetFunction( k, p );
			}
			mGenePool.push_back( m );
		}

	}
}

// Dump the contents of the complexity matrix to the given open file
void GAModel::DumpComplexityMatrix( std::ofstream& out_file )
{
	ComplexityMatrixIter cm_iter =	mCmplxMat.begin();
	ComplexityMatrixRow row;
	ComplexityQuotient r;
	// For each row of the matrix
	while( cm_iter != mCmplxMat.end() )
	{
		row = *cm_iter;
		// For each entry in the row
		ComplexityMatrixRowIter ccr_iter = row.begin();
		while( ccr_iter != row.end() )
		{
			out_file << "{ ";
			// For each quotient in the entry
			ComplexityMatrixEntry cm_entry = *ccr_iter;
			ComplexityMatrixEntryIter cme_iter = cm_entry.begin();
			while( cme_iter != cm_entry.end() )
			{
				r = *cme_iter++;
				out_file << "( " << r.p << " , " << r.q << " )   ";
			}
			out_file << " } ";
			++ccr_iter;
		}
		out_file << std::endl;
		++cm_iter;
	}
}

// Dump max_models models from the gene pool
//		if max_models = 0, dump the entire gene pool
void GAModel::DumpGenePool( std::ofstream& out_file, size_t max_models )
{
	// if max_models is zero, dump everything
	if( max_models == 0 ) max_models = mGenePool.size();

	std::partial_sort( mGenePool.begin(), mGenePool.begin() + max_models,
		mGenePool.end(), IsFirstGreater );

	size_t m = 1;
	GenePoolIter gp_iter = mGenePool.begin();
	while( gp_iter != mGenePool.end() && m <= max_models )
	{
		out_file << "Model:\t" << (int) m << std::endl;

		gp_iter->DumpModel( out_file );

		out_file << std::endl;
		
		++gp_iter; ++m;
	}
}


// Iterate functions in each model in the gene pool to develop a set of scores
void GAModel::IterateModels( )
{
	// Vector of Hamming distances, one for each function
	std::vector<size_t> hk;
	size_t tsl;		// Total number of time steps

	double hamming_ratio;
	size_t th, k, kov;

	TimeSeries r;

	// Initialize minimium and maximum scores to that from function 1 of the first model in the pool
	mMinPolyScore = 1.0;
	mMaxPolyScore = 0.0;

	// std::cout << "Gene Pool size = " << mGenePool.size() << std::endl;

	// For each model in the pool
	GenePoolIter gp_iter = mGenePool.begin();
	while( gp_iter != mGenePool.end() )
	{
		hk.assign( GAParams::NumVars(), 0 );
		tsl = 0;
		th  = 0;

		// Create a reference to the current model
		DynSysModel& m = *gp_iter;
		// for each wild type time series
		TimeSeriesVectorIter wt_iter = mWtSeries.begin();
		while( wt_iter != mWtSeries.end() )
		{
			// for each function in the model
			for( k = 1; k <= GAParams::NumVars(); ++k )
			{
				// if polynomial k is mutated, we need to re-iterate the time series
				if( m[k].IsMutatedQ() )
				{
					// Iterate the k'th function of the model with respect to the given time series
					hk[k-1] += m.Iterate( *wt_iter, k, r );

					// Make sure model mutation is on since at least 1 polynomial is mutated
					m.SetMutated();		// just for insurance
				}
			}
			// Accumulate total time series length (minus the initial state)
			tsl += wt_iter->size() - 1;

			// Now iterate the entire model from the initial state
			//	only if it has changed since last time
			if( m.IsMutatedQ() )
			{
				m.Iterate( wt_iter->front(), wt_iter->size(), r );

				// Compute the Hamming distance between the input time series and that produced by
				//	the candidate model from the same initial state
				th += NTuple::HammingDistance( *wt_iter, r );
			};

			++wt_iter;
		}
		// iterate each function except k'th for knockout series for variable k saving KO Hamming distances
		TimeSeriesVectorSetIter ko_iter = mKoSeriesSet.begin();
		while( ko_iter != mKoSeriesSet.end() )
		{
			kov = ko_iter->first;
			// for each function in the model
			for( k = 1; k <= GAParams::NumVars(); ++k )
			{
				// except for the knock out variable
				if( k != kov )
				{
					if( m[k].IsMutatedQ() )
					{
						// Iterate the k'th function of the model with respect to the given time series
						hk[k-1]  += m.Iterate( ko_iter->second, k, r, kov );
					}
				}
			}
			// Accumulate total time series length (minus the initial state)
			tsl += ko_iter->second.size() - 1;

			// Now iterate the entire model from the initial state
			if( m.IsMutatedQ( ) )
			{
				// Now iterate the entire model from the initial state using the knockout 
				m.KoIterate( ko_iter->second.front(), ko_iter->second.size(), r, kov );

				// Compute the Hamming distance between the input time series and that produced by
				//	the candidate model from the same initial state
				th += NTuple::HammingDistance( ko_iter->second, r );
			}
			// next KO time series
			++ko_iter;
		}

		// (3) compute a composite score for each function of the model based on:
		//		- Hamming distances, complexity( max-degree), reverse matrix, bio-matrix
		// Compute the single variable Hamming scores
		for( k = 1; k <= GAParams::NumVars(); ++k )
		{
			if( m[k].IsMutatedQ() )
			{
				// Polynomial Hamming score = total bits wrong in this variable / total time steps over all time series
				hamming_ratio = (double) hk[k-1] / (double) tsl;
				m.SetPolyHamming( k, hamming_ratio );
			}
		}
		// Model Hamming score = total bits wrong in all variables / total time steps * variables per time step
		if( m.IsMutatedQ() )
		{
			hamming_ratio = (double) th / (double) (GAParams::NumVars() * tsl );
			m.SetHamming( hamming_ratio );
		}
	
		// Set the complexity score for each polynomial of the model
		if( GAParams::ComplexityWeight() > 0.0 )
		{
			m.SetPolyComplexities();
		}

		// Set the dependency graph score for each polnomial in the model
		if( GAParams::DepGraphWeight() > 0.0 && mCmplxMat.size() == GAParams::NumVars() )
		{
			// TBD - we need to hook this up when we have a formula
		}

		// Compute the reverse engineering score for each polynomial of the model
		if( mRevWeightMatrix.size() == GAParams::NumVars() )
		{
			m.ScoreReverseEngineeringWeights( mRevWeightMatrix );
		}

		// Compute the reverse engineering score for each polynomial of the model
		if( mBioDepMatrix.size() == GAParams::NumVars() )
		{
			m.ScoreBioProbabilities( mBioDepMatrix );
		}

		// Update the minimum and maximum scores using all functions in this model
		double s;
		for( k = 1; k <= GAParams::NumVars(); ++k )
		{
			// also computes overall score for the k'th polynomial
			// *** This is valid only because we have already computed   ***
			// *** all of the individual polynomial scores above         ***
			s = m.GetPolyProb( k );
			if( s < mMinPolyScore ) mMinPolyScore = s;
			if( s > mMaxPolyScore ) mMaxPolyScore = s;
		}
		
		// Get the model overall score and track min / max
		s = m.GetScore( );
		if( s < mMinModelScore ) mMinModelScore = s;
		if( s > mMaxModelScore ) mMaxModelScore = s;

		// We have finished scoring this model, mark it as no longer mutated
		//		this must happen after calling GetPolyProb for each changed polynomial
		m.UnMutate();

		// Next model from the pool
		++gp_iter;
	}
}

// Create a candidate set from the best polynomials in the current gene pool
void GAModel::CreateCandidateSet()
{
	size_t pool_size = mGenePool.size();
	size_t i,k;

	mCandidates.clear();

	GenePool temp_pool;

	// Partial Sort to find the best NumParentsToPreserve models
	size_t np = GAParams::NumParentsToPreserve(); 
	std::nth_element( mGenePool.begin(), mGenePool.begin() + np + 2,
		mGenePool.end(), IsFirstGreater );

	// Get rid of identical models near the top
	// std::unique processes the given range of sorted entries so that
	// the first k entries are distinct elements with no duplicates, 
	// followed by the duplicates, if any, in no particular order
	// see http://www.sgi.com/tech/stl/unique.html
	GenePoolIter gp_iter_d = std::unique( mGenePool.begin(), 
		mGenePool.begin() + np + 2 );

	// gp_iter_d now points 1 past the last non-duplicate entry
	// Copy these distinct entries to the temporary pool
	GenePoolIter gp_iter = mGenePool.begin();
	while( gp_iter != gp_iter_d )
	{
		temp_pool.push_back( *gp_iter );
		++gp_iter;
	}

	// Compute the range of the polynomial scores
	double delta_score = mMaxPolyScore - mMinPolyScore;

	// If delta score is vanishingly small - we have a problem
	if( delta_score < 1e-9 )
	{
		throw( "GAModel::CreateNextGeneration() - range of scores over all polynomials in the gene pool is too close to zero" );
	}

	// Dynamically allocated array of lists of polynomials
	PolyModel* good_polys = new PolyModel[ GAParams::NumVars() ];

	size_t r = GAParams::NumCandidates();

	// for each variable
	for( k = 1; k <= GAParams::NumVars(); ++k )
	{
		// Copy the polynomial for that variable from each model in the gene pool
		// onto the good_poly array
		GenePoolIter gp_iter = mGenePool.begin();
		while( gp_iter != mGenePool.end() )
		{
			good_polys[k-1].push_back( (*gp_iter)[k] );
			++gp_iter;
		}
		// Partial sort to select the best r polynomials for this variable
		std::nth_element( good_polys[k-1].begin(), good_polys[k-1].begin() + r,
				good_polys[k-1].end(), IsFirstGreaterPoly );

		// Throw aways the ones we don't need
		good_polys[k-1].erase( good_polys[k-1].begin() + r, good_polys[k-1].end() );
	}

	// For each set of polynomials in good_polys
	for( i = 1; i <= r; ++i )
	{
		// Make a new model
		DynSysModel new_model;
		for( k = 1; k <= GAParams::NumVars(); ++k )
		{
			new_model.SetFunction( k, good_polys[k-1][i-1] );
		}
		// Save the new model as a member of the candidate polynomial pool
		mCandidates.push_back( new_model );
	}

	// Free up the dynamically allocated memory
	delete [] good_polys;

	// Now that we have a set of candidates, we can wipe out the previous generation,
	//	except for the few we want to preserve to the next
	mGenePool.clear();
	mGenePool.assign( temp_pool.begin(), temp_pool.end() );
}

// Re-populate the gene pool via inheritance, crossover and mutation
//   using the candidate set as parents
void GAModel::CreateNextGeneration()
{
	size_t i,j, k;
	mNumMutations = 0;

	// 5% totally random models
	// std::cout << "Generating Random Models" << std::endl;
	for( i = GAParams::GenePoolSize()/20; i > 0; --i )
	{
		DynSysModel m;

		Polynomial p;
		for( k = 1; k <= (int)GAParams::NumVars(); ++k )
		{
			p.GenerateRandomPolynomial( mMaxTerms );
			m.SetFunction( k, p );
		}
		mGenePool.push_back( m );
	}

	// std::cout << "Gene Pool Size = " << GAParams::GenePoolSize() << std::endl;
	// Until we have enough children
	while( mGenePool.size() <= GAParams::GenePoolSize() )
	{
		// Make a new model
		DynSysModel new_model;

		// Effect crossover by selecting random polynomials from the candidates
		for( k = 1; k <= GAParams::NumVars(); ++k )
		{
			// Choose a random model in the candidate set
			j = PolyMathRand::RandInt( 1, (int) mCandidates.size() );

			// Copy the k'th polynomial from the randomly selected model
			new_model.SetFunction( k, mCandidates[j-1][k] );

		}

		// Modulate the mutation rate by the average fitness of the polynomials in this model
		double mutate_prob = GAParams::MutateProbability()
				* ( 0.1 / (0.101 + new_model.GetAvgPolyScore() ) );

		// std::cout << "mutate_prob = " << mutate_prob << std::endl;

		// with probability mutate_prob
		while( PolyMathRand::RandReal() < mutate_prob  )
		{
			++mNumMutations;
			new_model.Mutate( );
		}

		// Save the new model in the gene pool
		mGenePool.push_back( new_model );

	}
}

void GAModel::Randomize( int seed )
{
	// Make sure each run is different!
	PolyMathRand::Randomize( seed );
}

// Run - run the G.A. from the initial state until convergence or max_iter
void GAModel::Run( char * best_models_filename)
{
	// Populate the initial gene pool
	FillGenePool( GAParams::GenePoolSize() );

	// How much the max score has to change to be recognized as better
	double epsilon  = 1e-4;

#ifdef GA_DEBUG_FILE
	std::ofstream debug_file( "DebugModels.txt" );
#endif 
	std::cout << "Running first generation" << std::endl;

	// For up to MaxGenerations
	size_t generation = 1;
	while( true )
	{
		// Iterate each model in the gene pool, producing scores for each one
		IterateModels();		

#ifdef GA_DEBUG_FILE
//		DumpGenePool( debug_file, 5*GAParams::NumParentsToPreserve() );
#endif
		std::cout << "Generation: " << (int) generation << "\t Best score: " << mMaxModelScore;

		if( generation >= GAParams::MaxGenerations() ) break;
		// Test for convergence
		if( fabs( mMaxModelScore - mPrevBestScore ) < epsilon )
		{
			if( ++num_stable_generations > GAParams::StableGenerationLimit() ) break;
		}
		else
		{
			num_stable_generations = 1;
			if( mMaxModelScore > mPrevBestScore ) mPrevBestScore = mMaxModelScore;
		}

		// std::cout << "Creating Candidate Set" << std::endl;
		// Create a set of candidate genomes for production of the next generation
		CreateCandidateSet( );

		// std::cout << "Creating Next Generation" << std::endl;
		// Create next generation by mutation and crossover of the candidates
		CreateNextGeneration();

			
			
		std::cout << "\tMutations:" << (int) mNumMutations << std::endl;

		++generation;
	}
#ifdef GA_DEBUG_FILE
	debug_file.close(); 
#endif
	// Select best 'r' models and output them to a result file
	std::ofstream out_file( best_models_filename );		// TBD - get the name from the control file
	DumpGenePool( out_file, 5*GAParams::NumParentsToPreserve() );
	out_file.close();
}

