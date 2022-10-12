#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include "calc_core.h"

namespace s21 {

constexpr double kEps = 1e-8;

class Controller {
 private:
  Model core_{};
  double param_{}, left_{-5}, right_{5};
  int count_{};
  std::vector<double> graph_x_{}, graph_y_{};

 public:
  void InputString(std::string src) { core_.ChangeExpr(src); }
  void SetParam(double src) { param_ = src; }
  void SetLeft(double src) { left_ = src; }
  void SetRight(double src) { right_ = src; }
  void SetCount(int src) { count_ = src; }
  double GetParam() { return param_; }
  std::vector<double> const &GetGraphX() { return graph_x_; }
  std::vector<double> const &GetGraphY() { return graph_y_; }
  bool NeedsGraph() { return core_.HasVarX(); }

  double CalculateSingle() { return core_.CalcProc(param_); }

  void CalculateGraph() {
    graph_x_.clear();
    graph_y_.clear();
    double step = (right_ - left_) / (count_ - 1);
    double coord = left_;
    for (int i{}; i < count_; i++) {
      graph_x_.push_back(coord);
      graph_y_.push_back(core_.CalcProc(coord));
      coord += step;
    }
  }
};

}  // namespace s21

#endif  // SRC_CONTROLLER_H_
