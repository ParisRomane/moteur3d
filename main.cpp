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



int main(int argc, char** argv) {
    // dimensions
    constexpr int width = 512 ;
    constexpr int height = 512 ;
    constexpr int t_width = 1023;
    constexpr int t_height = 1023;
    //file to process
    std::ifstream file("tinyrenderer/obj/diablo3_pose/diablo3_pose.obj");
    std::string myText; 
    std::vector<std::vector<int> > vertices;
    std::vector<std::vector<int> > vertices_texture;
    std::vector<std::vector<int> > faces;
    std::vector<std::vector<int> > t_faces;
    std::string type;
    while ( file >> type ){

        if( type == "v"){
            float x, y, z;
            file >> x >> y >> z ;
            std::vector<int> v = {std::round((x*width/2)+width/2),std::round((y*height/2)+height/2),std::round((z*height/2)+height/2)};
            vertices.push_back(v);
        }
                
        if( type == "vt"){
            float x, y, z;
            file >> x >> y >> z ;
            std::vector<int> v = {std::round(x*t_width),std::round(y*t_height),std::round(z*t_height)};
            vertices_texture.push_back(v);
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
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage texture(1,1,TGAImage::RGB);
    texture.read_tga_file("tinyrenderer/obj/diablo3_pose/diablo3_pose_diffuse.tga");
    texture.flip_vertically();
    draw_triangles(faces,vertices, t_faces, vertices_texture, framebuffer, texture, red, width, height);
    //framebuffer.set(10,10, red);
    framebuffer.write_tga_file("output.tga");
    matices_multiplication( {{1.5,0},{0,1.5}}  , { {1,1,1,0,-1},{-1,-1,0,1,1} });
    return 0;
}