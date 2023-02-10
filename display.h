#include "tgaimage.h"
#include <vector>
#include <math.h>
#include "vec3.h"
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
TGAImage &shadder_normals, TGAImage &img,TGAImage &texture, std::vector<std::vector<float>> z_buffer,vec3 light_vector, 
int width, int height, vec3 cam ){
    vec3 A = vertices[0];
    vec3 B = vertices[1];
    vec3 C = vertices[2];
    int xmax = std::max(std::max(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymax = std::max(std::max(vertices[0][1],vertices[1][1]),vertices[2][1]);
    int xmin = std::min(std::min(vertices[0][0],vertices[1][0]),vertices[2][0]);
    int ymin = std::min(std::min(vertices[0][1],vertices[1][1]),vertices[2][1]);
    for(int x = std::max(0,xmin); x <= std::min(xmax,width-1); x++)
    {
        for(int y= std::max(0,ymin); y <= std::min(ymax,height-1); y++)
        {
            vec3 baricentric_coord =  baricentric(A,B,C,x,y);
            float z, t_x, t_y;
            z = t_x = t_y  = 0;
            for (int v : {0,1,2}) {
                z += baricentric_coord[v]*vertices[v][2];
                t_x += baricentric_coord[v]*vertices_t[v][0];
                t_y += baricentric_coord[v]*vertices_t[v][1];
            }
            int tx = int(t_x);
            int ty = int(t_y);
            TGAColor shadder_normal = shadder_normals.get(tx,ty);
            vec3 sn = {shadder_normal[0] , shadder_normal[1] , 2*(shadder_normal[2]-128) };
            sn = sn.normalized();
            float diffuse = sn*light_vector;
            vec3 reflexion = ( 2 *sn*(sn*light_vector) - light_vector).normalized();
            float specular = (reflexion * cam)*(reflexion * cam)*(reflexion * cam);
            //std::cout << normal << " " << std::sqrt(n_x*n_x + n_y*n_y + n_z*n_z) <<" \n"; //c'est moyen normal.... quelques écart par rapport à 1...
            if (baricentric_coord[0] >= 0 && baricentric_coord[1] >= 0 && baricentric_coord[2] >= 0 && diffuse > 0 )
            {
                if(z_buffer[x][y] < z){
                    z_buffer[x][y]  = z;
                    TGAColor color = TGAColor(std::min(int(255 *(specular)), 255),
                    std::min(int(255 *(specular)),255),
                    std::min(int(255*(specular)),255));
                    //img.set(x,y,texture.get(tx,ty));

                    //TGAColor color = TGAColor(int(200*diffuse),int(200*diffuse),int(200*diffuse));
                    img.set(x,y,color);
                }
            }
        }
    }
    return z_buffer;
}
void draw_triangles(std::vector<vec3 > faces, std::vector<vec3> vertices,TGAImage &shadder_normals,std::vector<vec3> t_faces, std::vector<vec3> vertices_texture,
 TGAImage &img, TGAImage &texture, int width, int height, vec3 light_vector,vec3 cam){
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
        shadder_normals, 
         img, texture, z_buffer, light_vector,width,height,cam.normalized());
    }
}
