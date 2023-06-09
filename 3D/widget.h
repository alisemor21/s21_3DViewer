#ifndef WIDGET_H
#define WIDGET_H

#include <QBasicTimer>
#include <QColorDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMessageBox>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QSettings>
#include <QTimer>
#include <QVector3D>
#include <QVector4D>
#include <iostream>

extern "C" {
#include "../Backend/s21_3DViewer.h"
}

struct VData {
  QVector3D position;
  QVector4D colorFill;
  QVector4D colorLine;
  QVector4D colorPoint;
  QVector4D colorBackground;
};

class Widget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  std::string file_info;
  qreal aspect;
  void addFile();
  void keyPress(QKeyEvent *event);
  double moveX;
  double moveY;
  int setScale(double newScale);
  double getScale() const;
  int setLineWidth(int newWidth);
  int getLineWidth() const;
  int setPointSize(int newSize);
  int getPointSize() const;
  int setPointStyle(int newPointStyle);
  int getPointStyle() const;

  int setLineStyle(int newPointStyle);
  int getLineStyle() const;
  void rotationX(int angle);
  void rotationY(int angle);
  void rotationZ(int angle);
  void changeLineColor();
  void changePointColor();
  void changeFillColor();
  void changeBackgroundColor();
  QColor colorLine;
  QColor colorPoint;
  QColor colorFill;
  QColor colorBackground;
  QColor getColor();

  int pointStyle;
  int lineStyle;
  int lineWidth;
  int pointSize;
  double *v;
  GLushort *p;
  int len_v;
  int len_p;
  int realLenP;
  int Mode;

 protected:
  void initializeGL();
  void parse(char *fileName);
  void resizeGL(int w, int h);
  void paintGL();
  void initTextures();
  void initShaders();

 private:
  int isLoading;
  void fuckingGraz();
  void initArrays();
  void Draw(quintptr offset, GLenum mode, int len);
  double scale;
  QOpenGLBuffer vertex;
  QOpenGLBuffer lines;
  QOpenGLBuffer polygon;
  QOpenGLBuffer pointIndexes;
  int polsCount;

  QOpenGLShaderProgram shaderProgram;
  QMatrix4x4 matrix;
};

#endif  // WIDGET_H
