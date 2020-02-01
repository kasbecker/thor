// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>
#define SKIP_TEST *boost::unit_test::disabled()
using namespace std;

const MapCheckpoints* makeCheckpoints()
{
    uint256 p259201 = uint256("0x1c9121bf9329a6234bfd1ea2d91515f19cd96990725265253f4b164283ade5dd");
    uint256 p623933 = uint256("0xc7aafa648a0f1450157dc93bd4d7448913a85b7448f803b4ab970d91fc2a7da7");
    return new MapCheckpoints{{259201,p259201},{623933,p623933}};
}

static bool wasSet = false;
static CCheckpointData checkpointData;

const CCheckpointData& checkpoint_data_test()
{
    checkpointData = CCheckpointData {makeCheckpoints(),0,0,0.0};
    wasSet = true;
    return checkpointData;
}

void clearCheckpointData()
{
    if(wasSet && !checkpointData.mapCheckpoints)
    {
        delete checkpointData.mapCheckpoints;
        wasSet = false;
        checkpointData = CCheckpointData();
    }
}

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    CCheckpoints checkpointsService(checkpoint_data_test);
    uint256 p259201 = uint256("0x1c9121bf9329a6234bfd1ea2d91515f19cd96990725265253f4b164283ade5dd");
    uint256 p623933 = uint256("0xc7aafa648a0f1450157dc93bd4d7448913a85b7448f803b4ab970d91fc2a7da7");
    BOOST_CHECK(checkpointsService.CheckBlock(259201, p259201));
    BOOST_CHECK(checkpointsService.CheckBlock(623933, p623933));


    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!checkpointsService.CheckBlock(259201, p623933));
    BOOST_CHECK(!checkpointsService.CheckBlock(623933, p259201));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(checkpointsService.CheckBlock(259201+1, p623933));
    BOOST_CHECK(checkpointsService.CheckBlock(623933+1, p259201));

    BOOST_CHECK(checkpointsService.GetTotalBlocksEstimate() >= 623933);
    clearCheckpointData();
}

BOOST_AUTO_TEST_SUITE_END()
