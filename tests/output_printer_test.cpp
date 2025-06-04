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
