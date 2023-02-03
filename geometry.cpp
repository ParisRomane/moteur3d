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

std::vector<vec3> retro_projection(float c,std::vector<vec3> list_of_coords){
    std::vector<vec3> list_coords_process;
    for (int ligne = 0; ligne < list_of_coords.size();ligne++){
        vec3 v ;
        for (int coords_indice = 0; coords_indice < 3;coords_indice++){
            if (((list_of_coords[ligne][2])/c) != 1 ){
                v[coords_indice] = (list_of_coords[ligne][coords_indice]/(1-(list_of_coords[ligne][2]/c) ));
            }else{v[coords_indice] = (list_of_coords[ligne][coords_indice]);std::cout<<"here \n";}
        }
        list_coords_process.push_back(v);
    }
    return list_coords_process;
}


std::vector<vec3> transpose(std::vector<vec3> mat){
    std::vector<vec3> new_mat;
    for (int colone = 0; colone < 3;colone++){
        vec3 v ;
        for (int ligne = 0; ligne < mat.size();ligne++){
            v[colone] = (mat[ligne][colone]);
        }
        new_mat.push_back(v);
    }
    return new_mat;
}

void dist(std::vector<vec3> mat, std::vector<vec3> mat2){
    int err;
    for (int colone = 0; colone < 3 ;colone++){
        for (int ligne = 0; ligne < mat.size();ligne++){
            err += std::abs(mat[ligne][colone]-mat2[ligne][colone]);
        }
    }
    std::cout<< err <<" \n";
}
std::vector<vec3> compute_perspective( std::vector<vec3>vertices, vec3 cam_vector){
    /*//creation du vecteur "caméra projection"
    std::vector<vec3> proj = { {1,0,0,0},{0,1,0,0}, {0,0,1,0}};
    vec3 v;
    for (int ind = 0; ind < cam_vector.size();ind++){
        if (cam_vector[ind] != 0){
            v.push_back(-1.0/cam_vector[ind]);
        }else{v.push_back(0);}
    }
    v.push_back(1);
    proj.push_back(v);
    std::vector<vec3> new_vertices = vertices;
    // il faut mettre la 4ème dimention de vertices avec 1 partout.
    for (int ind = 0; ind < vertices.size();ind++){
        new_vertices[ind].push_back(1);
    }
    new_vertices = transpose(new_vertices);
    */
    std::vector<vec3> new_vertices = retro_projection(cam_vector[2],vertices);
    return new_vertices;
}
