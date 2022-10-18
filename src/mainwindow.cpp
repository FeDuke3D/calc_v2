#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      control_(new s21::Controller) {
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
  ConnectingSlots();
}

MainWindow::~MainWindow() {
  delete ui;
  if (control_ != nullptr) delete control_;
}

void MainWindow::text_button_clicked() {
  auto button = qobject_cast<QPushButton *>(sender());
  ui->expression_edit->setText(ui->expression_edit->text() + button->text());
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
      control_->InputString(ui->expression_edit->text().toStdString());
      if (control_->NeedsGraph()) {
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
  answer_ = control_->CalculateSingle();
  QString tmp =
      QString::number(answer_, 'f', 8).remove(QRegularExpression("0+$"));
  ui->answer_label->setText(tmp.remove(QRegularExpression("\\.$")));
}

void MainWindow::DrawGraph() {
  control_->SetCount(ui->graph_area->width());
  ui->graph_area->clearGraphs();
  ui->graph_area->addGraph();
  control_->CalculateGraph();
  QVector<double> x_data = fromStdVector(control_->GetGraphX());
  QVector<double> y_data = fromStdVector(control_->GetGraphY());
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
  if (control_->GetParam() > x_min_ && control_->GetParam() < x_max_ &&
      answer_ > y_min_ && answer_ < y_max_) {
    double y_size_ = (y_max_ - y_min_) / 20;
    double x_size_ = (x_max_ - x_min_) / 20;
    ui->graph_area->addGraph();
    QVector<double> cross_x_line_x{control_->GetParam(), control_->GetParam()};
    QVector<double> cross_x_line_y{answer_ - y_size_, answer_ + y_size_};
    ui->graph_area->graph(1)->addData(cross_x_line_x, cross_x_line_y);
    ui->graph_area->graph(1)->setPen(QPen(Qt::red));
    ui->graph_area->addGraph();
    QVector<double> cross_y_line_x{control_->GetParam() - x_size_,
                                   control_->GetParam() + x_size_};
    QVector<double> cross_y_line_y{answer_, answer_};
    ui->graph_area->graph(2)->addData(cross_y_line_x, cross_y_line_y);
    ui->graph_area->graph(2)->setPen(QPen(Qt::red));
  }
}

void MainWindow::on_x_edit_editingFinished() {
  if (ui->x_edit->text().isEmpty()) {
    ui->answer_label->clear();
  } else {
    control_->SetParam(ui->x_edit->text().toDouble());
  }
}

void MainWindow::on_x_min_edit_editingFinished() {
  if (!ui->x_min_edit->text().isEmpty()) {
    x_min_ = ui->x_min_edit->text().toDouble();
    control_->SetLeft(x_min_);
    if (x_min_ >= x_max_) {
      x_max_ = x_min_ + 1;
      control_->SetRight(x_max_);
      ui->x_max_edit->setText(QString::number(x_max_));
    }
    ui->graph_area->xAxis->setRange(x_min_, x_max_);
  }
}

void MainWindow::on_x_max_edit_editingFinished() {
  if (!ui->x_max_edit->text().isEmpty()) {
    x_max_ = ui->x_max_edit->text().toDouble();
    control_->SetRight(x_max_);
    if (x_min_ >= x_max_) {
      x_min_ = x_max_ - 1;
      control_->SetLeft(x_min_);
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

void MainWindow::ConnectingSlots() {
  connect(ui->button_right_par, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_left_par, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_minus, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_asin, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_acos, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_atan, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_sqrt, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_plus, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_mult, SIGNAL(clicked()), this,
          SLOT(text_button_clicked()));
  connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_log, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_div, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_pow, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_dot, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_0, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_1, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_2, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_3, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_4, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_5, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_6, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_7, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_8, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_9, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
  connect(ui->button_x, SIGNAL(clicked()), this, SLOT(text_button_clicked()));
}
