#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QDoubleValidator>
#include <QMainWindow>
#include <QRegularExpression>
#include <QVector>

#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

  s21::Controller control_;
  QDoubleValidator validator_;
  double x_min_{-5}, x_max_{5}, y_min_{-5}, y_max_{5}, answer_{};

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_button_sin_clicked();
  void on_button_cos_clicked();
  void on_button_tan_clicked();
  void on_button_ln_clicked();
  void on_button_asin_clicked();
  void on_button_acos_clicked();
  void on_button_atan_clicked();
  void on_button_log_clicked();
  void on_button_left_par_clicked();
  void on_button_right_par_clicked();
  void on_button_sqrt_clicked();
  void on_button_plus_clicked();
  void on_button_7_clicked();
  void on_button_8_clicked();
  void on_button_9_clicked();
  void on_button_x_clicked();
  void on_button_mod_clicked();
  void on_button_minus_clicked();
  void on_button_4_clicked();
  void on_button_5_clicked();
  void on_button_6_clicked();
  void on_button_mult_clicked();
  void on_button_1_clicked();
  void on_button_2_clicked();
  void on_button_3_clicked();
  void on_button_div_clicked();
  void on_button_pow_clicked();
  void on_button_0_clicked();
  void on_button_dot_clicked();
  void on_button_ca_clicked();
  void on_button_eq_clicked();
  void DrawGraph();
  void DrawCross();
  void on_x_edit_editingFinished();
  void on_x_min_edit_editingFinished();
  void on_x_max_edit_editingFinished();
  void on_y_min_edit_editingFinished();
  void on_y_max_edit_editingFinished();

 private:
  Ui::MainWindow *ui;
  QVector<double> fromStdVector(std::vector<double>);
  void FormAnswer();
};
#endif  // SRC_MAINWINDOW_H_
