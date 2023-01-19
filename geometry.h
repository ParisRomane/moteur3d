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