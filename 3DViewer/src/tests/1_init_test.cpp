#include "test.h"

namespace s21 {

TEST(ParserTest, parserTest) {
  Parser parser;
  parser.initParser("tests/tests_files/no.obj");
  ASSERT_EQ(parser.response, Response::BadFile);
  ASSERT_EQ(parser.vertices.size(), 0);
  ASSERT_EQ(parser.polygons.size(), 0);
  ASSERT_EQ(parser.raw_polygons.size(), 0);
  parser.initParser("tests/tests_files/cube.obj");
  ASSERT_EQ(parser.response, Response::NormalDone);
  ASSERT_EQ(parser.vertices.size(), 8);
  ASSERT_EQ(parser.polygons.size(), 0);
  ASSERT_EQ(parser.raw_polygons.size(), 6);
  parser.initParser("tests/tests_files/cube_norm_text.obj");
  ASSERT_EQ(parser.response, Response::NormalDone);
  ASSERT_EQ(parser.vertices.size(), 8);
  ASSERT_EQ(parser.polygons.size(), 0);
  ASSERT_EQ(parser.raw_polygons.size(), 6);
  parser.initParser("tests/tests_files/cube_mod_vert.obj");
  ASSERT_EQ(parser.response, Response::NormalDone);
  ASSERT_EQ(parser.vertices.size(), 7);
  ASSERT_EQ(parser.polygons.size(), 3);
  ASSERT_EQ(parser.raw_polygons.size(), 3);
  parser.initParser("tests/tests_files/cube_mod_poly.obj");
  ASSERT_EQ(parser.response, Response::NormalDone);
  ASSERT_EQ(parser.vertices.size(), 8);
  ASSERT_EQ(parser.polygons.size(), 1);
  ASSERT_EQ(parser.raw_polygons.size(), 4);
  parser.initParser("tests/tests_files/cube_negative.obj");
  ASSERT_EQ(parser.response, Response::NormalDone);
  ASSERT_EQ(parser.vertices.size(), 8);
  ASSERT_EQ(parser.polygons.size(), 0);
  ASSERT_EQ(parser.raw_polygons.size(), 6);
}

TEST(ControllerTest, badModelTest) {
  Controller controller;

  controller.executeCommand(
      std::make_unique<OpenFileCommand>("tests/tests_files/no.obj"));
  const Model &model = controller.getModel();

  ASSERT_EQ(model.response, Response::BadFile);
  ASSERT_EQ(model.vertices.size(), 0);
  ASSERT_EQ(model.polygons.size(), 0);
  ASSERT_EQ(model.raw_polygons.size(), 0);

  controller.executeCommand(
      std::make_unique<OpenFileCommand>("tests/tests_files/badpoly.obj"));
  ASSERT_EQ(model.response, Response::NormalDone);
  ASSERT_EQ(model.vertices.size(), 3);
  ASSERT_EQ(model.polygons.size(), 1);
  ASSERT_EQ(model.raw_polygons.size(), 0);

  controller.executeCommand(
      std::make_unique<OpenFileCommand>("tests/tests_files/zero.obj"));
  ASSERT_EQ(model.response, Response::NormalDone);
  ASSERT_EQ(model.vertices.size(), 3);
  ASSERT_EQ(model.polygons.size(), 1);
  ASSERT_EQ(model.raw_polygons.size(), 0);
}

TEST(ModelTest, triangulationTest) {
  Controller controller;
  controller.executeCommand(
      std::make_unique<OpenFileCommand>("tests/tests_files/cube.obj"));
  const Model &model = controller.getModel();
  Parser parser;
  parser.initParser("tests/tests_files/cube.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  ASSERT_EQ(model.polygons.size(), 12);
  ASSERT_EQ(model.vertices.size(), 8);
  ASSERT_EQ(model.raw_polygons.size(), 6);
  ASSERT_EQ(parser.polygons.size(), 0);
  ASSERT_EQ(parser.vertices.size(), 8);
  ASSERT_EQ(parser.raw_polygons.size(), 6);
}

TEST(ModelTest, normalizationTest) {
  Controller controller;
  controller.executeCommand(
      std::make_unique<OpenFileCommand>("tests/tests_files/cube.obj"));
  const Model &model = controller.getModel();
  Parser parser;
  parser.initParser("tests/tests_files/cube_norm.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
}
}  // namespace s21