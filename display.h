#include "tgaimage.h"
#include <vector>
#include <math.h>
#include "geometry.h"


void line(int x0, int y0, int x1, int y1, TGAImage &img, TGAColor color){
    bool steep = false;
    if (std::abs(x1-x0) < std::abs(y1-y0)){
        std::swap(x1,y1);
        std::swap(x0,y0);
        steep = true;
    }
    if(x0 >x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int y = y0;
    int derror = 2*std::abs(dy); // float derror = std::abs(dy); float derror =  std::abs(dy/static_cast<float>(dx));
    int error = 0;
    for (int x=x0; x<=x1; x++){
        if (steep){
            img.set(y,x, color);
        }
        else {
            img.set(x,y, color);
        }
        error += derror;
        if (error >dx){
            y += dy > 0 ? 1 : -1;
            error -= 2*dx;
        }
    }
}
// ne pas oublier que l'on peut paralleliser certains algos et pas d'autres.
// choisir conter-clock (+) / clock (-) wise aire triangles
std::vector<std::vector<float>> draw_full_triangle_with_texture(std::vector<std::vector<int> > vertices,std::vector<std::vector<int> > vertices_t, 
std::vector<std::vector<float> > vn, TGAImage &img,TGAImage &texture, std::vector<std::vector<float>> z_buffer,std::vector<float> cam_vector, 
int width, int height ){
    std::vector<int> A = vertices[0];
    std::vector<int> B = vertices[1];
    std::vector<int> C = vertices[2];
    std::vector<int> At = vertices_t[0];
    std::vector<int> Bt = vertices_t[1];
    std::vector<int> Ct = vertices_t[2];
    int xmax = std::max(std::max(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymax = std::max(std::max(vertices[0][1],vertices[1][1]),vertices[2][1]);
    int xmin = std::min(std::min(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymin = std::min(std::min(vertices[0][1],vertices[1][1]),vertices[2][1]);
    for(int x = std::max(0,xmin); x <= std::min(xmax,width); x++)
    {
        for(int y= std::max(0,ymin); y <= std::min(ymax,height); y++)
        {
            std::vector<float> baricentric_coord =  baricentric(A,B,C,x,y);
            float z, t_x, t_y, n_x,n_y,n_z;
            z = t_x = t_y = n_x = n_y = n_z = 0;
            for (int v : {0,1,2}) {
                z += baricentric_coord[v]*vertices[v][2];
                t_x += baricentric_coord[v]*vertices_t[v][0];
                t_y += baricentric_coord[v]*vertices_t[v][1];
                n_x += baricentric_coord[v]*vn[v][0];
                n_y +=baricentric_coord[v]*vn[v][1];
                n_z +=baricentric_coord[v]*vn[v][2];
            }
            int tx = int(t_x);
            int ty = int(t_y);
            float normal = normal_v[0] * cam_vector[0] + normal_v[1] * cam_vector[1] + normal_v[2] * cam_vector[2];

            if (normal>0 && baricentric_coord[0] >= 0 && baricentric_coord[1] >= 0 && baricentric_coord[2] >= 0)
            {
                if(z_buffer[x][y] < z){
                    z_buffer[x][y]  = z;
                    TGAColor color = TGAColor(std::max(int(texture.get(tx,ty)[2] *normal), 0),
                    std::max(int(texture.get(tx,ty)[1] *normal),0),
                    std::max(int(texture.get(tx,ty)[0]*normal),0));
                    //img.set(x,y,texture.get(tx,ty));
                    
                    img.set(x,y,color);
                }
            }
        }
    }
    return z_buffer;
}
void draw_triangles(std::vector<std::vector<int> > faces, std::vector<std::vector<int> > vertices,std::vector<std::vector<float> > vertice_normals,std::vector<std::vector<int> > t_faces, std::vector<std::vector<int> > vertices_texture, TGAImage &img, TGAImage &texture, int width, int height, std::vector<float> cam_vector){
    std::vector<std::vector<float>> z_buffer(width, std::vector<float>(height));
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::infinity();
        }
    }
    for (int i = 0; i < faces.size(); i++)
    {
        z_buffer = draw_full_triangle_with_texture({vertices[faces[i][0]],vertices[faces[i][1]],vertices[faces[i][2]]},
        {vertices_texture[t_faces[i][0]],vertices_texture[t_faces[i][1]],vertices_texture[t_faces[i][2]]}, 
        {vertice_normals[faces[i][0]],vertice_normals[faces[i][1]],vertice_normals[faces[i][2]]}, 
         img, texture, z_buffer, cam_vector,width,height);
    }
}

std::vector<std::vector<int>> resize(std::vector<std::vector<float>> mat,  int width, int height){
    std::vector<std::vector<int>> new_mat;
    for (int ligne = 0; ligne < mat.size(); ligne++){
        std::vector<int> v = {int((mat[ligne][0]*width/2)+width/2),int((mat[ligne][1]*height/2)+height/2),int((mat[ligne][2]*height/2)+height/2)};
        new_mat.push_back(v);
    }
    return new_mat;
}