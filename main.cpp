#include "tgaimage.h"
#include <iostream>    
#include <regex> 
#include <fstream>    
#include <vector>
#include <math.h>
#include "vec3.h"
#include "display.h"
// f -> triangles, premier sommet c'est 1 pas 0, il faut décrementer.


const int depth = 255;
//for the output.
int width =  800 ;
int height = 800 ;
// to be displayed
vec3 eye = vec3({1,1,3});
vec3 center = vec3({0,0,0});
vec3 up = {0,1,0};
std::string path_struct = "african_head/african_head.obj";
std::string path_texture = "african_head/african_head_diffuse.tga";
std::string path_shadder = "african_head/african_head_nm.tga";

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
    mat<4,4> mat;
    std::vector<vec3 > new_vertices = compute_perspective(vertices,eye,center,up,width,height, &mat);
    vec3 light_vector = {1,0,1}; light_vector = light_vector.normalized();
    std::vector<vec3 > shadow_vertices = compute_perspective(vertices,light_vector,center,up,width/2,height/2, &mat);
    // load shadder 
    TGAImage shadder(1,1,TGAImage::RGB);
    shadder.read_tga_file(path_shadder);
    shadder.flip_vertically();
    //load texture.
    TGAImage texture(1,1,TGAImage::RGB);
    texture.read_tga_file(path_texture);
    texture.flip_vertically();
    // creating the image
    TGAImage framebuffer(width, height, TGAImage::RGB);
    draw_triangles(faces, new_vertices, shadder, t_faces, vertices_texture, framebuffer, texture, width, height, light_vector, center-eye, shadow_vertices, mat);

    framebuffer.write_tga_file("output.tga");
    return 0;
}