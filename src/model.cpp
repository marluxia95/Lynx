#include <stdio.h>
#include <vector>
#include <string.h>

#include <glm/glm.hpp>

#include "vertexBuffer.h"
#include "model.h"

using namespace std;

namespace Lynx {

vector<Vertex>* loadModelFromFile(const char* path) {
    vector<Vertex>* finalArray = new vector<Vertex>();

    vector<glm::vec3> vertices;
    vector<glm::vec2> uv_pos;
    vector<glm::vec3> n_pos;

    vector<GLuint> v_indices;
    vector<GLuint> uv_indices;
    vector<GLuint> n_indices;

    FILE* file = fopen(path, "r");
    printf("Loading model %s\n", path);
    if(file == NULL){
        printf("Unable to open model file.\n");
        return nullptr;
    }
    bool vertexPart = false, uvPart = false, normalPart = false;
    
    int count;
    while ( 1 ) {
        char header[128];
        count++;

        int res = fscanf(file, "%s", header);
        if(res == EOF)
            break;

        if(strcmp(header, "v") == 0) {
            if(!vertexPart){vertexPart = true; count = 0;} 
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }else if(strcmp(header, "vt") == 0){
            if(!uvPart){uvPart = true; count = 0;}
            glm::vec2 tex_vertex;
            fscanf(file, "%f %f\n", &tex_vertex.x, &tex_vertex.y);
            uv_pos.push_back(tex_vertex);
        }else if(strcmp(header, "vn") == 0) {
            if(!normalPart){normalPart = true; count = 0;}
            glm::vec3 normal_vertex;
            fscanf(file, "%f %f %f\n", &normal_vertex.x, &normal_vertex.y, &normal_vertex.z);
            n_pos.push_back(normal_vertex);
        }else if(strcmp(header, "f") == 0){
            unsigned int v1,t1,n1,v2,t2,n2,v3,t3,n3;
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n3, &v3, &t3, &n3);
            if(matches < 9){
                printf("Invalid format \n");
                return nullptr;
            }
            // Set an offset of 1 because the OBJ format's indices starts at 1 not 0
            v_indices.push_back(v1-1);
            v_indices.push_back(v2-1);
            v_indices.push_back(v3-1);

            uv_indices.push_back(t1-1);
            uv_indices.push_back(t2-1);
            uv_indices.push_back(t3-1);

            n_indices.push_back(n1-1);
            n_indices.push_back(n2-1);
            n_indices.push_back(n3-1);
        }

        
    }

    // Convert from indices to arrays ( sorry performance :( )
    int v_size = vertices.size();
    int vi_size = v_indices.size();
    int uv_size = uv_pos.size();
    int uvi_size = uv_indices.size();
    int n_size = n_pos.size();
    int ni_size = n_indices.size();
    printf("vertices size : %d \n", v_size);
    printf("vert indices size : %d \n", vi_size);
    printf("uv's size : %d \n", uv_size);
    printf("uv indices size : %d \n", uvi_size);
    printf("normals size : %d \n",n_size);
    printf("normal indices size : %d \n", ni_size);

    for ( int v=0; v<vi_size; v++){
        printf("%d ", v_indices[v]+1);
        Vertex v1;
        v1.Position = vertices[v_indices[v]];
        v1.TextureCoords = uv_pos[uv_indices[v]];
        v1.Normal = n_pos[n_indices[v]];
        finalArray->push_back(v1);
    }

    printf("Finished converting model. \n");
    return finalArray;
}


}