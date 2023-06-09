#include "widget.h"

#include <math.h>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      moveX(0),
      moveY(0),
      colorLine(0, 255, 0),
      colorPoint(255, 255, 255),
      colorFill(255, 0, 255),
      colorBackground(0, 0, 0),
      lineWidth(2),
      pointSize(1),
      len_v(0),
      len_p(0),
      realLenP(0),
      Mode(1),
      isLoading(1),
      scale(1),
      lines(QOpenGLBuffer::IndexBuffer),
      polygon(QOpenGLBuffer::IndexBuffer),
      pointIndexes(QOpenGLBuffer::IndexBuffer),
      polsCount(0) {}

Widget::~Widget() {}

int Widget::setScale(double newScale) {
  if (newScale != 0) {
    scale = newScale;
    return 1;
  }
  return 0;
}
double Widget::getScale() const { return scale; }

int Widget::setLineWidth(int newWidth) {
  if (newWidth > 0) {
    lineWidth = newWidth;
    return 1;
  }
  return 0;
}
int Widget::getLineWidth() const { return lineWidth; }

int Widget::setPointSize(int newSize) {
  if (newSize > 0) {
    pointSize = newSize;
    return 1;
  }
  return 0;
}
int Widget::getPointSize() const { return pointSize; }

int Widget::setPointStyle(int newPointStyle) {
  if (newPointStyle == GL_POINTS || newPointStyle == GL_POINT_SMOOTH) {
    pointStyle = newPointStyle;
    return 1;
  }
  return 0;
}
int Widget::getPointStyle() const { return pointStyle; }

int Widget::setLineStyle(int newLineStyle) {
  if (newLineStyle == GL_LINES || newLineStyle == GL_LINE_STIPPLE) {
    lineStyle = newLineStyle;
    return 1;
  }
  return 0;
}
int Widget::getLineStyle() const { return lineStyle; }

QColor Widget::getColor() {
  return QColorDialog::getColor(Qt::white, this, tr("Select color")).toRgb();
}

void Widget::changeLineColor() {
  colorLine = getColor();
  update();
}

void Widget::changePointColor() {
  colorPoint = getColor();
  update();
}

void Widget::changeFillColor() {
  colorFill = getColor();
  update();
}

void Widget::changeBackgroundColor() {
  colorBackground = getColor();
  update();
}

void Widget::addFile() {
  QString fileName =
      QFileDialog::getOpenFileName(this, "  ", "/Users", "*.obj", nullptr,
                                   QFileDialog::Option::DontUseNativeDialog);
  QByteArray ba = fileName.toLocal8Bit();
  char *str = ba.data();
  parse(str);
  update();
}

void Widget::keyPress(QKeyEvent *event) {
  switch (event->key()) {
    case (Qt::Key_X):
      this->rotationX(5);
      update();
      break;
    case (Qt::Key_Y):
      this->rotationY(5);
      update();
      break;
    case (Qt::Key_Z):
      this->rotationZ(5);
      update();
      break;
    case (Qt::Key_Minus):
      this->scale -= 0.1;
      update();
      break;
    case (Qt::Key_Equal):
      this->scale += 0.1;
      update();
      break;
    case (Qt::Key_Left):
      this->moveX += 1;
      update();
      break;
    case (Qt::Key_Right):
      this->moveX -= 1;
      update();
      break;
    case (Qt::Key_Up):
      this->moveY -= 1;
      update();
      break;
    case (Qt::Key_Down):
      this->moveY += 1;
      update();
      break;
    case (Qt::Key_F):
      addFile();
      break;
  }
}

void Widget::rotationX(int angle) {
  for (int i = 0; i < len_v * 3; i += 3) {
    double newY =
        cos(angle * 0.01745) * v[i + 1] + sin(angle * 0.01745) * v[i + 2];
    double newZ =
        -sin(angle * 0.01745) * v[i + 1] + cos(angle * 0.01745) * v[i + 2];
    v[i + 1] = newY;
    v[i + 2] = newZ;
  }
}

