#include "tgaimage.h"
#include <iostream>    
#include <regex> 
#include <fstream>    
#include <vector>
#include <math.h>
#include "display.h"
// f -> triangles, premier sommet c'est 1 pas 0, il faut décrementer.


const int depth = 512;
mat<3,3> ModelView;
//for the output.
constexpr int width = 512 ;
constexpr int height = 512 ;
// to be displayed
vec3 eye = vec3({0,0,1});
vec3 center = vec3({1,1,1});
std::string path_struct = "tinyrenderer/obj/african_head/african_head.obj";
std::string path_texture = "tinyrenderer/obj/african_head/african_head_diffuse.tga";


//int argc, char** argv
int main(int argc, char** argv) {
    /// arg1 : path_struct, arg2 : path_texture, arg3 : eye, arg4 : center -> canvas dimension ?
    // dimensions
    constexpr int t_width = 1023;
    constexpr int t_height = 1023;
    // arg input.
    if (argc == 5){
        path_struct = argv[1];
         path_texture = argv[2];
        /* eye = argv[3];
         center = argv[4];*/
    }else {
        std::cout << " le nombre d'arguments rentrée est insufisant : " << argc<<"\n Arguments par défaut pris. \n";
    }
    //FILE PROSSESSING
    std::ifstream file(path_struct);
    std::string myText; 
    std::vector<vec3 > vertices;
    std::vector<vec3 > vertices_texture;
    std::vector<vec3 > vertice_normals;
    std::vector<vec3> faces;
    std::vector<vec3> t_faces;
    std::string type;
    while ( file >> type ){

        if( type == "v"){
            double x, y, z;
            file >> x >> y >> z ;
            vec3 v = {x,y,z};
            vertices.push_back(v);
        }
                
        if( type == "vt"){
            double x, y, z;
            file >> x >> y >> z ;
            vec3 v = {std::round(x*t_width),std::round(y*t_height),std::round(z*t_height)};
            vertices_texture.push_back(v);
        }
        if( type == "vn"){
            double x, y, z;
            file >> x >> y >> z ;
            vec3 v = {x,y,z};
            vertice_normals.push_back(v);
        }
        if( type == "f"){   
            double x, y, z, tx ,ty, tz;
            std::string coord;
            file >>  coord;
            x = std::stoi(coord.substr(0,coord.find("/")));
            coord = coord.substr(coord.find("/")+1,coord.length());
            tx = std::stoi(coord.substr(0,coord.find("/")));
            file >>  coord;
            y = std::stoi(coord.substr(0,coord.find("/")));
            coord = coord.substr(coord.find("/")+1,coord.length());
            ty = std::stoi(coord.substr(0,coord.find("/")));
            file >>  coord;
            z = std::stoi(coord.substr(0,coord.find("/")));
            coord = coord.substr(coord.find("/")+1,coord.length());
            tz = std::stoi(coord.substr(0,coord.find("/")));
            vec3 v = {x-1,y-1,z-1};
            faces.push_back(v);
            vec3 tv = {tx-1,ty-1,tz-1};
            t_faces.push_back(tv);
        } 
    }
    file.close();
    //END FILE PROSSESSING

    // Compute perspective
    vec3 cam_vector = {0,2,2};
    std::vector<vec3 > computed_vertices = compute_perspective(vertices,cam_vector);
    std::vector<vec3> new_vertices =resize(computed_vertices, width, height);
    vec3 light_vector = {1,-1,1}; light_vector = light_vector.normalized();
    //load texture.
    TGAImage texture(1,1,TGAImage::RGB);
    texture.read_tga_file(path_texture);
    texture.flip_vertically();
    // creating the image
    TGAImage framebuffer(width, height, TGAImage::RGB);
    draw_triangles(faces, new_vertices, vertice_normals, t_faces, vertices_texture, framebuffer, texture, width, height, light_vector);

    framebuffer.write_tga_file("output.tga");
    return 0;
}