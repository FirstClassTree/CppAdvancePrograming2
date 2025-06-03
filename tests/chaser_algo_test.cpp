#include "../common/GameManager.h"
#include "../common/entities/Tank.h"
#include "ActionRequest.h"
#include <gtest/gtest.h>

TEST(ChaserAlgoTest, TestChaserSimpleVertical) {
  GameManager game_manager;

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" /
                                        "algorithm" / "chaser" /
                                        "test_chaser_vertical.txt";

  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  // simulate the game manager running a step only for one tank.

  int tankIndex = 0;
  int playerIndex = 0;
  auto player = game_manager.get_players()[playerIndex].get();
  TankAlgorithm &ai = game_manager.get_tank(tankIndex, playerIndex)->get_ai();

  auto action = ai.getAction();
  EXPECT_EQ(action, ActionRequest::GetBattleInfo);
  auto satellite = game_manager.create_satellite_view(playerIndex, tankIndex);
  player->updateTankWithBattleInfo(ai, *satellite);
  action = ai.getAction();
  EXPECT_EQ(action, ActionRequest::RotateRight90);
}
