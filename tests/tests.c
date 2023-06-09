#include <check.h>

#include "../Backend/s21_3DViewer.h"

START_TEST(parser_cube) {
  obj_data Data;
  int error;
  int res_numV = 8, res_numF = 12;
  double res_V[8 * 3] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5,
                         -0.5, -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,
                         -0.5, 0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5};
  int res_F[12][3] = {{1, 7, 5}, {1, 3, 7}, {1, 4, 3}, {1, 2, 4},
                      {3, 8, 7}, {3, 4, 8}, {5, 7, 8}, {5, 8, 6},
                      {1, 5, 6}, {1, 6, 2}, {2, 6, 8}, {2, 8, 4}};
  error = fileReading(&Data, "obj_files/cube_test.obj");
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(Data.num_V, res_numV);
  ck_assert_int_eq(Data.num_F, res_numF);
  for (int i = 0; i < res_numV * 3; i++) {
    ck_assert_double_eq_tol(Data.V[i], res_V[i], eps);
  }
  for (int i = 0; i < res_numF; i++) {
    for (int j = 0; j < Data.F[i].num_vertices; j++)
      ck_assert_int_eq(Data.F[i].vertices_on_polygon[j], res_F[i][j]);
  }
  delete_data(&Data);
}
END_TEST

START_TEST(parser_error) {
  obj_data Data;
  int error;
  error = fileReading(&Data, "obj_files/memory.obj");
  ck_assert_int_eq(error, ERROR_FILE_STRUCT);
  delete_data(&Data);
}
END_TEST

START_TEST(rotation_X) {
  obj_data Data;
  int error;
  int res_numV = 8, res_numF = 12;
  double res_V[8 * 3] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5,
                         -0.5, -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,
                         -0.5, 0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5};
  error = fileReading(&Data, "obj_files/cube_test.obj");
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(Data.num_V, res_numV);
  ck_assert_int_eq(Data.num_F, res_numF);
  rotX(&Data, 90);
  rotX(&Data, -90);
  for (int i = 0; i < res_numV * 3; i++) {
    ck_assert_double_eq_tol(Data.V[i], res_V[i], eps);
  }
  delete_data(&Data);
}
END_TEST

START_TEST(rotation_Y) {
  obj_data Data;
  int error;
  int res_numV = 8, res_numF = 12;
  double res_V[8 * 3] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5,
                         -0.5, -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,
                         -0.5, 0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5};
  error = fileReading(&Data, "obj_files/cube_test.obj");
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(Data.num_V, res_numV);
  ck_assert_int_eq(Data.num_F, res_numF);
  rotY(&Data, 20);
  rotY(&Data, -20);
  for (int i = 0; i < res_numV * 3; i++) {
    ck_assert_double_eq_tol(Data.V[i], res_V[i], eps);
  }
  delete_data(&Data);
}
END_TEST

START_TEST(rotation_Z) {
  obj_data Data;
  int error;
  int res_numV = 8, res_numF = 12;
  double res_V[8 * 3] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5,  -0.5, 0.5,
                         -0.5, -0.5, 0.5,  0.5,  0.5,  -0.5, -0.5, 0.5,
                         -0.5, 0.5,  0.5,  0.5,  -0.5, 0.5,  0.5,  0.5};
  error = fileReading(&Data, "obj_files/cube_test.obj");
  ck_assert_int_eq(error, OK);
  ck_assert_int_eq(Data.num_V, res_numV);
  ck_assert_int_eq(Data.num_F, res_numF);
  rotZ(&Data, 70);
  rotZ(&Data, -70);
  for (int i = 0; i < res_numV * 3; i++) {
    ck_assert_double_eq_tol(Data.V[i], res_V[i], eps);
  }
  delete_data(&Data);
}
END_TEST

Suite *Viewer_parser(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("Viewer_parser");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, parser_cube);
  tcase_add_test(tc_core, parser_error);
  tcase_add_test(tc_core, rotation_X);
  tcase_add_test(tc_core, rotation_Y);
  tcase_add_test(tc_core, rotation_Z);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int no_failed;
  Suite *s;
  SRunner *sr;

  s = Viewer_parser();
  sr = srunner_create(s);

  // srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  // srunner_run_all(sr, CK_VERBOSE);
  no_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
