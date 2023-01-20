#include <vector>
#include <math.h>

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

std::vector<std::vector<float>> matices_multiplication (std::vector<std::vector<float>> mult, std::vector<std::vector<float>> list_of_coords){
    int mat_size = mult.size();
    int list_size = list_of_coords[0].size();
    std::vector<std::vector<float>> list_coords_process;
    for (int ligne = 0; ligne < mat_size;ligne++){
        std::vector<float> v ;
        for (int coords_indice = 0; coords_indice < list_size;coords_indice++){
            float temp = 0;
            for (int col = 0; col < mat_size;col++){
            temp += mult[ligne][col] * list_of_coords[col][coords_indice] ;
            }
            v.push_back(temp);
        }
        list_coords_process.push_back(v);
    }
    return list_coords_process;

}