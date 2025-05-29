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
  EXPECT_EQ(map.get_map()[0][0].ground.lock()->get_type(), EntityType::WALL);
  EXPECT_EQ(map.get_map()[0][9].ground.lock()->get_type(), EntityType::WALL);
  EXPECT_EQ(map.get_map()[9][0].ground.lock()->get_type(), EntityType::WALL);
  EXPECT_EQ(map.get_map()[9][9].ground.lock()->get_type(), EntityType::WALL);
  // Test player 1 position
  EXPECT_EQ(map.get_map()[1][3].actor.lock()->get_id(), 1);

  // Test player 2 position
  EXPECT_EQ(map.get_map()[8][6].actor.lock()->get_id(), 2);
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
  EXPECT_EQ(map.get_map()[1][1].actor.lock()->get_id(), 1);
  EXPECT_EQ(map.get_map()[1][2].actor.lock()->get_id(), 2);
  EXPECT_EQ(map.get_map()[1][3].actor.lock()->get_id(), 3);
  EXPECT_EQ(map.get_map()[2][1].actor.lock()->get_id(), 4);
  EXPECT_EQ(map.get_map()[2][2].actor.lock()->get_id(), 5);
  EXPECT_EQ(map.get_map()[2][3].actor.lock()->get_id(), 6);
  EXPECT_EQ(map.get_map()[3][1].actor.lock()->get_id(), 7);
  EXPECT_EQ(map.get_map()[3][2].actor.lock()->get_id(), 8);
  EXPECT_EQ(map.get_map()[3][3].actor.lock()->get_id(), 9);
}


// Tests map parser correctly ignore row / columns beyond declared dimensions
TEST(MapTest, ExtraRowsAndColsAreIgnored) {
  GameManager game_manager;

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path = current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" / "maps" / "test_map_cols_overflow.txt";

  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);

  Map map = game_manager.get_map();
  EXPECT_EQ(map.get_rows(), 4);  // declared size
  EXPECT_EQ(map.get_cols(), 10);

  // Ensure extra column is ignored
  EXPECT_EQ(map.get_map()[0].size(), 10);
}


// checks if spaces around equal sign are parsed correctly:
TEST(MapTest, ValidWhitespaceMapParsesCorrectly) {
  GameManager game_manager;
  auto path = std::filesystem::path(__FILE__).parent_path().parent_path()
            / "files" / "tests" / "maps" / "test_map_whitespace.txt";

  int result = game_manager.load_map(path.string());
  EXPECT_EQ(result, 0);

  Map map = game_manager.get_map();
  EXPECT_EQ(map.get_max_steps(), 3000);
  EXPECT_EQ(map.get_num_shells(), 15);
  EXPECT_EQ(map.get_rows(), 5);
  EXPECT_EQ(map.get_cols(), 5);

  // Spot-check some tank positions
  EXPECT_EQ(map.get_map()[1][1].actor.lock()->get_id(), 1);
  EXPECT_EQ(map.get_map()[2][2].actor.lock()->get_id(), 5);
  EXPECT_EQ(map.get_map()[3][3].actor.lock()->get_id(), 9);
}

// Tests missing Meta data line 
TEST(MapTest, MissingMetadataLineFails) {
  GameManager game_manager;

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path = current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path = project_root_path / "files" / "tests" / "maps" / "missing_rows_map.txt";

  int result = game_manager.load_map(map_file_path.string());
  EXPECT_EQ(result, -1); // assuming -1 on error
}
