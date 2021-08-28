#include <stdio.h>
#include "buffers.h"
#include "vertexArray.h"
#include "terrain.h"
#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

    Terrain::Terrain(float** heightmap) : Mesh()
    {
        m_heightmap = heightmap;

        VAO = VertexArray::Create();
        VAO->Bind();

        m_rows = sizeof(heightmap)/sizeof(float);
        m_columns = sizeof(heightmap[0])/sizeof(float);
        m_vertcount = m_rows * m_columns;
        VBO = VertexBuffer::Create();
        SetupMesh();
    }

    glm::vec3 Terrain::calculate_normal(unsigned int x, unsigned int z)
    {
        x = x == 0 ? 1 : x;
        z = z == 0 ? 1 : z;

        float height_l = m_heightmap[x-1][z];
        float height_r = m_heightmap[x+1][z];
        float height_u = m_heightmap[x][z+1];
        float height_d = m_heightmap[x][z-1];

        return glm::normalize(glm::vec3(height_l - height_r,1.0f, height_d - height_u));
    }

    int Terrain::getpos(unsigned int x, unsigned int z)
    {
        return x + z * m_columns; 
    }

    void Terrain::SetupMesh()
    {
        // Process vertices
        vertices = (Vertex*)malloc(m_vertcount * sizeof(Vertex));


        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices[getpos(r,c)].Position = glm::vec3(-1.0f + float(r) / float(m_rows-1), m_heightmap[r][c], -1.0f + float(c) / float(m_columns-1));
            }
            
        }

        // Process UV's
        const float tex_stepU = 0.1f;
        const float tex_stepV = 0.1f;

        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices[getpos(r,c)].TextureCoords = glm::vec2(tex_stepU * r, tex_stepV * c);
            }
        }

        // Process normals
        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices[getpos(r,c)].Normal = calculate_normal(r,c);
            }
        }
        
        VBO->SetData(vertices, m_rows * m_columns * sizeof(Vertex));

        // Process indices

        int index_count = (m_rows-1)*m_columns*2 + m_rows-1;
        int index_size = index_count * sizeof(unsigned int);
        indices = (unsigned int*)malloc(index_count*sizeof(unsigned int));

        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                for (int n = 0; n < 2; n++){
                    int index = (r+n)*m_columns+c;
                    indices[r + c * m_columns + n *2] = index;
                }
            }
        }

        EBO = ElementBuffer::Create(indices, index_count*sizeof(unsigned int));
    }

    void Terrain::Render()
    {
        EBO->Bind();
        RendererAPI::DrawIndexed(index_size);
    }

    float** load_heightmap_from_image(const char* path)
    {
        TextureData* texture = loadTexture(path);
        float** heightmap = (float**)malloc((texture->height * texture->width) * sizeof(float));

        unsigned char* pixel_ptr = &texture->data[0];
        for ( int x = 0; x < texture->height; x++ ) {
            for ( int z = 0; z < texture->width; z++ ) {
                log_debug("T %d %d", x,z);
                heightmap[x][z] = float(*pixel_ptr) / 255.0f;
                pixel_ptr += texture->channels;
            }
        }
        
        return heightmap;
    }
}
