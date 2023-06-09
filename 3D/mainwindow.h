#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "ima/qgifimage_p.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void loadSettings();
  void saveSettings();

 private slots:
  void on_addfile_clicked();

  void on_lineEdit_moveY_returnPressed();

  void on_lineEdit_moveX_returnPressed();

  void on_lineEdit_rotationY_returnPressed();

  void on_lineEdit_rotationX_returnPressed();

  void on_lineEdit_rotationZ_returnPressed();

  void on_submitMoveX_clicked();

  void on_submitMoveY_clicked();

  void on_submitRotateY_clicked();

  void on_submitRotateX_clicked();

  void on_submitRotateZ_clicked();

  void on_bigger_clicked();

  void on_smaller_clicked();

  void on_lineEdit_lineWidth_returnPressed();

  void on_lineEdit_pointSize_returnPressed();

  void on_radioButton_disablePoint_clicked(bool checked);

  void on_radioButton_circlePoint_clicked(bool checked);

  void on_radioButton_squarePoint_clicked(bool checked);

  void on_radioButton_solidLine_clicked(bool checked);

  void on_radioButton_dottedLine_clicked(bool checked);

  // void on_kill_clicked();

  void on_selectLineColor_clicked();

  void on_selectFillColor_clicked();

  void on_selectPointColor_clicked();

  void on_takeScreenShot_clicked();

  void on_makeGif_clicked();

  void addImg();

  void on_selectBackgroundColor_clicked();

  void on_switchProjection_clicked();

 private:
  Ui::MainWindow *ui;
  void keyPressEvent(QKeyEvent *event);
  QString fileSettings;

  QTimer *tim;
  int imgIter;
  QGifImage gif;

  QImage mp[100];
};
#endif  // MAINWINDOW_H
