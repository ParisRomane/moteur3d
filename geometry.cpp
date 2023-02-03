#include "geometry.h"
#include <vector>
#include <math.h>
#include <iostream>

vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec<3>{v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}


std::vector<float> baricentric(std::vector<int> A, std::vector<int> B,std::vector<int> C, int x, int y){
    float aire_ABC = (B[0] - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(B[1] - A[1]);
    float aire_APC = (x - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(y - A[1]);
    float aire_PBC = (B[0] - x)*(C[1] - y) -  (C[0] - x)*(B[1] - y);
    float aire_ABP = (B[0] - A[0])*(y - A[1]) -  (x - A[0])*(B[1] - A[1]);
    std::vector<float> vect = {aire_PBC/aire_ABC, aire_APC/aire_ABC, aire_ABP/aire_ABC};
    return vect ;
}
std::vector<float> normal_vect(std::vector<int> A, std::vector<int> B, std::vector<int> C){
    float x = (B[2] - A[2])*(C[1] - A[1]) -  (C[2] - A[2])*(B[1] - A[1]);
    float y = (B[0] - A[0])*(C[2] - A[2]) -  (C[0] - A[0])*(B[2] - A[2]);
    float z = (B[0] - A[0])*(C[1] - A[1]) -  (C[0] - A[0])*(B[1] - A[1]);
    std::vector<float> vect = { x/std::sqrt(x*x + y*y + z*z),y/std::sqrt(x*x + y*y + z*z), z/std::sqrt(x*x + y*y + z*z) };
    return vect ;
}

std::vector<std::vector<float>> retro_projection(float c,std::vector<std::vector<float>> list_of_coords){
    std::vector<std::vector<float>> list_coords_process;
    for (int ligne = 0; ligne < list_of_coords.size();ligne++){
        std::vector<float> v = {} ;
        for (int coords_indice = 0; coords_indice < 3;coords_indice++){
            if (((list_of_coords[ligne][2])/c) != 1 ){
                v.push_back(list_of_coords[ligne][coords_indice]/(1-(list_of_coords[ligne][2]/c) ));
            }else{v.push_back(list_of_coords[ligne][coords_indice]);std::cout<<"here \n";}
        }
        list_coords_process.push_back(v);
    }
    return list_coords_process;
}


std::vector<std::vector<float>> transpose(std::vector<std::vector<float>> mat){
    std::vector<std::vector<float>> new_mat;
    for (int colone = 0; colone < mat[0].size();colone++){
        std::vector<float> v ;
        for (int ligne = 0; ligne < mat.size();ligne++){
            v.push_back(mat[ligne][colone]);
        }
        new_mat.push_back(v);
    }
    return new_mat;
}

void dist(std::vector<std::vector<float>> mat, std::vector<std::vector<float>> mat2){
    int err;
    for (int colone = 0; colone < mat[0].size();colone++){
        for (int ligne = 0; ligne < mat.size();ligne++){
            err += std::abs(mat[ligne][colone]-mat2[ligne][colone]);
        }
    }
    std::cout<< err <<" \n";
}
std::vector<std::vector<float>> compute_perspective( std::vector<std::vector<float>>vertices, std::vector<float> cam_vector){
    /*//creation du vecteur "caméra projection"
    std::vector<std::vector<float>> proj = { {1,0,0,0},{0,1,0,0}, {0,0,1,0}};
    std::vector<float> v;
    for (int ind = 0; ind < cam_vector.size();ind++){
        if (cam_vector[ind] != 0){
            v.push_back(-1.0/cam_vector[ind]);
        }else{v.push_back(0);}
    }
    v.push_back(1);
    proj.push_back(v);
    std::vector<std::vector<float>> new_vertices = vertices;
    // il faut mettre la 4ème dimention de vertices avec 1 partout.
    for (int ind = 0; ind < vertices.size();ind++){
        new_vertices[ind].push_back(1);
    }
    new_vertices = transpose(new_vertices);
    */
    std::vector<std::vector<float>> new_vertices = retro_projection(cam_vector[2],vertices);
    return new_vertices;
}
