#include "s21_3DViewer.h"

void print_V(const obj_data *Data);
void print_F(const obj_data *Data);

int main() {
    obj_data Data;
    int error;
    // error = fileReading(&Data, "obj_files/12140_Skull_v3_L2.obj");
    error = fileReading(&Data, "obj_files/cube_test.obj");
    // error = fileReading(&Data, "obj_files/memory.obj");
    // error = fileReading(&Data, "obj_files/Glass.obj");
    // error = fileReading(&Data, "obj_files/cow.obj");
    // error = fileReading(&Data, "obj_files/alduin.obj");
    // error = fileReading(&Data, "obj_files/sandal.obj");
    // error = fileReading(&Data, "obj_files/gnom.obj");
    // error = fileReading(&Data, "obj_files/pumpkin.obj");
    // error = fileReading(&Data, "obj_files/teddy.obj");
    // error = fileReading(&Data, "obj_files/star.obj");
    printf("\nError = %d\n", error);
    if (error == OK) {
        rotationX(&Data, 90);
        rotationX(&Data, -90);
        printf("num_V = %d\n", Data.num_V);
        printf("num_F = %d\n", Data.num_F);

        printf("V_min: ");
        for (int i = 0; i < 3; i++) {
            printf("%lf ", Data.V_min[i]);
        }
        printf("\n");

        printf("V_max: ");
        for (int i = 0; i < 3; i++) {
            printf("%lf ", Data.V_max[i]);
        }
        printf("\n");
        print_V(&Data);
        print_F(&Data);
        delete_data(&Data);
    }

    return 0;
}


void print_V(const obj_data *Data) {
    for (int i = 0; i < Data->num_V && i < 10; i++) {
        printf("v%d:    %lf %lf %lf\n", i+1, Data->V[i * 3 + 0], Data->V[i * 3 + 1], Data->V[i * 3 + 2]);
    }
}

void print_F(const obj_data *Data) {
    for (int i = 0; i < Data->num_F; i++) {
        printf("f%d = %d: ", i+1, Data->F[i].num_vertices);
        for (int j = 0; j < Data->F[i].num_vertices; j++) {
            printf("%6d ", Data->F[i].vertices_on_polygon[j]);
        }
        printf("\n");
    }
}