void Widget::rotationY(int angle) {
  for (int i = 0; i < len_v * 3; i += 3) {
    double newX = cos(angle * 0.01745) * v[i] - sin(angle * 0.01745) * v[i + 2];
    double newZ = sin(angle * 0.01745) * v[i] + cos(angle * 0.01745) * v[i + 2];
    v[i] = newX;
    v[i + 2] = newZ;
  }
}

void Widget::rotationZ(int angle) {
  for (int i = 0; i < len_v * 3; i += 3) {
    double newX = cos(angle * 0.01745) * v[i] + sin(angle * 0.01745) * v[i + 1];
    double newY =
        -sin(angle * 0.01745) * v[i] + cos(angle * 0.01745) * v[i + 1];
    v[i] = newX;
    v[i + 1] = newY;
  }
}

void Widget::initializeGL() {
  vertex.create();
  polygon.create();
  pointIndexes.create();
  lines.create();
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  initShaders();
}

void Widget::resizeGL(int w, int h) {
  aspect = qreal(w) / qreal(h ? h : 1);
  matrix.setToIdentity();
  matrix.perspective(45.0f, aspect, 0.01f, 1000.0f);
}

void Widget::initShaders() {
  if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                             ":/vert.vsh"))
    close();

  if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                             ":/frag.fsh"))
    close();

  if (!shaderProgram.link()) close();

  if (!shaderProgram.bind()) close();
}

void Widget::initArrays() {
  VData *vbuf = new VData[len_v];
  int k = 0;

  for (int i = 0; i < len_v * 3; i += 3) {
    vbuf[k].position = QVector3D((v[i] + moveX) * scale,
                                 (v[i + 1] + moveY) * scale, v[i + 2] * scale);
    vbuf[k].colorFill =
        QVector4D(colorFill.redF(), colorFill.greenF(), colorFill.blueF(), 1);
    vbuf[k].colorLine =
        QVector4D(colorLine.redF(), colorLine.greenF(), colorLine.blueF(), 1);
    vbuf[k].colorPoint = QVector4D(colorPoint.redF(), colorPoint.greenF(),
                                   colorPoint.blueF(), 1);
    k++;
  }
  vertex.bind();
  vertex.allocate(vbuf, sizeof(VData) * len_v);
  vertex.release();
  delete[] vbuf;

  GLushort *linesBuf = new GLushort[polsCount * 4];
  int iter = 0;
  for (int i = 0; i < polsCount * 3; i += 3) {
    linesBuf[iter++] = p[i];
    linesBuf[iter++] = p[i + 1];
    linesBuf[iter++] = p[i + 1];
    linesBuf[iter++] = p[i + 2];
  }

  lines.bind();
  lines.allocate(linesBuf, sizeof(GLushort) * polsCount * 4);
  lines.release();
  delete[] linesBuf;

  GLushort *polyBuf = new GLushort[polsCount * 6];
  iter = 0;
  for (int i = 0; i < polsCount * 3; i += 3) {
    polyBuf[iter++] = p[i];
    polyBuf[iter++] = p[i + 1];
    polyBuf[iter++] = p[i + 2];
    polyBuf[iter++] = p[i + 2];
    polyBuf[iter++] = p[i + 1];
    polyBuf[iter++] = p[i];
  }

  polygon.bind();
  polygon.allocate(polyBuf, sizeof(GLushort) * polsCount * 6);
  polygon.release();
  delete[] polyBuf;
}

void Widget::Draw(quintptr offset, GLenum mode, int len) {
  int vertexLoc = shaderProgram.attributeLocation("position");
  shaderProgram.enableAttributeArray(vertexLoc);
  shaderProgram.setAttributeBuffer(vertexLoc, GL_FLOAT, 0, 3, sizeof(VData));

  int tLoc = shaderProgram.attributeLocation("tex");
  shaderProgram.enableAttributeArray(tLoc);
  shaderProgram.setAttributeBuffer(tLoc, GL_FLOAT, offset, 4, sizeof(VData));

  glDrawElements(mode, len, GL_UNSIGNED_SHORT, nullptr);
}

