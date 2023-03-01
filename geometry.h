#include <vector>
#include <math.h>
#include "vec3.h"

vec3 baricentric(vec3 A, vec3 B,vec3 C, int x, int y);

vec3 normal_vect(vec3 A, vec3 B, vec3 C);

std::vector<vec3> matices_multiplication (std::vector<vec3> mult, std::vector<vec3> list_of_coords);

std::vector<vec3> transpose(std::vector<vec3> mat);

void dist(std::vector<vec3> mat, std::vector<vec3> mat2);

mat<4,4> viewport(int x, int y, int w, int h);

std::vector<vec3> compute_perspective( std::vector<vec3>vertices, vec3 eye, vec3 center, vec3 up, int width, int height, mat<4,4> *M);

vec3 ModelView (vec3 point,vec3 eye, vec3 center, vec3 up);