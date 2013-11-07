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
        shader=new CShader("shaders/phong.glsl");
    }

    void generate()
    {
        SVertex v0=SVertex().setPos(-1,0,1).setUV1(1,1);
        SVertex v1=SVertex().setPos(1,0,1).setUV1(0,1);
        SVertex v2=SVertex().setPos(1,0,-1).setUV1(0,0);
        SVertex v3=SVertex().setPos(-1,0,-1).setUV1(1,0);
        vertices.resize(4);
        vertices={v0,v1,v2,v3};

        indices.resize(6);
        indices={0,1,2,2,3,0};

        calculateNormals();
        createBuffers();
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
