#include "geometry.h"
#include <vector>
#include <math.h>
#include <iostream>

vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec<3>{v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

vec3 baricentric(vec3 A, vec3 B,vec3 C, int x, int y){
    float aire_ABC = (B[0] - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(B[1] - A[1]);
    float aire_APC = (x - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(y - A[1]);
    float aire_PBC = (B[0] - x)*(C[1] - y) -  (C[0] - x)*(B[1] - y);
    float aire_ABP = (B[0] - A[0])*(y - A[1]) -  (x - A[0])*(B[1] - A[1]);
    vec3 vect = {aire_PBC/aire_ABC, aire_APC/aire_ABC, aire_ABP/aire_ABC};
    return vect ;
}
vec3 normal_vect(vec3 A, vec3 B, vec3 C){
    float x = (B[2] - A[2])*(C[1] - A[1]) -  (C[2] - A[2])*(B[1] - A[1]);
    float y = (B[0] - A[0])*(C[2] - A[2]) -  (C[0] - A[0])*(B[2] - A[2]);
    float z = (B[0] - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(B[1] - A[1]);
    vec3 vect = { x/std::sqrt(x*x + y*y + z*z),y/std::sqrt(x*x + y*y + z*z), z/std::sqrt(x*x + y*y + z*z) };
    return vect ;
}


std::vector<vec3> transpose(std::vector<vec3> mat){
    std::vector<vec3> new_mat;
    for (int colone = 0; colone < 3;colone++){
        vec3 v ;
        for (long unsigned int ligne = 0; ligne < mat.size();ligne++){
            v[colone] = (mat[ligne][colone]);
        }
        new_mat.push_back(v);
    }
    return new_mat;
}

void dist(std::vector<vec3> mat, std::vector<vec3> mat2){
    int err;
    for (int colone = 0; colone < 3 ;colone++){
        for (long unsigned int ligne = 0; ligne < mat.size();ligne++){
            err += std::abs(mat[ligne][colone]-mat2[ligne][colone]);
        }
    }
    std::cout<< err <<" \n";
}

mat<4,4> ModelView (vec3 eye, vec3 center, vec3 up){
    vec3 z = (eye-center).normalized();
    vec3 x = cross(up,z).normalized();
    vec3 y = cross(z,x).normalized();
    mat<4,4> Minv ; Minv = Minv.identity();
    mat<4,4> Tr   = mat<4,4>::identity();
    for (int i=0; i<3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = -eye[i];
    }
    return Minv * Tr;
}
mat<4,4> viewport(int x, int y, int w, int h) {
    mat<4,4> m = mat<4,4>::identity();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = 255/1.f;

    m[0][0] = w/1.f;
    m[1][1] = h/1.f;
    m[2][2] = 255/1.f;
    return m;
}

std::vector<vec3> compute_perspective( std::vector<vec3>vertices, vec3 eye, vec3 center, vec3 up, int width, int height,mat<4,4> *M){
    vec3 cam_vector =  (eye-center);
    std::vector<vec3> new_vertices;
    mat<4,4> proj = mat<4,4>::identity();
    proj[3][2] = -1/cam_vector[2];
    mat<4,4> view = viewport(center[0],center[1],width, height);
    mat<4,4> model = ModelView(eye,center,up);
    for (long unsigned int i = 0 ; i < vertices.size(); i++){
        vec4 aux2 = {vertices[i][0],vertices[i][1],vertices[i][2],1};
        aux2 = proj*model*aux2;
        aux2 = {aux2[0]/aux2[3],aux2[1]/aux2[3],aux2[2]/aux2[3],1};
        aux2 = view*aux2;
        vec3 aux = {aux2[0]/aux2[3],aux2[1]/aux2[3],aux2[2]/aux2[3]};
        new_vertices.push_back(aux);
    }
    (*M) = view*proj*model;
    return new_vertices;
}