#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"read-obj.h"

namespace pipeline3D {

    // reads an obj file and creates the list of triangles
    // does not implement vp, materials, named objects and groups, smooth shading flag, relative indices, texture map specification
    std::vector<std::array<Vertex,3>> read_obj(const char* file) {
        std::vector<std::array<Vertex,3>> result;
        std::vector<std::array<float, 3>> vertices;
        std::vector<std::array<float, 2>> t_coords;
        std::vector<std::array<float, 3>> norms;

        //add dummy entries as face indices start with 1.
        //used for non-specified texture coordinates or normals;
        vertices.push_back(std::array<float, 3>{0.0f,0.0f,0.0f});
        t_coords.push_back(std::array<float, 2>{0.0f,0.0f});
        norms.push_back(std::array<float, 3>{0.0f,0.0f,0.0f});
        
        std::ifstream in(file);
        
        while (in) {
            std::string str;
            std::getline(in, str);
            std::stringstream tokenizer(str, std::ios_base::in);
            std::string head;
            tokenizer >> head;

            if (head=="v") { // xyz specification
                float x, y, z;
                tokenizer >> x >> y >> z;
                //std::cout << "vertex: {" << x << ", " << y << ", " << z << "}\n";
                vertices.push_back(std::array<float, 3>{x,y,z});
            } else if (head == "vt") {
                float u, v;
                tokenizer >> u >> v;
                //std::cout << "texture: {" << u << ", " << u << "}\n";
                t_coords.push_back(std::array<float, 2>{u,v});
            } else if (head == "vn") {
                float nx, ny, nz;
                tokenizer >> nx >> ny >> nz;
                //std::cout << "normal: {" << nx << ", " << ny << ", " << nz << "}\n";
                norms.push_back(std::array<float, 3>{nx,ny,nz});
            } else if (head == "f") {
                std::array<int,3> face({0,0,0}), texture({0,0,0}), norm({0,0,0});
                for(int i=0; i!=3; ++i)
                {
                    std::string token;
                    tokenizer >> token;
                    std::stringstream fin(token, std::ios_base::in);
                    fin >> face[i];
                    char c = fin.peek();
                    if (c == '/') {
                        fin >> c;
                        fin >> texture[i];
                        char c = fin.peek();
                        if (c == '/') {
                            fin >> c;
                            fin >> norm[i];
                        }
                    }
                }


                std::array<float, 3> &v1 = vertices[face[0]];
                std::array<float, 3> &v2 = vertices[face[1]];
                std::array<float, 3> &v3 = vertices[face[2]];
                std::array<float, 2> &t1 = t_coords[texture[0]];
                std::array<float, 2> &t2 = t_coords[texture[1]];
                std::array<float, 2> &t3 = t_coords[texture[2]];
                std::array<float, 3> &n1 = norms[norm[0]];
                std::array<float, 3> &n2 = norms[norm[1]];
                std::array<float, 3> &n3 = norms[norm[2]];

                /*
                std::cerr << "vertex1={coordinate: " << face[0] << "; texture: " << texture[0] << "; normal: " << norm[0] << "; } -> {";
                std::cerr << v1[0] << ", " << v1[1] << ", " << v1[2] << ", " << n1[0] << ", " << n1[1] << ", " << n1[2] << ", " << t1[0] << ", " << t1[1] << "}\n";
                std::cerr << "vertex1={coordinate: " << face[1] << "; texture: " << texture[1] << "; normal: " << norm[1] << "; } -> {";
                std::cerr << v2[0] << ", " << v2[1] << ", " << v2[2] << ", " << n2[0] << ", " << n2[1] << ", " << n2[2] << ", " << t2[0] << ", " << t2[1] << "}\n";
                std::cerr << "vertex1={coordinate: " << face[2] << "; texture: " << texture[2] << "; normal: " << norm[2] << "; } -> {";
                std::cerr << v3[0] << ", " << v3[1] << ", " << v3[2] << ", " << n3[0] << ", " << n3[1] << ", " << n3[2] << ", " << t3[0] << ", " << t3[1] << "}\n";
                */

                result.push_back(std::array<Vertex,3>{
                                     Vertex({v1[0], v1[1], v1[2], n1[0], n1[1], n1[2], t1[0], t1[1]}),
                                     Vertex({v2[0], v2[1], v2[2], n2[0], n2[1], n2[2], t2[0], t2[1]}),
                                     Vertex({v3[0], v3[1], v3[2], n3[0], n3[1], n3[2], t3[0], t3[1]}),
                                    });
            }
        }
        //std::cout << "Result size: " << result.size() << " \n";
        return result;
    }

};
