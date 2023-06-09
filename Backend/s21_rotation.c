#include "s21_3DViewer.h"

void rotX(obj_data *Data, int angle) {
  for (int i = 0; i < Data->num_V * 3; i += 3) {
    double newY = cos(angle * 0.01745) * Data->V[i + 1] +
                  sin(angle * 0.01745) * Data->V[i + 2];
    double newZ = -sin(angle * 0.01745) * Data->V[i + 1] +
                  cos(angle * 0.01745) * Data->V[i + 2];
    Data->V[i + 1] = newY;
    Data->V[i + 2] = newZ;
  }
}

void rotY(obj_data *Data, int angle) {
  for (int i = 0; i < Data->num_V * 3; i += 3) {
    double newX = cos(angle * 0.01745) * Data->V[i] -
                  sin(angle * 0.01745) * Data->V[i + 2];
    double newZ = sin(angle * 0.01745) * Data->V[i] +
                  cos(angle * 0.01745) * Data->V[i + 2];
    Data->V[i] = newX;
    Data->V[i + 2] = newZ;
  }
}

void rotZ(obj_data *Data, int angle) {
  for (int i = 0; i < Data->num_V * 3; i += 3) {
    double newX = cos(angle * 0.01745) * Data->V[i] +
                  sin(angle * 0.01745) * Data->V[i + 1];
    double newY = -sin(angle * 0.01745) * Data->V[i] +
                  cos(angle * 0.01745) * Data->V[i + 1];
    Data->V[i] = newX;
    Data->V[i + 1] = newY;
  }
}
