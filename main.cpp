#include"rasterization.h"
#include"scene.h"
#include"read-obj.h"
#include"threadPool.hpp"
using namespace pipeline3D;

#include<iostream>

using namespace std;

struct my_shader{
    char operator ()(const Vertex &v)  {
        return static_cast<char>((v.z-1)*10.0f+0.5f)%10+'0';
    }
};


int main() {
    
    const int w = 150;
    const int h = 50;
    std::vector<char> screen(w*h,'.');
    
    my_shader shader;
    Rasterizer<char> rasterizer;
    
    rasterizer.set_perspective_projection(-1,1,-1,1,1,2);
    rasterizer.set_target(w, h, &screen[0]);
    
    Scene<char> scene;
    scene.view_ = {0.5f,0.0f,0.0f,0.7f,0.0f,0.5f,0.0f,0.7f,0.0f,0.0f,0.5f,0.9f,0.0f,0.0f,0.0f,1.0f};
    
    std::vector<std::array<Vertex,3>> meshCube = read_obj("/Users/filippo/cubeMod.obj");

    // change the argument to choose #threads that must be > 0
    ThreadPool tp;
    
    
    /* Benchmark mode */
    //auto start_time = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; i++){
        // add the same object at the same position 1Mln times by copy and not using std::move
        scene.add_object(Scene<char>::Object(meshCube,shader));
    }
    
    scene.render(rasterizer, &tp);
    
    //auto end_time = std::chrono::high_resolution_clock::now();
    
    /* Classic render*/
//    scene.add_object(Scene<char>::Object(std::move(meshCube),shader));
//    scene.render(rasterizer, &tp);

    tp.joinAll();
    
    // print out the screen with a frame around it
    std::cout << '+';
    for (int j = 0; j != w; ++j) std::cout << '-';
    std::cout << "+\n";

    for (int i = 0;i != h;++i) {
        std::cout << '|';
        for (int j = 0; j != w ; ++j) std::cout << screen[i*w+j];
        std::cout << "|\n";
    }

    std::cout << '+';
    for (int j = 0; j != w ; ++j) std::cout << '-';
    std::cout << "+\n";
    
    //std::cout << "Elapsed time: " << std::chrono::duration<double>(end_time-start_time).count() << '\n';
    
    return 0;
}

