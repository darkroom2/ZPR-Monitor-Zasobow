#pragma once

#include <memory>
#include <iostream>
#include <tuple>

#include "../Providers/ResProvider.h"

class PollManager {

    std::unique_ptr<ResProvider> _resProvider;

public:
    PollManager(std::unique_ptr<ResProvider> resProvider);

    std::tuple<int, int, int> getUsages();
};
