#include "../common/GameManager.h"
#include "../common/entities/Tank.h"
#include <gtest/gtest.h>

TEST(TankTest, TestTankId) {
  GameManager game_manager;

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" /
                                        "entities" / "tank" /
                                        "test_tanks_id.txt";

  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  std::vector<std::shared_ptr<Tank>> tanks = game_manager.get_tanks();
  EXPECT_EQ(6, tanks.size());
  for (int i = 0; i < 6; i++) {
    EXPECT_EQ(i, tanks[i]->get_tank_id());
  }
}
