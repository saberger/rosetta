// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file   test/protocols/match/ProteinSCSampler.cxxtest.hh
/// @brief
/// @author Andrew Leaver-Fay (aleaverfay@gmail.com)


// Test headers
#include <cxxtest/TestSuite.h>
#include <test/util/pose_funcs.hh>
#include <test/core/init_util.hh>
#include <utility> // for std::pair
#include <vector>  // for std::vector

// Utility headers

/// Project headers
#include <core/types.hh>

// C++ headers

//Auto Headers
#include <core/pack/dunbrack/DunbrackRotamer.hh>




// --------------- Test Class --------------- //

class FoldTreeFromSSTests: public CxxTest::TestSuite {

public:


	// --------------- Fixtures --------------- //

	// Define a test fixture (some initial state that several tests share)
	// In CxxTest, setUp()/tearDown() are executed around each test case. If you need a fixture on the test
	// suite level, i.e. something that gets constructed once before all the tests in the test suite are run,
	// suites have to be dynamically created. See CxxTest sample directory for example.


	// Shared initialization goes here.
	void setUp() {
		core_init();
	}
	void test_hello_world() {
	TS_ASSERT( true );
	}
	void find_ones_block( utility::vector1< int > const & bitstring, std::vector< std::pair<int, int> > & ranges ) {
    int start = 0;
    for ( uint ii = 1; ii <= bitstring.size(); ++ii ) {
        if ( start != 0 ) {
            if ( bitstring[ ii ] != 1 ) {
                ranges.emplace_back(start, ii - 1); // Add the range to the list
                start = 0;
            }
        } else {
            if ( bitstring[ ii ] == 1 ) {
                start = ii;
            }
        }
    }
    // If the last block ends at the end of the vector
    if ( start != 0 ) {
        ranges.emplace_back(start, bitstring.size());
    }
}
};
