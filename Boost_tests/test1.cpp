#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN //- don't need to repeat this define in more than one cpp file

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <utility>
#include "../Config/ConfigLoader.h"
#include "../Managers/ConditionManager.h"


BOOST_AUTO_TEST_SUITE(Providers)
#ifdef _linux_
    BOOST_AUTO_TEST_CASE(LinuxProvider)
    {

    }
#endif
#ifdef _WIN32
    BOOST_AUTO_TEST_CASE(WindowsProvider)
    {

    }
#endif
    BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Managers)

    BOOST_AUTO_TEST_CASE(CondManagerTest) {
        std::string path(R"(/home/piotr/CLionProjects/zpr_monitor_zasobow/config.cfg)");
        auto cfgLoader = std::make_unique<ConfigLoader>(path);
        auto serverManager = std::make_unique<ServerManager>();

        ResProvider *resProvider = ResProvider::makeResProvider();

        std::unique_ptr<ResProvider> uqResProvider(resProvider);

        auto pollManager = std::make_unique<PollManager>(std::move(uqResProvider));
        auto cndManager = std::make_unique<ConditionManager>(std::move(cfgLoader), std::move(pollManager),
                                                             std::move(serverManager));

        BOOST_CHECK_EQUAL(cndManager->conditionMet(std::make_tuple(50, 50, 50)),true);
        BOOST_CHECK_EQUAL(cndManager->conditionMet(std::make_tuple(40, 40, 40)),false);
    }

    BOOST_AUTO_TEST_CASE(PollManagerTest) {

    }

    BOOST_AUTO_TEST_CASE(ServerManagerTest) {

    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Config)
    std::string path(R"(/home/piotr/CLionProjects/zpr_monitor_zasobow/config.cfg)");
    auto testConfigLoader = std::make_unique<ConfigLoader>(path);

    BOOST_AUTO_TEST_CASE(ConfigLoaderTestIsNumber) {
        BOOST_CHECK_EQUAL(testConfigLoader->isNumber(std::string("123")),true);
        BOOST_CHECK_EQUAL(testConfigLoader->isNumber(std::string("123abc")),false);
        BOOST_CHECK_EQUAL(testConfigLoader->isNumber(std::string("abc")),false);
    }

    BOOST_AUTO_TEST_CASE(ConfigLoaderTestValidValue) {
        BOOST_CHECK_EQUAL(testConfigLoader->isValidValue(std::string("123")),false);
        BOOST_CHECK_EQUAL(testConfigLoader->isValidValue(std::string("123abc")),false);
        BOOST_CHECK_EQUAL(testConfigLoader->isValidValue(std::string("abc")),false);
        BOOST_CHECK_EQUAL(testConfigLoader->isValidValue(std::string("50")),true);
    }

    BOOST_AUTO_TEST_CASE(ConfigLoaderTestConfigFile) {
        BOOST_CHECK_EQUAL(testConfigLoader->cfgCases.size(), 3);
        BOOST_CHECK_EQUAL(testConfigLoader->cfgCases[0].hdd, 50);
        BOOST_CHECK_EQUAL(testConfigLoader->cfgCases[1].cpu, 0);
        BOOST_CHECK_EQUAL(testConfigLoader->cfgCases[2].ram, 0);
    }

BOOST_AUTO_TEST_SUITE_END()
