#include "../common/OutputPrinter.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include "../common/Constants.h"

OutputPrinter::OutputPrinter(size_t numTanks)
    : tankStates_(numTanks), roundLines_() { }

// Set each tanks action in by tank index and ActionRequest
void OutputPrinter::setTankAction(size_t tankIndex, ActionRequest action) {
    assert(tankIndex < tankStates_.size());
    tankStates_[tankIndex] = std::make_tuple(action, false, false);
}

// If tank action was ignored 
void OutputPrinter::markTankIgnored(size_t tankIndex) {
    assert(tankIndex < tankStates_.size());
    std::get<1>(tankStates_[tankIndex]) = true;
}

// If tank was killed
void OutputPrinter::markTankKilled(size_t tankIndex) {
    assert(tankIndex < tankStates_.size());
    std::get<2>(tankStates_[tankIndex]) = true;
}
// we want to be able to translate ActionRequest to String:
namespace {
    const std::unordered_map<ActionRequest, std::string> actionToString = {
        {ActionRequest::MoveForward, "MoveForward"},
        {ActionRequest::MoveBackward, "MoveBackward"},
        {ActionRequest::RotateLeft90, "RotateLeft90"},
        {ActionRequest::RotateRight90, "RotateRight90"},
        {ActionRequest::RotateLeft45, "RotateLeft45"},
        {ActionRequest::RotateRight45, "RotateRight45"},
        {ActionRequest::Shoot, "Shoot"},
        {ActionRequest::GetBattleInfo, "GetBattleInfo"},
        {ActionRequest::DoNothing, "DoNothing"}
    };
}

// After setting each tank Action for the round, this adds it to the rounds lines 
void OutputPrinter::finalizeRound() {
    std::ostringstream line;
    for (size_t i = 0; i < tankStates_.size(); ++i) {
        const auto& [action, ignored, killed] = tankStates_[i];
        auto it = actionToString.find(action);
        assert(it != actionToString.end());

        line << it->second;
        if (ignored) line << " (ignored)";
        if (killed) line << " (killed)";

        if (i + 1 < tankStates_.size())
            line << ", ";
    }

    roundLines_.push_back(line.str());

    // Prepare for next round
    tankStates_ = std::vector<std::tuple<ActionRequest, bool, bool>>(tankStates_.size());
}

// When game ends log result
void OutputPrinter::logResult(const std::vector<int>& remainingTanksPerPlayer,
                               int winningPlayer,
                               bool maxStepsReached,
                               bool zeroShellsTimeout,
                              int maxSteps) {

    std::ostringstream result;

    if (winningPlayer != -1) {
        int remaining = remainingTanksPerPlayer.at(winningPlayer - 1);
        result << "Player " << winningPlayer << " won with " << remaining << " tanks still alive";
    } else if (zeroShellsTimeout) {
        result << "Tie, both players have zero shells for " << ZERO_SHELLS_GRACE_STEPS << " steps";
    } else if (maxStepsReached) {
        result << "Tie, reached max steps = " << maxSteps;
        for (size_t i = 0; i < remainingTanksPerPlayer.size(); ++i) {
            result << (i == 0 ? ", " : ", ")
                   << "player " << (i + 1) << " has " << remainingTanksPerPlayer[i] << " tanks";
        }
    } else {
        result << "Tie, both players have zero tanks";
    }

    roundLines_.push_back(result.str());
}
// Upon game end write to the file
void OutputPrinter::writeToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    for (const auto& line : roundLines_) {
        outFile << line << '\n';
    }
}
