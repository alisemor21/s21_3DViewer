#include "mainwindow.h"

#include <QColorDialog>

#include "ui_mainwindow.h"
#include "widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(geometry().width(), geometry().height());
  ui->myWidget->setFocus();

  fileSettings = QApplication::applicationDirPath() + "/settings_demo.conf";
  loadSettings();
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

void MainWindow::on_addfile_clicked() {
  ui->myWidget->addFile();
  ui->label_info->setText(QString::fromStdString(ui->myWidget->file_info));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  ui->myWidget->keyPress(event);
}

void MainWindow::on_lineEdit_moveY_returnPressed() {
  QString m = ui->lineEdit_moveY->text();
  ui->myWidget->moveY += m.toInt();
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_moveX_returnPressed() {
  QString m = ui->lineEdit_moveX->text();
  ui->myWidget->moveX += m.toInt();
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_rotationY_returnPressed() {
  QString angle = ui->lineEdit_rotationY->text();
  ui->myWidget->rotationY(angle.toInt());
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_rotationX_returnPressed() {
  QString angle = ui->lineEdit_rotationX->text();
  ui->myWidget->rotationX(angle.toInt());
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_rotationZ_returnPressed() {
  QString angle = ui->lineEdit_rotationZ->text();
  ui->myWidget->rotationZ(angle.toInt());
  ui->myWidget->update();
}

void MainWindow::on_submitMoveX_clicked() {
  on_lineEdit_moveX_returnPressed();
  ui->lineEdit_moveX->clear();
}

void MainWindow::on_submitMoveY_clicked() {
  on_lineEdit_moveY_returnPressed();
  ui->lineEdit_moveY->clear();
}

void MainWindow::on_submitRotateY_clicked() {
  on_lineEdit_rotationY_returnPressed();
  ui->lineEdit_rotationY->clear();
}

void MainWindow::on_submitRotateX_clicked() {
  on_lineEdit_rotationX_returnPressed();
  ui->lineEdit_rotationX->clear();
}

void MainWindow::on_submitRotateZ_clicked() {
  on_lineEdit_rotationZ_returnPressed();
  ui->lineEdit_rotationZ->clear();
}

void MainWindow::on_bigger_clicked() {
  ui->myWidget->setScale(ui->myWidget->getScale() + 0.2);
  ui->myWidget->update();
}

void MainWindow::on_smaller_clicked() {
  ui->myWidget->setScale(ui->myWidget->getScale() - 0.2);
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_lineWidth_returnPressed() {
  QString w = ui->lineEdit_lineWidth->text();
  ui->lineEdit_pointSize->clear();
  ui->myWidget->setLineWidth(w.toInt());
  ui->myWidget->update();
}

void MainWindow::on_lineEdit_pointSize_returnPressed() {
  QString s = ui->lineEdit_pointSize->text();
  ui->lineEdit_pointSize->clear();
  ui->myWidget->setPointSize(s.toInt());
  ui->myWidget->update();
}

void MainWindow::on_radioButton_disablePoint_clicked(bool checked) {
  if (checked) {
    ui->myWidget->setPointSize(1);
    ui->myWidget->update();
  }
}

void MainWindow::on_radioButton_circlePoint_clicked(bool checked) {
  if (checked) {
    if (ui->myWidget->getPointSize() == 1) {
      ui->myWidget->setPointSize(15);
    }
    ui->myWidget->setPointStyle(GL_POINT_SMOOTH);
    ui->myWidget->update();
  }
}

void MainWindow::on_radioButton_squarePoint_clicked(bool checked) {
  if (checked) {
    if (ui->myWidget->getPointSize() == 1) {
      ui->myWidget->setPointSize(15);
    }
    ui->myWidget->setPointStyle(GL_POINTS);
    ui->myWidget->update();
  }
}

void MainWindow::on_radioButton_solidLine_clicked(bool checked) {
  if (checked) {
    ui->myWidget->setLineStyle(GL_LINES);
    ui->myWidget->update();
  }
}

void MainWindow::on_radioButton_dottedLine_clicked(bool checked)  //
{
  if (checked) {
    ui->myWidget->setLineStyle(GL_LINE_STIPPLE);
    ui->myWidget->update();
  }
}

void MainWindow::on_selectLineColor_clicked() {
  ui->myWidget->changeLineColor();
}

void MainWindow::on_selectFillColor_clicked() {
  ui->myWidget->changeFillColor();
}

void MainWindow::on_selectPointColor_clicked() {
  ui->myWidget->changePointColor();
}

void MainWindow::loadSettings() {
  QFile file(fileSettings);
  if (file.exists()) {
    QSettings settings(fileSettings, QSettings::IniFormat);
    ui->myWidget->pointStyle = settings.value("pointStyle").toInt();
    ui->myWidget->lineStyle = settings.value("lineStyle").toInt();
    ui->myWidget->lineWidth = settings.value("lineWidth").toInt();
    ui->myWidget->pointSize = settings.value("pointSize").toInt();
    QVariant colorLineSetting = settings.value("colorLine");
    ui->myWidget->colorLine = colorLineSetting.value<QColor>();
    QVariant colorPointSetting = settings.value("colorPoint");
    ui->myWidget->colorPoint = colorPointSetting.value<QColor>();
    QVariant colorFillSetting = settings.value("colorFill");
    ui->myWidget->colorFill = colorFillSetting.value<QColor>();
    QVariant colorBackgroundSetting = settings.value("colorBackground");
    ui->myWidget->colorBackground = colorBackgroundSetting.value<QColor>();
  }
}

void MainWindow::saveSettings() {
  QSettings settings(fileSettings, QSettings::IniFormat);
  settings.setValue("pointStyle", ui->myWidget->pointStyle);
  settings.setValue("lineStyle", ui->myWidget->lineStyle);
  settings.setValue("lineWidth", ui->myWidget->lineWidth);
  settings.setValue("pointSize", ui->myWidget->pointSize);
  settings.setValue("colorLine", ui->myWidget->colorLine);
  settings.setValue("colorPoint", ui->myWidget->colorPoint);
  settings.setValue("colorFill", ui->myWidget->colorFill);
  settings.setValue("colorBackground", ui->myWidget->colorBackground);
}

void MainWindow::on_takeScreenShot_clicked() {
  QString str = QFileDialog::getSaveFileName(
      nullptr, "", "", "", nullptr, QFileDialog::Option::DontUseNativeDialog);

  if (!str.isEmpty()) {
    ui->myWidget->grab().save(str);
  }
}

void MainWindow::on_makeGif_clicked() {
  tim = new QTimer();
  imgIter = 0;
  connect(tim, SIGNAL(timeout()), this, SLOT(addImg()));
  tim->start(100);
}

void MainWindow::addImg() {
  if (imgIter > 99) {
    tim->stop();
    disconnect(tim);
    imgIter = 0;
    for (int i = 0; i < 100; gif.addFrame(mp[i++], 200)) {
    }
    QString str = QFileDialog::getSaveFileName(
        0, "Сохранить файл как", "", "*.gif", nullptr,
        QFileDialog::Option::DontUseNativeDialog);
    gif.save(str);
    ui->makeGif->setText("GIF");
    ui->makeGif->setEnabled(1);
  } else {
    mp[imgIter] = ui->myWidget->grab().toImage();
    imgIter += 1;
    ui->makeGif->setText(QString::number(imgIter / 10));
  }
}

void MainWindow::on_selectBackgroundColor_clicked() {
  ui->myWidget->changeBackgroundColor();
}

void MainWindow::on_switchProjection_clicked() {
  ui->myWidget->Mode = ui->myWidget->Mode * (-1);
  ui->myWidget->update();
}
