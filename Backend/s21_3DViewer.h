#ifndef SRC_BACKEND_S21_3DVIEWER_H_
#define SRC_BACKEND_S21_3DVIEWER_H_

/*! \mainpage Drawing Shapes
 *
 *
 * 3DViewer v1.0
 */

/*!
\file
*Header for 3DViewer
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Max size of string in file
#define buff_size 256  // 1024

/// Code of error for fileReading(). Data was read
#define OK 0
/// Code of error for fileReading(). File not open
#define ERROR_FILE_OPEN 1
/// Code of error for fileReading(). Error in data of file
#define ERROR_FILE_STRUCT 2
/// Code of error for fileReading(). Error of memory
#define ERROR_MEMORY 3

/// Minimum number of vertices in model
#define MIN_VERTICES 3

/// Precision for tests
#define eps 1e-6

/// Struct for vertices on polygon
typedef struct polygon {
  int num_vertices;
  int *vertices_on_polygon;
} polygon;

/// Struct for data of vertices and polygon
typedef struct obj_data {
  int num_V;        ///< Number of vertices
  int num_F;        ///< Number of surfaces
  double *V;        ///< Array for coordinates of vertices
  polygon *F;       ///< Array of struct for vertices on polygon
  double V_min[3];  ///< Minimum coordinate values of vertices
  double V_max[3];  ///< Maximum coordinate values of vertices
} obj_data;

/*!
Init struct of data by zero
*/
void init_data(obj_data *Data);
/*!
Delete struct of data
*/
void delete_data(obj_data *Data);

/*!
    Parser of file
    \param Data Output struct of data
    \param file_name Name of file
    \result Code of error
*/
int fileReading(obj_data *Data, char *file_name);

/*!
    Counts the number of vertices and polygons
    \result Code of error
*/
int count_VF(obj_data *Data, FILE *file);

/*!
    Adds data in struct
    \result Code of error
*/
int add_points(obj_data *Data, FILE *file);

/*!
    Adds coordinates of vertices
    \result Code of error
*/
int add_vertice(const char *str, obj_data *Data, int count_V);

/*!
    Adds vertices on polygon
    \result Code of error
*/
int add_polygon(const char *str, obj_data *Data, int count_F);

/*!
    Counts number of vertices on current polygon
    \result Code of error
*/
int count_vertices_on_polygon(const char *str, obj_data *Data, int count_F);

/*!
    Checks coordinates of vertices
    \result Code of error
*/
int check_digit(const char *str);

/*!
    Checks 'v' and 'f'
    \result Code of error
*/
int check_symb(char *str, char c);

/*!
    Shifts all points to the new center of the coordinate axis
    \result Code of error
*/
void centering(obj_data *Data);

/*!
    Rotation OX
*/
void rotX(obj_data *Data, int angle);

/*!
    Rotation OY
*/
void rotY(obj_data *Data, int angle);

/*!
    Rotation OZ
*/
void rotZ(obj_data *Data, int angle);

#endif  // SRC_BACKEND_S21_3DVIEWER_H_
