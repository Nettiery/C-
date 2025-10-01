#include "test.h"

namespace s21 {

TEST(TransformTest, scaleTest) {
  float f = 2.0f;
  static Model model;
  model.openModel("tests/tests_files/cube_norm.obj");
  model.scale(f, false);
  Parser parser;
  parser.initParser("tests/tests_files/cube_x2.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
  model.scale(f, true);
  parser.initParser("tests/tests_files/cube_norm.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
  f = 0.0f;
  model.scale(f, false);
  parser.initParser("tests/tests_files/cube_xk.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
  model.scale(f, true);
  parser.initParser("tests/tests_files/cube_norm.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
}

TEST(TransformTest, translateTest) {
  Vertex move = {2.0f, -1.0f, 0.5f};
  static Model model;
  model.openModel("tests/tests_files/cube_norm.obj");
  model.translate(move, false);
  Parser parser;
  parser.initParser("tests/tests_files/cube_trans.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
  model.openModel("tests/tests_files/cube_norm.obj");
  move = {1.0f, 0.0f, 0.0f};
  model.translate(move, true);
  parser.initParser("tests/tests_files/cube_trans_k.obj");
  ASSERT_EQ(verticesEq(model.vertices, parser.vertices), true);
}

TEST(Rotation, rotateStrategyX) {
  RotateX strategy(10);
  Apply_rotation_strategy affin_transform(&strategy);
  static Model model;
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  affin_transform.apply_rotation(model.vertices);
  Parser parser;
  parser.initParser("tests/tests_files/after_rotate_x.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

TEST(Rotation, rotateStrategyY) {
  RotateY strategy(10);
  Apply_rotation_strategy affin_transform(&strategy);
  static Model model;
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  affin_transform.apply_rotation(model.vertices);
  Parser parser;
  parser.initParser("tests/tests_files/after_rotate_y.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

TEST(Rotation, rotateStrategyZ) {
  RotateZ strategy(10);
  Apply_rotation_strategy affin_transform(&strategy);
  static Model model;
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  affin_transform.apply_rotation(model.vertices);
  Parser parser;
  parser.initParser("tests/tests_files/after_rotate_z.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

TEST(Rotation, rotateMethod) {
  static Model model;
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  Vertex move = {0.0f, 0.0f, 10.0f};
  model.rotate(move, false);
  ASSERT_EQ(model.response, Response::NormalDone);
  Parser parser;
  parser.initParser("tests/tests_files/after_rotate_z.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

TEST(Rotation, rotateMethodDefault) {
  static Model model;
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  Vertex move = {1.0f, 0.0f, 0.0f};
  model.rotate(move, true);
  ASSERT_EQ(model.response, Response::NormalDone);
  Parser parser;
  parser.initParser("tests/tests_files/after_rotate_x.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  move = {0.0f, 1.0f, 0.0f};
  model.rotate(move, true);
  ASSERT_EQ(model.response, Response::NormalDone);
  parser.initParser("tests/tests_files/after_rotate_y.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
  model.openModel("tests/tests_files/before_rotate.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  move = {0.0f, 0.0f, 1.0f};
  model.rotate(move, true);
  ASSERT_EQ(model.response, Response::NormalDone);
  parser.initParser("tests/tests_files/after_rotate_z.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

TEST(ResetTest, resetTest) {
  static Model model;
  model.openModel("tests/tests_files/cube.obj");
  ASSERT_EQ(model.response, Response::NormalDone);
  Parser parser;
  parser.initParser("tests/tests_files/cube_norm.obj");
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
  Vertex move = {1.0f, -3.0f, 8.0f};
  float f = 3.5;
  model.translate(move, false);
  model.scale(f, false);
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), false);
  model.normalization();
  ASSERT_EQ(verticesEq(parser.vertices, model.vertices), true);
}

}  // namespace s21