#include <stdio.h>
#include "buffers.h"
#include "vertexArray.h"
#include "terrain.h"
#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"
#include "lynx.h"

namespace Lynx::Graphics {

    Terrain::Terrain(std::vector<float> heightmap) : Mesh()
    {
        log_debug("Creating terrain");
        m_heightmap = heightmap;

        VAO = VertexArray::Create();
        VAO->Bind();

        m_rows = sizeof(heightmap)/sizeof(float);
        m_columns = sizeof(heightmap[0])/sizeof(float);
        m_vertcount = m_rows * m_columns;
        VBO = VertexBuffer::Create();
        log_debug("Creating terrain mesh");
        SetupMesh();
    }

    Terrain::Terrain(const char* image) : Mesh()
    {
        log_debug("Creating terrain");
        load_heightmap_from_image(image);

        VAO = VertexArray::Create();
        VAO->Bind();

        m_rows = sizeof(m_heightmap)/sizeof(float);
        m_columns = sizeof(m_heightmap[0])/sizeof(float);
        m_vertcount = m_rows * m_columns;
        VBO = VertexBuffer::Create();
        log_debug("Creating terrain mesh");
        SetupMesh();
    }

    Terrain::~Terrain()
    {

    }

    glm::vec3 Terrain::calculate_normal(unsigned int x, unsigned int z)
    {
        x = x == 0 ? 1 : x;
        z = z == 0 ? 1 : z;

        float height_l = m_heightmap[x-1 + z     * gridlen];
        float height_r = m_heightmap[x+1 + z     * gridlen];
        float height_u = m_heightmap[x   + (z+1) * gridlen];
        float height_d = m_heightmap[x   + (z-1) * gridlen];

        return glm::normalize(glm::vec3(height_l - height_r,1.0f, height_d - height_u));
    }

    int Terrain::getpos(unsigned int x, unsigned int z)
    {
        return x + z * m_columns; 
    }

    void Terrain::SetupMesh()
    {
        // Process vertices
        vertices = new std::vector<Vertex>();
        vertices->resize(m_rows * m_columns);

        log_debug("Processing vertices ");

        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices->at(getpos(r,c)).Position = glm::vec3(-1.0f + float(r) / float(m_rows-1), m_heightmap[r + c * gridlen], -1.0f + float(c) / float(m_columns-1));
            }
            
        }

        log_debug("Processing UV's ");

        // Process UV's
        const float tex_stepU = 0.1f;
        const float tex_stepV = 0.1f;

        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices->at(getpos(r,c)).TextureCoords = glm::vec2(tex_stepU * r, tex_stepV * c);
            }
        }

        log_debug("Processing normals ");

        // Process normals
        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                vertices->at(getpos(r,c)).Normal = calculate_normal(r,c);
            }
        }
        
        VBO->SetData(vertices, m_rows * m_columns * sizeof(Vertex));

        // Process indices

        log_debug("Processing indices ");

        int index_count = (m_rows-1)*m_columns*2 + m_rows-1;
        int index_size = index_count * sizeof(unsigned int);

        indices = new std::vector<unsigned int>;
        log_debug("indice array size %d", index_count);
        indices->resize(index_count);

        for ( int r = 0; r < m_rows; r++) {
            for (int c = 0; c < m_columns; c++) {
                for (int n = 0; n < 2; n++){
                    unsigned int index = (r+n)*m_columns+c;
                    indices->at(r + c * m_columns + n *2) = index;
                    log_debug("%d", r + c * m_columns + n *2);
                }
            }
        }

        log_debug("Creating EBO ");

        EBO = ElementBuffer::Create(indices, index_count*sizeof(unsigned int));
    }

    void Terrain::load_heightmap_from_image(const char* path)
    {
        Texture* texture = new Texture(path);
        TextureData* tdata = texture->GetData();
        
        LYNX_ASSERT(tdata->GetData(), "Invalid terrain data");

        gridlen = tdata->GetWidth();

        m_heightmap.resize(tdata->GetWidth() * tdata->GetHeight());

        m_heightmap[0] = 1.0f;        

        log_debug("Loading terrain with heightmap image %s ( %d:%d )", path, tdata->GetWidth(), tdata->GetHeight());

        unsigned char* data = tdata->GetData();
        for ( int x = 0; x < tdata->GetHeight(); x++ ) {
            for ( int z = 0; z < tdata->GetWidth(); z++ ) {
                m_heightmap[x + z * gridlen] = data[x + z] / 255.0f;
            }
        }

        delete texture;
    }
}
