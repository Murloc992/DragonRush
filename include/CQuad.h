#ifndef CQUAD_H_INCLUDED
#define CQUAD_H_INCLUDED

#include "CMesh.h"

class CQuad:public CMesh
{
private:
public:
    CShader *shader;

    CQuad()
    {
        CMesh();
        shader=new CShader("shaders/default.glsl");
    }

    void generate()
    {
        SVertex v0=SVertex().setPos(1,0,1).setUV1(1,1);
        SVertex v1=SVertex().setPos(-1,0,1).setUV1(0,1);
        SVertex v2=SVertex().setPos(-1,0,-1).setUV1(0,0);
        SVertex v3=SVertex().setPos(1,0,-1).setUV1(1,0);
        vertices.resize(4);
        vertices={v0,v1,v2,v3};

        indices.resize(6);
        indices={0,1,2,2,3,0};

        calculateNormals();

        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(SVertex),&vertices[0],GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        //vec3 pos
        glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(SVertex),nullptr);
        //vec3 norm
        glVertexAttribPointer(1,3,GL_FLOAT,false,sizeof(SVertex),(void*)12);
        //vec2 uv
        glVertexAttribPointer(2,2,GL_FLOAT,false,sizeof(SVertex),(void*)24);
        //vec4 col
        glVertexAttribPointer(3,4,GL_FLOAT,false,sizeof(SVertex),(void*)40);

        glGenBuffers(1,&IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(uint32_t),&indices[0],GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void draw()
    {
        shader->use();

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,nullptr);
        glBindVertexArray(0);
    }
protected:
};

#endif // CQUAD_H_INCLUDED
