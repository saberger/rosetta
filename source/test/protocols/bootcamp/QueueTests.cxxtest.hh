// -*- mode:c++;tab-width:2;indent-tabs-mode:t;show-trailing-whitespace:t;rm-trailing-spaces:t -*-
// vi: set ts=2 noet:
//
// (c) Copyright Rosetta Commons Member Institutions.
// (c) This file is part of the Rosetta software suite and is made available under license.
// (c) The Rosetta software is developed by the contributing members of the Rosetta Commons.
// (c) For more information, see http://www.rosettacommons.org. Questions about this can be
// (c) addressed to University of Washington CoMotion, email: license@uw.edu.

/// @file  protocols/bootcamp/QueueTests.cxxtest.hh
/// @brief  Tests for the Queue class
/// @author saberger (sarah.berger01@gmx.at)


// Test headers
#include <test/UMoverTest.hh>
#include <test/UTracer.hh>
#include <cxxtest/TestSuite.h>
#include <test/util/pose_funcs.hh>
#include <test/core/init_util.hh>


// Project Headers
#include <protocols/bootcamp/Queue.hh>

// Core Headers
#include <core/pose/Pose.hh>
#include <core/import_pose/import_pose.hh>

// Utility, etc Headers
#include <basic/Tracer.hh>

static basic::Tracer TR("QueueTests");


class QueueTests : public CxxTest::TestSuite {
private:
    protocols::bootcamp::Queue queue;

public:
    void test_enqueue() {
        TS_TRACE("Testing enqueue method");
        queue.enqueue("A");
        queue.enqueue("B");
        TS_ASSERT_EQUALS(queue.size(), 2);
        TS_ASSERT(!queue.is_empty());
    }

    void test_dequeue() {
        TS_TRACE("Testing dequeue method");
        queue.enqueue("A");
        queue.enqueue("B");
        std::string first = queue.dequeue();
        TS_ASSERT_EQUALS(first, "A");
        TS_ASSERT_EQUALS(queue.size(), 1);
        std::string second = queue.dequeue();
        TS_ASSERT_EQUALS(second, "B");
        TS_ASSERT(queue.is_empty());
    }

};