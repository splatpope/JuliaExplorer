#include "mdl.h"

void Model::FillBuffers()
{
    //Consolidate the VBO and EBO with the data from every mesh, then fill them

    std::vector<Vertex> _vertices;
    std::vector<uint> _indices;

    int n = meshes.size();
    int sv = 0, si = 0;

    //get total size of all mesh vertices and indices
    for (int i = 0; i < n; i++)
    {
        sv += meshes[i].vertices.size();
        si += meshes[i].indices.size();
    }

    //prepare destination vectors and fill em
    _vertices.reserve(sv);
    _indices.reserve(si);

    for (int i = 0; i < n; i++)
    {
        _vertices.insert(_vertices.end(), meshes[i].vertices.begin(), meshes[i].vertices.end());
        _indices.insert(_indices.end(), meshes[i].indices.begin(), meshes[i].indices.end());
    }

    //Then bind VBO and add vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);
    //Then bind EBO and add indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint), _indices.data(), GL_STATIC_DRAW); 
    n_indices = _indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



//todo :  make primitive classes to generate indices and unpack data to vbo and ebo