void Widget::parse(char *fileName) {
  obj_data Data;
  int error = fileReading(&Data, fileName);
  if (error != OK) {
    QMessageBox msgBox;
    msgBox.setFixedSize(QSize(600, 120));
    msgBox.setFont(QFont("ITF Devanagari", 18));
    msgBox.addButton("Ok", QMessageBox::YesRole);
    msgBox.setText("Oops! Error of file reading.");
    msgBox.setStyleSheet(
        "QWidget{background-color: rgb(221, 210, 228);}"
        "QPushButton{width:60px; height: 30; font-size: 16px; border-radius: "
        "15px; border:  3px solid rgb(181, 154, 188); background-color: "
        "rgb(208, 181, 206);}"
        "QDialogButtonBox { qproperty-centerButtons: true; }"
        "QPushButton:pressed {width:60px; height: 30; font-size: 16px; "
        "border-radius: 15px; border:  4px solid rgb(157, 134, 164); "
        "background-color:  rgb(189, 163, 188);}");
    msgBox.exec();
  } else {
    file_info = "File name: " + std::string(fileName) +
                "\nNumber of vertices: " + std::to_string(Data.num_V) +
                "\nNumber of edges: " + std::to_string(Data.num_F);
    scale = 1;
    len_v = Data.num_V;
    len_p = Data.num_F;
    v = (double *)malloc(len_v * sizeof(double) * 3);
    double max = 0;
    for (int i = 0; i < len_v * 3; i++) {
      v[i] = Data.V[i];
      max = v[i] > max ? v[i] : max;
    }
    int coef = 1;
    while (max > 11) {
      coef *= 10;
      max /= 10;
    }
    for (int i = 0; i < len_v * 3; i++) {
      v[i] /= coef;
    }
    int ii = 0;
    int fourCount = 0;
    for (int i = 0; i < len_p; i++) {
      fourCount += Data.F[i].num_vertices == 4 ? 1 : 0;
    }
    realLenP = ((fourCount * 12) + ((len_p - fourCount)) * 6);
    p = (GLushort *)malloc(sizeof(GLushort) * realLenP);
    for (int i = 0; i < len_p; i++) {
      p[ii++] = Data.F[i].vertices_on_polygon[0] - 1;
      p[ii++] = Data.F[i].vertices_on_polygon[1] - 1;
      p[ii++] = Data.F[i].vertices_on_polygon[2] - 1;
      if (Data.F[i].num_vertices == 4) {
        p[ii++] = Data.F[i].vertices_on_polygon[0] - 1;
        p[ii++] = Data.F[i].vertices_on_polygon[2] - 1;
        p[ii++] = Data.F[i].vertices_on_polygon[3] - 1;
      }
      polsCount = ii / 3;
    }

    GLushort *pointsBuf = new GLushort[len_v];
    for (int i = 0; i < len_v; i++) pointsBuf[i] = i;

    pointIndexes.bind();
    pointIndexes.allocate(pointsBuf, sizeof(GLushort) * len_v);
    pointIndexes.release();

    delete[] pointsBuf;
    delete_data(&Data);
  }
}

