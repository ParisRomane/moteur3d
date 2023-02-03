#include "tgaimage.h"
#include <iostream>    
#include <regex> 
#include <fstream>    
#include <vector>
#include <math.h>
#include "display.h"
// f -> triangles, premier sommet c'est 1 pas 0, il faut décrementer.

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
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
        /* path_struct = argv[1];
         path_texture = argv[2];
         eye = argv[3];
         center = argv[4];*/
    }else {
        std::cout << " le nombre d'arguments rentrée est insufisant : " << argc<<"\n Arguments par défaut pris. \n";
    }
    //file to process
    std::ifstream file(path_struct);
    std::string myText; 
    std::vector<std::vector<float> > vertices;
    std::vector<std::vector<int> > vertices_texture;
    std::vector<std::vector<float> > vertice_normals;
    std::vector<std::vector<int> > faces;
    std::vector<std::vector<int> > t_faces;
    std::string type;
    while ( file >> type ){

        if( type == "v"){
            float x, y, z;
            file >> x >> y >> z ;
            std::vector<float> v = {x,y,z};
            vertices.push_back(v);
        }
                
        if( type == "vt"){
            float x, y, z;
            file >> x >> y >> z ;
            std::vector<int> v = {std::round(x*t_width),std::round(y*t_height),std::round(z*t_height)};
            vertices_texture.push_back(v);
        }
        if( type == "vn"){
            float x, y, z;
            file >> x >> y >> z ;
            std::vector<float> v = {x,y,z};
            vertice_normals.push_back(v);
        }
        if( type == "f"){   
            int x, y, z, tx ,ty, tz;
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
            std::vector<int> v = {x-1,y-1,z-1};
            faces.push_back(v);
            std::vector<int> tv = {tx-1,ty-1,tz-1};
            t_faces.push_back(tv);
        } 
    }
    file.close();
    //END file process

    // Compute perspective
    std::vector<float> cam_vector = {0,2,2};
    std::vector<std::vector<float> > computed_vertices = compute_perspective(vertices,cam_vector);
    std::vector<std::vector<int>> new_vertices =resize(computed_vertices, width, height);
    std::vector<float> light_vector = {0,0,1};
    //load texture.
    TGAImage texture(1,1,TGAImage::RGB);
    texture.read_tga_file(path_texture);
    texture.flip_vertically();
    // creating the image
    TGAImage framebuffer(width, height, TGAImage::RGB);
    draw_triangles(faces, new_vertices, vertice_normals, t_faces, vertices_texture, framebuffer, texture, red, width, height, light_vector);

    framebuffer.write_tga_file("output.tga");
    return 0;
}