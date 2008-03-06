
/*********************************************************
 *  Soothsayer, an extensible predictive text entry system
 *  ------------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/

#include <profileManagerTest.h>
#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ProfileManagerTest );

void ProfileManagerTest::setUp()
{
    profileManager = new ProfileManager();
    profile        = 0;
    
    contextTracker = 0;
    selector       = 0;
    predictor      = 0;
}

void ProfileManagerTest::tearDown()
{
    delete predictor;
    delete selector;
    delete contextTracker;

    delete profile;
    delete profileManager;
}

void ProfileManagerTest::testDefaultProfile()
{

    std::cout << "ProfileManagerTest::testDefaultProfile()" << std::endl;
    profileManager->buildProfile();
    profile = profileManager->getProfile();
    configuration = profile->get_configuration();

    contextTracker = new ContextTracker(configuration);
    selector = new Selector(configuration, contextTracker);
    predictor = new Predictor(configuration, contextTracker);

    std::cout << "ProfileManagerTest: before testProfile()" << std::endl;
    testProfile();
    std::cout << "Exiting ProfileManagerTest::testDefaultProfile( ...)" << std::endl;

}

void ProfileManagerTest::testNonExistantProfile()
{
    std::cout << "ProfileManagerTest::testNonExistantProfile()" << std::endl;

    // hopefully a file with the following name will not exists
    const std::string wacky_profile("this_IS_a_wAckY_profileName.xml");

    CPPUNIT_ASSERT( !profileManager->loadProfile(wacky_profile) );

    /* This block of code was commented out because ProfileManager
     * will not build a default profile when the specified profile
     * cannot be loaded.

     profile = profileManager->getProfile();

     contextTracker = new ContextTracker(profile);
     selector = new Selector(profile, contextTracker);
     predictor = new Predictor(profile, contextTracker);

    testProfile();
    */
}

void ProfileManagerTest::testProfile()
{
    // test init contextTracker
    CPPUNIT_ASSERT_EQUAL(DEFAULT_MAX_BUFFER_SIZE,
			 contextTracker->getMaxBufferSize());

    // test init predictor
    CPPUNIT_ASSERT_EQUAL(DEFAULT_PREDICT_TIME,
			 predictor->getPredictTime());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_COMBINATION_POLICY,
			 predictor->getCombinationPolicy());

    // test init selector
    CPPUNIT_ASSERT_EQUAL(DEFAULT_SUGGESTIONS,
			 selector->suggestions());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_REPEAT_SUGGESTION,
			 selector->repeat_suggestions());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_GREEDY_SUGGESTION_THRESHOLD,
			 selector->greedy_suggestion_threshold());
}

void ProfileManagerTest::testCustomProfile()
{
    std::cout << "ProfileManagerTest::testCustomProfile()" << std::endl;

    // create custom profile
    const std::string custom_profile = "custom_profile.xml";
    std::ofstream profile_stream(custom_profile.c_str());
    profile_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
                   << "<Soothsayer>\n"
                   << "    <Custom>CUSTOM</Custom>\n"
                   << "</Soothsayer>\n";
    profile_stream.close();

    profileManager->loadProfile(custom_profile);
    profile = profileManager->getProfile();
    configuration = profile->get_configuration();

    Variable variable("Soothsayer.Custom");

    Value value = configuration->get(variable);

    CPPUNIT_ASSERT_EQUAL(std::string("CUSTOM"), value);

#ifdef HAVE_UNISTD_H
    // remove file
    int result = unlink(custom_profile.c_str());
    assert(result == 0);
#else
    // fail test
    std::string message = "Unable to remove custom profile file " + custom_profile;
    CPPUNIT_FAIL(message.c_str());
#endif

}
