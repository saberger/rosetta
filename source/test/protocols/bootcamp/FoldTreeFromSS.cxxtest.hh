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
#include <iostream>
#include <string>
#include <cassert>

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
utility::vector1< std::pair< core::Size, core::Size > >
identify_secondary_structure_spans( std::string const & ss_string )
{
  utility::vector1< std::pair< core::Size, core::Size > > ss_boundaries;
  core::Size strand_start = -1;
  for ( core::Size ii = 0; ii < ss_string.size(); ++ii ) {
    if ( ss_string[ ii ] == 'E' || ss_string[ ii ] == 'H'  ) {
      if ( int( strand_start ) == -1 ) {
        strand_start = ii;
      } else if ( ss_string[ii] != ss_string[strand_start] ) {
        ss_boundaries.push_back( std::make_pair( strand_start+1, ii ) );
        strand_start = ii;
      }
    } else {
      if ( int( strand_start ) != -1 ) {
        ss_boundaries.push_back( std::make_pair( strand_start+1, ii ) );
        strand_start = -1;
      }
    }
  }
  if ( int( strand_start ) != -1 ) {
    // last residue was part of a ss-eleemnt                                                                                                                                
    ss_boundaries.push_back( std::make_pair( strand_start+1, ss_string.size() ));
  }
  for ( core::Size ii = 1; ii <= ss_boundaries.size(); ++ii ) {
    std::cout << "SS Element " << ii << " from residue "
      << ss_boundaries[ ii ].first << " to "
      << ss_boundaries[ ii ].second << std::endl;
  }
  return ss_boundaries;
}
// Assuming the function signature is something like this:
std::vector<std::pair<int, int>> parse_secondary_structure(const std::string& ss_string);

void test_parse_secondary_structure() {
    // Test case 1
    std::string ss_string1 = "   EEEEE   HHHHHHHH  EEEEE   IGNOR EEEEEE   HHHHHHHHHHH  EEEEE  HHHH   ";
    std::vector<std::pair<int, int>> expected1 = {
        {4, 8}, {12, 19}, {22, 26}, {36, 41}, {45, 55}, {58, 62}, {65, 68}
    };
    std::vector<std::pair<int, int>> result1 = parse_secondary_structure(ss_string1);
    assert(result1.size() == expected1.size() && "Mismatch in number of secondary structure elements (Test 1)");
    for (size_t i = 0; i < expected1.size(); ++i) {
        assert(result1[i] == expected1[i] && "Mismatch in secondary structure boundaries (Test 1)");
    }

    // Test case 2
    std::string ss_string2 = "HHHHHHH   HHHHHHHHHHHH      HHHHHHHHHHHHEEEEEEEEEEHHHHHHH EEEEHHH ";
    std::vector<std::pair<int, int>> expected2 = {
        {1, 7}, {11, 22}, {29, 40}, {41, 50}, {51, 57}, {59, 62}, {63, 65}
    };
    std::vector<std::pair<int, int>> result2 = parse_secondary_structure(ss_string2);
    assert(result2.size() == expected2.size() && "Mismatch in number of secondary structure elements (Test 2)");
    for (size_t i = 0; i < expected2.size(); ++i) {
        assert(result2[i] == expected2[i] && "Mismatch in secondary structure boundaries (Test 2)");
    }

    // Test case 3
    std::string ss_string3 = "EEEEEEEEE EEEEEEEE EEEEEEEEE H EEEEE H H H EEEEEEEE";
    std::vector<std::pair<int, int>> expected3 = {
        {1, 9}, {11, 18}, {20, 28}, {30, 30}, {32, 36}, {38, 38}, {40, 40}, {42, 42}, {44, 51}
    };
    std::vector<std::pair<int, int>> result3 = parse_secondary_structure(ss_string3);
    assert(result3.size() == expected3.size() && "Mismatch in number of secondary structure elements (Test 3)");
    for (size_t i = 0; i < expected3.size(); ++i) {
        assert(result3[i] == expected3[i] && "Mismatch in secondary structure boundaries (Test 3)");
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_parse_secondary_structure();
    return 0;
}


};
