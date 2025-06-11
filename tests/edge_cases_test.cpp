#include "../common/GameManager.h"
#include <filesystem>
#include <gtest/gtest.h>

// Tests map parser correctly ignore row / columns beyond declared dimensions
TEST(EdgeCaseMaps, SimpleMap1) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "test_map_1.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}

TEST(EdgeCaseMaps, ChaseAndShoot) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "test_map_2.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}


TEST(EdgeCaseMaps, LockedPlayers) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "test_map_3.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}


TEST(EdgeCaseMaps, MultiPlayers) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "multiplayers.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}

TEST(EdgeCaseMaps, ZeroMap) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "zero_map.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}

TEST(EdgeCaseMaps, bigRows) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "test_map_edge_rows.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}

TEST(EdgeCaseMaps, bigCols) {

  std::filesystem::path current_test_file_path = __FILE__;
  std::filesystem::path project_root_path =
      current_test_file_path.parent_path().parent_path();
  std::filesystem::path map_file_path =
      project_root_path / "files" / "tests" / "edge_cases" / "test_map_edge_cols.txt";
  

  GameManager game(GamePlayerFactory{}, MyTankAlgorithmFactory{}, false);
  auto result = game.load_map(map_file_path.string());
  EXPECT_EQ(result, 0);
  game.run();
  EXPECT_EQ(true,true);
}