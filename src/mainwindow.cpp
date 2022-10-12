#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->graph_area->xAxis->setRange(-5, 5);
  ui->graph_area->yAxis->setRange(-5, 5);
  validator_.setRange(-1e6, 1e6, 8);
  validator_.setLocale(QLocale("en_US"));
  ui->x_edit->setValidator(&validator_);
  ui->x_min_edit->setValidator(&validator_);
  ui->x_max_edit->setValidator(&validator_);
  ui->y_min_edit->setValidator(&validator_);
  ui->y_max_edit->setValidator(&validator_);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_button_sin_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "sin");
}

void MainWindow::on_button_cos_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "cos");
}

void MainWindow::on_button_tan_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "tan");
}

void MainWindow::on_button_ln_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "ln");
}

void MainWindow::on_button_asin_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "asin");
}

void MainWindow::on_button_acos_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "acos");
}

void MainWindow::on_button_atan_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "atan");
}

void MainWindow::on_button_log_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "log");
}

void MainWindow::on_button_left_par_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "(");
}

void MainWindow::on_button_right_par_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + ")");
}

void MainWindow::on_button_sqrt_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "sqrt");
}

void MainWindow::on_button_plus_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "+");
}

void MainWindow::on_button_7_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "7");
}

void MainWindow::on_button_8_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "8");
}

void MainWindow::on_button_9_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "9");
}

void MainWindow::on_button_x_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "x");
}

void MainWindow::on_button_mod_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "mod");
}

void MainWindow::on_button_minus_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "-");
}

void MainWindow::on_button_4_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "4");
}

void MainWindow::on_button_5_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "5");
}

void MainWindow::on_button_6_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "6");
}

void MainWindow::on_button_mult_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "*");
}

void MainWindow::on_button_1_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "1");
}

void MainWindow::on_button_2_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "2");
}

void MainWindow::on_button_3_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "3");
}

void MainWindow::on_button_div_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "/");
}

void MainWindow::on_button_pow_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "^");
}

void MainWindow::on_button_0_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + "0");
}

void MainWindow::on_button_dot_clicked() {
  ui->expression_edit->setText(ui->expression_edit->text() + ".");
}

void MainWindow::on_button_ca_clicked() {
  ui->expression_edit->clear();
  ui->answer_label->clear();
  ui->x_edit->clear();
  ui->graph_area->clearGraphs();
  ui->graph_area->replot();
}

void MainWindow::on_button_eq_clicked() {
  if (ui->expression_edit->text().isEmpty()) {
    ui->answer_label->clear();
  } else {
    try {
      control_.InputString(ui->expression_edit->text().toStdString());
      if (control_.NeedsGraph()) {
        if (!ui->x_edit->text().isEmpty()) FormAnswer();
        DrawGraph();
      } else {
        ui->graph_area->clearGraphs();
        FormAnswer();
      }
    } catch (s21::SyntaxError const &) {
      ui->answer_label->setText("syntax error");
    } catch (s21::AlgorithmError const &) {
      ui->answer_label->setText("unexpected algorithm error");
    }
  }
}

void MainWindow::FormAnswer() {
  answer_ = control_.CalculateSingle();
  QString tmp =
      QString::number(answer_, 'f', 8).remove(QRegularExpression("0+$"));
  ui->answer_label->setText(tmp.remove(QRegularExpression("\\.$")));
}

void MainWindow::DrawGraph() {
  control_.SetCount(ui->graph_area->width());
  ui->graph_area->clearGraphs();
  ui->graph_area->addGraph();
  control_.CalculateGraph();
  QVector<double> x_data = fromStdVector(control_.GetGraphX());
  QVector<double> y_data = fromStdVector(control_.GetGraphY());
  ui->graph_area->graph(0)->addData(x_data, y_data);
  if (!ui->x_edit->text().isEmpty()) DrawCross();
  ui->graph_area->replot();
}

QVector<double> MainWindow::fromStdVector(std::vector<double> src) {
  QVector<double> res;
  for (auto i : src) {
    res.push_back(i);
  }
  return res;
}

void MainWindow::DrawCross() {
  if (control_.GetParam() > x_min_ && control_.GetParam() < x_max_ &&
      answer_ > y_min_ && answer_ < y_max_) {
    double y_size_ = (y_max_ - y_min_) / 20;
    double x_size_ = (x_max_ - x_min_) / 20;
    ui->graph_area->addGraph();
    QVector<double> cross_x_line_x{control_.GetParam(), control_.GetParam()};
    QVector<double> cross_x_line_y{answer_ - y_size_, answer_ + y_size_};
    ui->graph_area->graph(1)->addData(cross_x_line_x, cross_x_line_y);
    ui->graph_area->graph(1)->setPen(QPen(Qt::red));
    ui->graph_area->addGraph();
    QVector<double> cross_y_line_x{control_.GetParam() - x_size_,
                                   control_.GetParam() + x_size_};
    QVector<double> cross_y_line_y{answer_, answer_};
    ui->graph_area->graph(2)->addData(cross_y_line_x, cross_y_line_y);
    ui->graph_area->graph(2)->setPen(QPen(Qt::red));
  }
}

void MainWindow::on_x_edit_editingFinished() {
  if (ui->x_edit->text().isEmpty()) {
    ui->answer_label->clear();
  } else {
    control_.SetParam(ui->x_edit->text().toDouble());
  }
}

void MainWindow::on_x_min_edit_editingFinished() {
  if (!ui->x_min_edit->text().isEmpty()) {
    x_min_ = ui->x_min_edit->text().toDouble();
    control_.SetLeft(x_min_);
    if (x_min_ >= x_max_) {
      x_max_ = x_min_ + 1;
      control_.SetRight(x_max_);
      ui->x_max_edit->setText(QString::number(x_max_));
    }
    ui->graph_area->xAxis->setRange(x_min_, x_max_);
  }
}

void MainWindow::on_x_max_edit_editingFinished() {
  if (!ui->x_max_edit->text().isEmpty()) {
    x_max_ = ui->x_max_edit->text().toDouble();
    control_.SetRight(x_max_);
    if (x_min_ >= x_max_) {
      x_min_ = x_max_ - 1;
      control_.SetLeft(x_min_);
      ui->x_min_edit->setText(QString::number(x_min_));
    }
    ui->graph_area->xAxis->setRange(x_min_, x_max_);
  }
}

void MainWindow::on_y_min_edit_editingFinished() {
  if (!ui->y_min_edit->text().isEmpty()) {
    y_min_ = ui->y_min_edit->text().toDouble();
    if (y_min_ >= y_max_) {
      y_max_ = y_min_ + 1;
      ui->y_max_edit->setText(QString::number(y_max_));
    }
    ui->graph_area->yAxis->setRange(y_min_, y_max_);
  }
}

void MainWindow::on_y_max_edit_editingFinished() {
  if (!ui->y_max_edit->text().isEmpty()) {
    y_max_ = ui->y_max_edit->text().toDouble();
    if (y_min_ >= y_max_) {
      y_min_ = y_max_ - 1;
      ui->y_min_edit->setText(QString::number(y_min_));
    }
    ui->graph_area->yAxis->setRange(y_min_, y_max_);
  }
}
