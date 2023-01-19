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
    //file to process
    std::ifstream file("tinyrenderer/obj/diablo3_pose/diablo3_pose.obj");
    std::string myText; 
    std::vector<std::vector<int> > vertices;
    std::vector<std::vector<int> > vertices_texture;
    std::vector<std::vector<int> > faces;
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
            std::vector<int> v = {std::round((x*width/2)+width/2),std::round((y*height/2)+height/2),std::round((z*height/2)+height/2)};
            vertices_texture.push_back(v);
        }
        
        if( type == "f"){   
            int x, y, z;
            std::string coord;
            file >>  coord;
            x = std::stoi(coord.substr(0,coord.find("/")));
            file >>  coord;
            y = std::stoi(coord.substr(0,coord.find("/")));
            file >>  coord;
            z = std::stoi(coord.substr(0,coord.find("/")));
            if (x <= vertices.size() && y <= vertices.size() && z <= vertices.size()){
                std::vector<int> v = {x-1,y-1,z-1};
                faces.push_back(v);
            }
        } 
    }
    file.close();
    TGAImage framebuffer(width, height, TGAImage::RGB);
    draw_triangles(faces,vertices, vertices_texture, framebuffer, red, width, height);
    //framebuffer.set(10,10, red);
    framebuffer.write_tga_file("output.tga");
    return 0;
}