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

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void text_button_clicked();
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
  QDoubleValidator validator_;

  s21::Controller *control_;
  double x_min_{-5}, x_max_{5}, y_min_{-5}, y_max_{5}, answer_{};

  QVector<double> fromStdVector(std::vector<double>);
  void FormAnswer();
  void ConnectingSlots();
};
#endif  // SRC_MAINWINDOW_H_
