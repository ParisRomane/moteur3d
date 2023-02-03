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
std::vector<std::vector<float>> draw_full_triangle_with_texture(std::vector<vec3> vertices,std::vector<vec3 > vertices_t, 
std::vector<vec3 > vn, TGAImage &img,TGAImage &texture, std::vector<std::vector<float>> z_buffer,vec3 cam_vector, 
int width, int height ){
    vec3 A = vertices[0];
    vec3 B = vertices[1];
    vec3 C = vertices[2];
    vec3 At = vertices_t[0];
    vec3 Bt = vertices_t[1];
    vec3 Ct = vertices_t[2];
    int xmax = std::max(std::max(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymax = std::max(std::max(vertices[0][1],vertices[1][1]),vertices[2][1]);
    int xmin = std::min(std::min(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymin = std::min(std::min(vertices[0][1],vertices[1][1]),vertices[2][1]);
        for(int x = std::max(0,xmin); x <= std::min(xmax,width); x++)
        {
        for(int y= std::max(0,ymin); y <= std::min(ymax,height); y++)
        {
            vec3 baricentric_coord =  baricentric(A,B,C,x,y);
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
            float normal = n_x * cam_vector[0] + n_y * cam_vector[1] + n_z * cam_vector[2];
            //std::cout << normal << " " << std::sqrt(n_x*n_x + n_y*n_y + n_z*n_z) <<" \n"; //c'est moyen normal.... quelques écart par rapport à 1...
            if (baricentric_coord[0] >= 0 && baricentric_coord[1] >= 0 && baricentric_coord[2] >= 0 && normal > 0 )
            {
                if(z_buffer[x][y] < z){
                    z_buffer[x][y]  = z;
                    //TGAColor color = TGAColor(std::max(int(texture.get(tx,ty)[2] *normal), 0),
                    //std::max(int(texture.get(tx,ty)[1] *normal),0),
                    //std::max(int(texture.get(tx,ty)[0]*normal),0));

                    TGAColor color = TGAColor(int(200*normal),int(200*normal),int(200*normal));
                    //img.set(x,y,texture.get(tx,ty));
                    
                    img.set(x,y,color);
                }
            }
        }
    }
    return z_buffer;
}
void draw_triangles(std::vector<vec3 > faces, std::vector<vec3> vertices,std::vector<vec3> vertice_normals,std::vector<vec3> t_faces, std::vector<vec3> vertices_texture,
 TGAImage &img, TGAImage &texture, int width, int height, vec3 cam_vector){
    std::vector<std::vector<float>> z_buffer(width, std::vector<float>(height));
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            z_buffer[i][j] = -std::numeric_limits<float>::infinity();
        }
    }
    for (long unsigned int i = 0; i < faces.size(); i++)
    {
        z_buffer = draw_full_triangle_with_texture({vertices[faces[i][0]],vertices[faces[i][1]],vertices[faces[i][2]]},
        {vertices_texture[t_faces[i][0]],vertices_texture[t_faces[i][1]],vertices_texture[t_faces[i][2]]}, 
        {vertice_normals[faces[i][0]],vertice_normals[faces[i][1]],vertice_normals[faces[i][2]]}, 
         img, texture, z_buffer, cam_vector,width,height);
    }
}

std::vector<vec3> resize(std::vector<vec3> mat,  int width, int height){
    std::vector<vec3> new_mat;
    for (long unsigned int ligne = 0; ligne < mat.size(); ligne++){
        vec3 v = {std::round((mat[ligne][0]*width/2)+width/2),std::round((mat[ligne][1]*height/2)+height/2),std::round((mat[ligne][2]*height/2)+height/2)};
        new_mat.push_back(v);
    }
    return new_mat;
}