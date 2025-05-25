#include "../common/GameManager.h"
#include "../common/Map.h"
#include <filesystem>
#include <gtest/gtest.h>

TEST(MapTest, TestMapInitialization) {
  GameManager game_manager;

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "maps" / "test_map_1.txt";

  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  Map map = game_manager.get_map();

  EXPECT_EQ(map.get_map().size(), 10);
  EXPECT_EQ(map.get_map()[0].size(), 10);
  EXPECT_EQ(map.get_map()[0][0], '#');
  EXPECT_EQ(map.get_map()[0][9], '#');
  EXPECT_EQ(map.get_map()[9][0], '#');
  EXPECT_EQ(map.get_map()[9][9], '#');
  // Test player 1 position
  EXPECT_EQ(map.get_map()[1][3], '1');

  // Test player 2 position
  EXPECT_EQ(map.get_map()[8][6], '2');
}

TEST(MapTest, TestSymbolDetection) {
  GameManager game_manager;
  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "maps" / "test_map_symbol.txt";
  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  Map map = game_manager.get_map();
  EXPECT_EQ(map.get_map()[1][1], '1');
  EXPECT_EQ(map.get_map()[1][2], '2');
  EXPECT_EQ(map.get_map()[1][3], '3');
  EXPECT_EQ(map.get_map()[2][1], '4');
  EXPECT_EQ(map.get_map()[2][2], '5');
  EXPECT_EQ(map.get_map()[2][3], '6');
  EXPECT_EQ(map.get_map()[3][1], '7');
  EXPECT_EQ(map.get_map()[3][2], '8');
  EXPECT_EQ(map.get_map()[3][3], '9');
}