void Widget::paintGL() {
  glClearColor(colorBackground.redF(), colorBackground.greenF(),
               colorBackground.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shaderProgram.bind();

  matrix.setToIdentity();
  if (Mode == 1) {
    matrix.perspective(45.0f, aspect, 0.01f, 1000.0f);
  } else if (Mode == -1) {
    matrix.ortho(-10.0f, 1.0f, -1.0f, 10.0f, 0.1f, 1000.0f);
  }

  QMatrix4x4 projection;
  projection.setToIdentity();
  projection.translate(0.0, 0.0, -30.0);
  shaderProgram.setUniformValue("matrix", matrix * projection);

  initArrays();

  vertex.bind();
  polygon.bind();

  glLineWidth(lineWidth);
  glPointSize(pointSize);

  quintptr offset = sizeof(QVector3D);

  Draw(offset, GL_TRIANGLES, polsCount * 6);
  offset += sizeof(QVector4D);
  polygon.release();

  lines.bind();
  glEnable(getLineStyle());
  glLineStipple(2, 0X00FF);
  Draw(offset, GL_LINES, polsCount * 4);
  glDisable(getLineStyle());
  offset += sizeof(QVector4D);
  lines.release();
  pointIndexes.bind();
  glEnable(getPointStyle());
  Draw(offset, GL_POINTS, len_v);
  glDisable(getPointStyle());
  pointIndexes.release();
  vertex.release();
}

void Widget::fuckingGraz() {
  double *newVertexes = (double *)malloc(
      sizeof(double) * (this->len_v + this->polsCount * 6) * 3);
  GLushort *newEges =
      (GLushort *)malloc(sizeof(GLushort) * this->polsCount * 6 * 3);
  size_t newEdgesIter = 0;
  int len = this->polsCount * 3;
  for (int i = 0; i < this->len_v * 3; i++) {
    newVertexes[i] = v[i];
  }

  int offset = this->len_v * 3;
  this->len_v = (this->len_v + this->polsCount * 4);

  polsCount = 0;
  for (int i = 0; i < len; i += 3) {
    double *buf = new double[7 * 3];
    int iter = 0;
    for (int subI = 0; subI < 9; subI += 3) {
      buf[subI] = v[this->p[i + iter] * 3];
      buf[subI + 1] = v[this->p[i + iter] * 3 + 1];
      buf[subI + 2] = v[this->p[i + iter] * 3 + 2];
      iter++;
    }
    // point 4
    buf[3 * 3] = (buf[0] + buf[3] + buf[6]) / 3;
    buf[3 * 3 + 1] = (buf[1] + buf[4] + buf[7]) / 3;
    buf[3 * 3 + 2] = (buf[2] + buf[5] + buf[8]) / 3;

    // point 5
    buf[3 * 4] = (buf[0] + buf[6]) / 2;
    buf[3 * 4 + 1] = (buf[1] + buf[7]) / 2;
    buf[3 * 4 + 2] = (buf[2] + buf[8]) / 2;
    // point 6
    buf[3 * 5] = (buf[3] + buf[6]) / 2;
    buf[3 * 5 + 1] = (buf[4] + buf[7]) / 2;
    buf[3 * 5 + 2] = (buf[5] + buf[8]) / 2;
    // point 7
    buf[3 * 6] = (buf[0] + buf[3]) / 2;
    buf[3 * 6 + 1] = (buf[1] + buf[4]) / 2;
    buf[3 * 6 + 2] = (buf[2] + buf[5]) / 2;

    for (size_t ii = 0; ii < 12; ii++) {
      newVertexes[offset + ii] = buf[9 + ii];
    }

    // new edge 1
    newEges[newEdgesIter++] = this->p[i];
    newEges[newEdgesIter++] = offset / 3;
    newEges[newEdgesIter++] = offset / 3 + 1;

    // new edge 2
    newEges[newEdgesIter++] = offset / 3 + 1;
    newEges[newEdgesIter++] = this->p[i + 2];
    newEges[newEdgesIter++] = offset / 3;

    // new edge 3
    newEges[newEdgesIter++] = offset / 3;
    newEges[newEdgesIter++] = this->p[i + 2];
    newEges[newEdgesIter++] = offset / 3 + 2;

    // new edge 4
    newEges[newEdgesIter++] = offset / 3 + 2;
    newEges[newEdgesIter++] = this->p[i + 1];
    newEges[newEdgesIter++] = offset / 3;

    // new edge 5
    newEges[newEdgesIter++] = offset / 3;
    newEges[newEdgesIter++] = this->p[i + 1];
    newEges[newEdgesIter++] = offset / 3 + 3;

    // new edge 6
    newEges[newEdgesIter++] = offset / 3 + 3;
    newEges[newEdgesIter++] = offset / 3;
    newEges[newEdgesIter++] = this->p[i];

    polsCount += 6;

    free(buf);
  }
  free(this->p);
  this->p = newEges;
  free(this->v);
  v = newVertexes;
}
