#include "../common/GameManager.h"
#include "../common/entities/Tank.h"
#include <gtest/gtest.h>

TEST(TankTest, TestTankUUID) {
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
  EXPECT_EQ(4, tanks.size());
  EXPECT_EQ(11, tanks[0]->get_uuid());
  EXPECT_EQ(12, tanks[1]->get_uuid());
  EXPECT_EQ(27, tanks[2]->get_uuid());
  EXPECT_EQ(28, tanks[3]->get_uuid());
}

TEST(TankTest, TestTankOwnerId){
      std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" /
                                        "entities" / "tank" /
                                        "test_tanks_owner_id.txt";
  GameManager game_manager;
  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  std::vector<std::shared_ptr<Tank>> tanks = game_manager.get_tanks();
  EXPECT_EQ(6, tanks.size());
  for (int i = 0; i < tanks.size(); i++) {
    EXPECT_EQ((i % 2) +1, tanks[i]->get_owner_id());
  }
}

TEST(TankTest, TestTankIndex){
      std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" /
                                        "entities" / "tank" /
                                        "test_tanks_owner_id.txt";
  GameManager game_manager;
  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  std::vector<std::shared_ptr<Tank>> tanks = game_manager.get_tanks();
  EXPECT_EQ(6, tanks.size());
  for (int i = 0; i < tanks.size(); i++) {
    EXPECT_EQ(int(i/2), tanks[i]->get_tank_id());
  }
}