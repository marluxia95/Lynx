#include <stdio.h>
#include <vector>
#include <string.h>

#include <glm/glm.hpp>

#include "vertexBuffer.h"
#include "model.h"

namespace Lynx {

Model loadModelFromFile(const char* path) {
    Model model;

    vector<glm::vec3> vertices_temp;
    vector<glm::vec3> uv_temp;
    vector<glm::vec3> normal_temp;

    vector<GLuint> tex_indices;
    vector<GLuint> normal_indices;
    


    FILE* file = fopen(path, "r");
    printf("Loading model %s\n", path);
    if(file == NULL){
        printf("Unable to open model file.\n");
        return model;
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
            vertices_temp.push_back(vertex);
            
        }else if(strcmp(header, "vt") == 0){
            if(!uvPart){uvPart = true; count = 0;}
            glm::vec3 tex_vertex;
            fscanf(file, "%f %f %f\n", &tex_vertex.x, &tex_vertex.y, &tex_vertex.z);
            uv_temp.push_back(tex_vertex);
        }else if(strcmp(header, "vn") == 0) {
            if(!normalPart){normalPart = true; count = 0;}
            glm::vec3 normal_vertex;
            fscanf(file, "%f %f %f\n", &normal_vertex.x, &normal_vertex.y, &normal_vertex.z);
            normal_temp.push_back(normal_vertex);
        }else if(strcmp(header, "f") == 0){
            unsigned int v1,t1,n1,v2,t2,n2,v3,t3,n3;
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n3, &v3, &t3, &n3);
            if(matches < 9){
                printf("Invalid format \n");
                return model;
            }
            // Set an offset of 1 because the OBJ format's indices starts at 1 not 0
            model.indices.push_back(v1-1);
            model.indices.push_back(v2-1);
            model.indices.push_back(v3-1);

            tex_indices.push_back(t1-1);
            tex_indices.push_back(t2-1);
            tex_indices.push_back(t3-1);

            normal_indices.push_back(n1-1);
            normal_indices.push_back(n2-1);
            normal_indices.push_back(n3-1);
        }

        
    }

    
    printf("Converting vertices\n");
    for(int v=0; v<vertices_temp.size(); v++) { 
        Vertex vertex;
        vertex.Position = vertices_temp[v];
        model.vertices.push_back(vertex);
    }

    printf("Converting UV Coords ...\n");
    for( int uv=0; uv<tex_indices.size(); uv++) {
        //model.vertices[uv].TextureCoords = uv_temp[tex_indices[uv]];
    }

    printf("Converting Normal Coords ...\n");
    /*for( int n=0; n<normal_indices.size(); n++) {
        if(normal_indices.size()>model.vertices.size()){printf("bruh");}
        //model.vertices[n].Normal = normal_temp[normal_indices[n]];
    }*/

    printf("Finished converting model.\n");
    return model;
}


}