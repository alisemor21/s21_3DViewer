#include "s21_3DViewer.h"

int fileReading(obj_data *Data, char *file_name) {
  int error = ERROR_FILE_OPEN;

  init_data(Data);

  FILE *file;
  file = fopen(file_name, "r");
  if (file != NULL) {
    error = count_VF(Data, file);
    if (error == OK) {
      Data->V = (double *)calloc(Data->num_V * 3, sizeof(double));
      Data->F = (polygon *)calloc(Data->num_F, sizeof(polygon));
      if (Data->V && Data->F) {
        error = add_points(Data, file);
      } else {
        error = ERROR_MEMORY;
      }
    }
    if (error != OK) {
      delete_data(Data);
    } else {
      centering(Data);
    }
    fclose(file);
  }
  return error;
}

void init_data(obj_data *Data) {
  Data->num_V = 0;
  Data->num_F = 0;
  Data->V = NULL;
  Data->F = NULL;
  for (int i = 0; i < 3; i++) {
    Data->V_min[i] = 0;
    Data->V_max[i] = 0;
  }
}

void delete_data(obj_data *Data) {
  if (Data->V) free(Data->V);
  if (Data->F) {
    for (int i = 0; i < Data->num_F; i++) {
      if (Data->F[i].vertices_on_polygon) free(Data->F[i].vertices_on_polygon);
    }
    free(Data->F);
  }
}

// Считает количество вершин и поверхностей; Если v или f == 0, возвращает
// ошибку
int count_VF(obj_data *Data, FILE *file) {
  int error;
  char str_file[buff_size];

  while (fgets(str_file, sizeof(str_file), file)) {
    if (check_symb(str_file, 'v')) {
      Data->num_V++;
    } else if (check_symb(str_file, 'f')) {
      Data->num_F++;
    }
  }
  error = Data->num_V && Data->num_F ? OK : ERROR_FILE_STRUCT;
  rewind(file);

  return error;
}

int check_symb(char *str, char c) {
  int res = 0;
  if (str[0] == c && str[1] == ' ') res = 1;
  return res;
}

int check_digit(const char *str) {
  return ((str[0] <= '9' && str[0] >= '0') ||
          ((str[0] == '-' || str[0] == '+') && strlen(str) > 1 &&
           str[1] <= '9' && str[1] >= '0'))
             ? 1
             : 0;
}

// Добавляет координаты вершин и точки на поверхностях
int add_points(obj_data *Data, FILE *file) {
  int count_V = 0, count_F = 0;
  int error = OK;
  char str_file[buff_size];

  while (error == OK && (count_V < Data->num_V || count_F < Data->num_F) &&
         fgets(str_file, sizeof(str_file), file)) {
    if (check_symb(str_file, 'v')) {
      error = add_vertice(str_file, Data, count_V);
      count_V++;
    } else if (check_symb(str_file, 'f')) {
      error = count_vertices_on_polygon(str_file, Data, count_F);
      if (error == OK) {
        error = add_polygon(str_file, Data, count_F);
      }
      count_F++;
    }
  }

  return error;
}

// Добавляет координаты вершин в одной строке в Data
int add_vertice(const char *str, obj_data *Data, int count_V) {
  int error = OK;
  int i = 0;

  str = strchr(str, ' ');
  while (error == OK && str &&
         i < 3) {  // Игнорируем все, что лежит в конце строки
    str++;
    if (check_digit(str)) {  // *str &&
      Data->V[count_V * 3 + i] = atof(str);
      if (count_V == 0) {
        Data->V_min[i] = Data->V[count_V * 3 + i];
        Data->V_max[i] = Data->V[count_V * 3 + i];
      } else if (Data->V_min[i] > Data->V[count_V * 3 + i])
        Data->V_min[i] = Data->V[count_V * 3 + i];
      else if (Data->V_max[i] < Data->V[count_V * 3 + i])
        Data->V_max[i] = Data->V[count_V * 3 + i];

      i++;
      str++;
      while ((str[0] <= '9' && str[0] >= '0') || str[0] == '.')
        str++;  // *str &&
    } else if (str[0] != ' ')
      error = ERROR_FILE_STRUCT;

    if (i < 3 && str - strchr(str, ' ') == 0)
      str = strchr(str, ' ');
    else if (i < 3) {
      error = ERROR_FILE_STRUCT;
    }
  }
  return error;
}

// Считает кол-во точек в одной строке f и проверяет правильность первых N>=3
// точек, N <= num_V
int count_vertices_on_polygon(const char *str, obj_data *Data, int count_F) {
  int error = OK;
  int count_slash = 0;
  Data->F[count_F].num_vertices = 0;
  char *str_temp;
  int is_end = 0;

  str = strchr(str, ' ');
  while (error == OK && str && !is_end &&
         Data->F[count_F].num_vertices < Data->num_V) {
    str++;
    count_slash = 0;
    if (str[0] <= '9' && str[0] >= '0') {
      Data->F[count_F].num_vertices++;
      do {
        if (str[0] == '/' &&
            ((str[1] <= '9' && str[1] >= '0') || (str[1] == '/'))) {
          count_slash++;
        } else if (str[0] == '/') {
          error = ERROR_FILE_STRUCT;
        }
        str++;
      } while ((str[0] <= '9' && str[0] >= '0') ||
               (str[0] == '/' && count_slash < 2));

    } else if (str[0] != ' ' && Data->F[count_F].num_vertices < MIN_VERTICES) {
      error = ERROR_FILE_STRUCT;
    }

    str_temp = strchr(str, ' ');
    if (str_temp && str - str_temp == 0)
      str = str_temp;
    else if (Data->F[count_F].num_vertices < MIN_VERTICES)
      error = ERROR_FILE_STRUCT;
    else
      is_end = 1;
  }

  return error;
}

// Добавляет точки на поверхности в одной строке в Data; Ошибка, если номер
// точки > num_V
int add_polygon(const char *str, obj_data *Data, int count_F) {
  int error = OK;
  int i = 0;
  str++;
  Data->F[count_F].vertices_on_polygon =
      (int *)calloc(Data->F[count_F].num_vertices, sizeof(int));

  str = strchr(str, ' ');
  while (error == OK && str && i < Data->F[count_F].num_vertices) {
    str++;
    if (str[0] <= '9' && str[0] >= '0') {
      Data->F[count_F].vertices_on_polygon[i] = atoi(str);
      if (Data->F[count_F].vertices_on_polygon[i] > Data->num_V)
        error = ERROR_FILE_STRUCT;
      i++;
    }
    str = strchr(str, ' ');
  }

  return error;
}

// Переводит все точки в новую систему координат со свинутым центром
void centering(obj_data *Data) {
  double center[3];
  for (int j = 0; j < 3; j++) {
    center[j] = (Data->V_max[j] + Data->V_min[j]) / 2;
  }
  for (int i = 0; i < Data->num_V; i++)
    for (int j = 0; j < 3; j++) {
      Data->V[i * 3 + j] -= center[j];
    }
}
