#pragma once

#include <vector>
#include <string>
#include <tuple>
#include "../common/ActionRequest.h"

class OutputPrinter {
public:
    explicit OutputPrinter(size_t numTanks);

    void setTankAction(size_t tankIndex, ActionRequest action);
    void markTankIgnored(size_t tankIndex);
    void markTankKilled(size_t tankIndex);

    void finalizeRound();

    void logResult(const std::vector<int>& remainingTanksPerPlayer,
                   int maxSteps,
                   int winningPlayer = -1,
                   bool maxStepsReached = false,
                   bool zeroShellsTimeout = false
                   );

    void writeToFile(const std::string& filename);

private:
    std::vector<std::tuple<ActionRequest, bool, bool>> tankStates_;
    std::vector<std::string> roundLines_;
};
