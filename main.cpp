#include "tgaimage.h"
#include <iostream>    
#include <regex> 
#include <fstream>    
#include <vector>
#include <math.h>
#include "display.h"
// f -> triangles, premier sommet c'est 1 pas 0, il faut décrementer.


// dimensions
constexpr int width = 512 ;
constexpr int height = 512 ;
constexpr int t_width = 1023;
constexpr int t_height = 1023;

//int argc, char** argv
int main() {
    //file to process
    std::ifstream file("tinyrenderer/obj/african_head/african_head.obj");
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
    std::vector<float> cam_vector = {0,0,2};
    std::vector<std::vector<float> > computed_vertices = compute_perspective(vertices,cam_vector);
    std::vector<std::vector<int>> new_vertices =resize(computed_vertices, width, height);
    cam_vector = {0,0,1.};
    // creating the image
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage texture(1,1,TGAImage::RGB);
    texture.read_tga_file("tinyrenderer/obj/african_head/african_head_diffuse.tga");
    texture.flip_vertically();
    draw_triangles(faces, new_vertices, vertice_normals, t_faces, vertices_texture, framebuffer, texture, width, height, cam_vector);
    //framebuffer.set(10,10, red);
    framebuffer.write_tga_file("output.tga");
    return 0;
}