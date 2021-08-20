#ifndef READOBJ_H
#define READOBJ_H
#include<vector>
#include<array>

namespace pipeline3D {

    struct Vertex {
        float x;
        float y;
        float z;
        float nx;
        float ny;
        float nz;
        float u;
        float v;
    };

    inline Vertex interpolate(const Vertex& v1, const Vertex& v2, float w) {
            const float w2 = (1.0f-w);
            Vertex v;
            v.x = (w*v1.x + w2*v2.x);
            v.y = (w*v1.y + w2*v2.y);
            v.z = (w*v1.z + w2*v2.z);
            v.nx = (w*v1.nx + w2*v2.nx);
            v.ny = (w*v1.ny + w2*v2.ny);
            v.nz = (w*v1.nz + w2*v2.nz);
            v.u = (w*v1.u + w2*v2.u);
            v.v = (w*v1.v + w2*v2.v);

            return v;
    }

    inline void perspective_correct(Vertex& v) {
            v.z = 1.0f/v.z;
            v.x *= v.z;
            v.y *= v.z;
            v.nx *= v.z;
            v.ny *= v.z;
            v.nz *= v.z;
            v.u *= v.z;
            v.v *= v.z;
    }

    inline void transform(const std::array<float,16> &M, Vertex& v) {
        const float x = v.x*M[4*0+0] + v.y*M[4*0+1] + v.z*M[4*0+2] + M[4*0+3];
        const float y = v.x*M[4*1+0] + v.y*M[4*1+1] + v.z*M[4*1+2] + M[4*1+3];
        const float z = v.x*M[4*2+0] + v.y*M[4*2+1] + v.z*M[4*2+2] + M[4*2+3];
        const float w = v.x*M[4*3+0] + v.y*M[4*3+1] + v.z*M[4*3+2] + M[4*3+3];
        
        v.x = x/w;
        v.y = y/w;
        v.z = z/w;

        //assume roto-translation, should do proper contravariant transformation
        const float nx = v.nx*M[4*0+0] + v.ny*M[4*0+1] + v.nz*M[4*0+2];
        const float ny = v.nx*M[4*1+0] + v.ny*M[4*1+1] + v.nz*M[4*1+2];
        const float nz = v.nx*M[4*2+0] + v.ny*M[4*2+1] + v.nz*M[4*2+2];
        v.nx = nx;
        v.ny = ny;
        v.nz = nz;
}

    std::vector<std::array<Vertex,3>> read_obj(const char* file);
};

#endif // READOBJ_H
