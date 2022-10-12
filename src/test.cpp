#include <gtest/gtest.h>

#include "calc_core.h"
#include "controller.h"

TEST(parse_n_sort, simple_test) {
  std::string src("1+2* 3"), res("1 2 3 * + ");
  s21::Model B;
  B.ChangeExpr(src);
  ASSERT_EQ(res.compare(B.ExprToStr()), 0);
}

TEST(parse_n_sort, seq_pow) {
  std::string src("2^3^4"), res("2 3 4 ^ ^ ");
  s21::Model B;
  B.ChangeExpr(src);
  ASSERT_EQ(res.compare(B.ExprToStr()), 0);
}

TEST(parse_n_sort, impl_par) {
  std::string src("sinsqrt2x^2"), res("2 x * sqrt sin 2 ^ ");
  s21::Model B;
  B.ChangeExpr(src);
  ASSERT_EQ(res.compare(B.ExprToStr()), 0);
}

TEST(parse_n_sort, impl_mul) {
  std::string src("(1+2)(3-4)cosx"), res("1 2 + 3 4 - * x cos * ");
  s21::Model B;
  B.ChangeExpr(src);
  ASSERT_EQ(res.compare(B.ExprToStr()), 0);
}

TEST(parse_n_sort, unary) {
  std::string src("+2cos-x"), res("2 u+ x u- cos * ");
  s21::Model B;
  B.ChangeExpr(src);
  ASSERT_EQ(res.compare(B.ExprToStr()), 0);
}

TEST(parse_n_sort, exceptions) {
  s21::Model A;
  EXPECT_THROW(A.ChangeExpr("1qwer"), s21::SyntaxError);
  EXPECT_THROW(A.ChangeExpr("1+2)(3-4)"), s21::SyntaxError);
  EXPECT_THROW(A.ChangeExpr("(1+2)(3-4"), s21::SyntaxError);
  EXPECT_THROW(A.ChangeExpr("sincos"), s21::SyntaxError);
}

TEST(calculation, simple_test) {
  s21::Model A;
  A.ChangeExpr("2+2*2");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 6);
}

TEST(calculation, u_minus) {
  s21::Model A, B;
  A.ChangeExpr("sinx");
  B.ChangeExpr("sin-x");
  ASSERT_DOUBLE_EQ(A.CalcProc(0.5), -B.CalcProc(0.5));
  A.ChangeExpr("cosx");
  B.ChangeExpr("cos-x");
  ASSERT_DOUBLE_EQ(A.CalcProc(0.5), B.CalcProc(0.5));
}

TEST(calculation, all_functions) {
  s21::Model A;
  A.ChangeExpr("cos0+sin0+tan0*acos0-asin0+atan0-sqrt1+ln1-log1");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 0);
  A.ChangeExpr("-1+2*3/2modx");
  ASSERT_DOUBLE_EQ(A.CalcProc(2), 0);
  A.ChangeExpr("2^3^2");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 512);
  A.ChangeExpr("2*3mod2");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 0);
}

TEST(calculation, separator) {
  s21::Model A;
  A.ChangeExpr("0.5");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 0.5);
}

TEST(calculation, expr_persistence) {
  s21::Model A;
  A.ChangeExpr("7/2");
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 3.5);
  ASSERT_DOUBLE_EQ(A.CalcProc(0), 3.5);
}

TEST(calculation, exceptions) {
  s21::Model A;
  A.ChangeExpr("1+");
  ASSERT_THROW(A.CalcProc(0), s21::SyntaxError);
  A.ChangeExpr("*");
  ASSERT_THROW(A.CalcProc(0), s21::SyntaxError);
  A.ChangeExpr("1x2x3x");
  ASSERT_NO_THROW(A.CalcProc(1));
  ASSERT_DOUBLE_EQ(A.CalcProc(1), 6);
  A.ChangeExpr("");
  ASSERT_THROW(A.CalcProc(0), s21::SyntaxError);
}

TEST(controller, single) {
  s21::Controller A;
  A.InputString("-(1+2)(-3*4)sqrtx");
  A.SetParam(4);
  ASSERT_DOUBLE_EQ(A.CalculateSingle(), 72);
}

TEST(controller, table) {
  s21::Controller A;
  A.InputString("x+1");
  A.SetParam(7);
  A.SetLeft(0);
  A.SetRight(5);
  A.SetCount(6);
  A.CalculateGraph();
  std::list<double> res{A.GetGraphY().begin(), A.GetGraphY().end()};
  double j(1);
  for (auto i : res) {
    ASSERT_DOUBLE_EQ(i, j);
    j += 1;
  }
  ASSERT_DOUBLE_EQ(res.size(), 6);
  ASSERT_TRUE(A.NeedsGraph());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
