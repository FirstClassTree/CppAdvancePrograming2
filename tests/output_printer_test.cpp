#include "../common/OutputPrinter.h"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdio>

TEST(OutputPrinterTest, BasicRoundLogging) {
    OutputPrinter printer(2);

    printer.setTankAction(0, ActionRequest::MoveForward);
    printer.setTankAction(1, ActionRequest::Shoot);
    printer.markTankIgnored(1);
    printer.finalizeRound();

    printer.setTankAction(0, ActionRequest::DoNothing);
    printer.setTankAction(1, ActionRequest::DoNothing);
    printer.markTankKilled(0);
    printer.finalizeRound();

    std::vector<int> remaining{0, 1};
    printer.logResult(remaining, 2,false,false,100);  // Player 2 won

    const std::string filename = "test_output.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, Shoot (ignored)\n"
        "DoNothing (killed), DoNothing\n"
        "Player 2 won with 1 tanks still alive\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
TEST(OutputPrinterTest, IgnoredAndKilledSameRound) {
    OutputPrinter printer(2);  // Two tanks total

    printer.setTankAction(0, ActionRequest::MoveForward);
    printer.setTankAction(1, ActionRequest::RotateRight90);
    printer.markTankIgnored(1);
    printer.markTankKilled(1);  // Ignored and killed in same round
    printer.finalizeRound();

    std::vector<int> remaining{1, 0};
    printer.logResult(remaining, 1, false, false, 50);  // Player 1 wins

    const std::string filename = "test_ignored_killed.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, RotateRight90 (ignored) (killed)\n"
        "Player 1 won with 1 tanks still alive\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}

TEST(OutputPrinterTest, KilledTankAppearsAsKilledInLaterRounds) {
    OutputPrinter printer(2);  // Two tanks total

    // Round 1: Tank 0 performs valid action, Tank 1 is killed
    printer.setTankAction(0, ActionRequest::Shoot);
    printer.setTankAction(1, ActionRequest::MoveForward);
    printer.markTankKilled(1);
    printer.finalizeRound();

    // Round 2: Only Tank 0 acts, Tank 1 should appear as "killed"
    printer.setTankAction(0, ActionRequest::DoNothing);
    printer.finalizeRound();

    std::vector<int> remaining{1, 0};
    printer.logResult(remaining, 1, false, false, 50);  // Player 1 wins

    const std::string filename = "test_killed_persists.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "Shoot, MoveForward (killed)\n"
        "DoNothing, killed\n"
        "Player 1 won with 1 tanks still alive\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}

TEST(OutputPrinterTest, MaxStepsTie) {
    OutputPrinter printer(4);  // 4 tanks total, 2 per player

    // Simulate a few rounds of actions
    printer.setTankAction(0, ActionRequest::MoveForward);
    printer.setTankAction(1, ActionRequest::MoveForward);
    printer.setTankAction(2, ActionRequest::MoveForward);
    printer.setTankAction(3, ActionRequest::MoveForward);
    printer.finalizeRound();

    printer.setTankAction(0, ActionRequest::DoNothing);
    printer.setTankAction(1, ActionRequest::DoNothing);
    printer.setTankAction(2, ActionRequest::DoNothing);
    printer.setTankAction(3, ActionRequest::DoNothing);
    printer.finalizeRound();

    // Final result: tie due to max steps
    std::vector<int> remaining{1, 1};  // 1 tank each
    printer.logResult(remaining, -1, true, false, 50);  // -1 indicates no winner, maxStepsReached = true

    const std::string filename = "test_max_steps_tie.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, MoveForward, MoveForward, MoveForward\n"
        "DoNothing, DoNothing, DoNothing, DoNothing\n"
        "Tie, reached max steps = 50, player 1 has 1 tanks, player 2 has 1 tanks\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
TEST(OutputPrinterTest, TieDueToMaxStepsReached) {
    OutputPrinter printer(2);  // Two tanks total

    // Round 1: some arbitrary actions
    printer.setTankAction(0, ActionRequest::MoveForward);
    printer.setTankAction(1, ActionRequest::Shoot);
    printer.finalizeRound();

    // Round 2: same actions
    printer.setTankAction(0, ActionRequest::MoveBackward);
    printer.setTankAction(1, ActionRequest::RotateLeft90);
    printer.finalizeRound();

    std::vector<int> remaining{1, 2}; // Player 1 has 1, Player 2 has 2 tanks
    printer.logResult(remaining, -1, true, false, 50); // maxStepsReached = true

    const std::string filename = "test_max_steps_tie.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, Shoot\n"
        "MoveBackward, RotateLeft90\n"
        "Tie, reached max steps = 50, player 1 has 1 tanks, player 2 has 2 tanks\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
TEST(OutputPrinterTest, TieDueToShellTimeout) {
    OutputPrinter printer(2);  // Two tanks total

    // Round 1
    printer.setTankAction(0, ActionRequest::DoNothing);
    printer.setTankAction(1, ActionRequest::DoNothing);
    printer.finalizeRound();

    // Round 2
    printer.setTankAction(0, ActionRequest::GetBattleInfo);
    printer.setTankAction(1, ActionRequest::MoveForward);
    printer.finalizeRound();

    std::vector<int> remaining{1, 1}; // Both players still have one tank
    printer.logResult(remaining, -1, false, true, 50); // zeroShellsTimeout = true

    const std::string filename = "test_shell_timeout_tie.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "DoNothing, DoNothing\n"
        "GetBattleInfo, MoveForward\n"
        "Tie, both players have zero shells for 40 steps\n"; // Ensure constant used in implementation

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
TEST(OutputPrinterTest, MultipleTanksWithMixedActions) {
    OutputPrinter printer(4);  // Four tanks total

    // Round 1 actions
    printer.setTankAction(0, ActionRequest::MoveForward);       // Valid
    printer.setTankAction(1, ActionRequest::RotateRight90);     // Valid
    printer.setTankAction(2, ActionRequest::Shoot);             // Ignored
    printer.markTankIgnored(2);
    printer.setTankAction(3, ActionRequest::MoveBackward);      // Killed
    printer.markTankKilled(3);
    printer.finalizeRound();

    // Round 2 actions
    printer.setTankAction(0, ActionRequest::DoNothing);         // Valid
    printer.setTankAction(1, ActionRequest::DoNothing);         // Valid
    printer.setTankAction(2, ActionRequest::DoNothing);         // Valid
    // Tank 3 was killed last round and should show "killed"
    printer.finalizeRound();

    std::vector<int> remaining{2, 1};  // Player 1 has 2 tanks, player 2 has 1
    printer.logResult(remaining, 1, false, false, 50);  // Player 1 wins

    const std::string filename = "test_multiple_tanks.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, RotateRight90, Shoot (ignored), MoveBackward (killed)\n"
        "DoNothing, DoNothing, DoNothing, killed\n"
        "Player 1 won with 2 tanks still alive\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
TEST(OutputPrinterTest, MultiplePlayersTieDueToMaxSteps) {
    OutputPrinter printer(5);  // 5 tanks total

    // Round 1
    printer.setTankAction(0, ActionRequest::MoveForward);
    printer.setTankAction(1, ActionRequest::RotateRight90);
    printer.setTankAction(2, ActionRequest::Shoot);
    printer.setTankAction(3, ActionRequest::MoveBackward);
    printer.setTankAction(4, ActionRequest::GetBattleInfo);
    printer.finalizeRound();

    // Round 2
    printer.setTankAction(0, ActionRequest::DoNothing);
    printer.setTankAction(1, ActionRequest::RotateLeft90);
    printer.setTankAction(2, ActionRequest::DoNothing);
    printer.setTankAction(3, ActionRequest::Shoot);
    printer.setTankAction(4, ActionRequest::DoNothing);
    printer.finalizeRound();

    std::vector<int> remaining{2, 2, 1};  // Per-player tank counts
    printer.logResult(remaining, -1, true, false, 60);  // Tie due to max steps

    const std::string filename = "test_multi_players_max_steps.txt";
    printer.writeToFile(filename);

    std::ifstream inFile(filename);
    std::stringstream contents;
    contents << inFile.rdbuf();

    std::string expected =
        "MoveForward, RotateRight90, Shoot, MoveBackward, GetBattleInfo\n"
        "DoNothing, RotateLeft90, DoNothing, Shoot, DoNothing\n"
        "Tie, reached max steps = 60, player 1 has 2 tanks, player 2 has 2 tanks, player 3 has 1 tanks\n";

    EXPECT_EQ(contents.str(), expected);

    std::remove(filename.c_str());  // Clean up
}
