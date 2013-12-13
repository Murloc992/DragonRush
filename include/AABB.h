#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

class AABB:public CMesh
{
private:
    float minx,maxx;
    float miny,maxy;
    float minz,maxz;
public:
    AABB(std::vector<SVertex> vertices)
    {
        recalculate(vertices);
    };
    ~AABB()
    {

    };

    void recalculate(std::vector<SVertex> vertices)
    {
        minx=maxx=0.0f;
        miny=maxy=0.0f;
        minz=maxz=0.0f;

        for(uint32_t i=0; i<vertices.size(); i++)
        {
            glm::vec3 p=vertices[i].pos;
            if(minx>p.x) minx=p.x;
            if(miny>p.y) miny=p.y;
            if(minz>p.z) minz=p.z;

            if(maxx<p.x) maxx=p.x;
            if(maxy<p.y) maxy=p.y;
            if(maxz<p.z) maxz=p.z;
        }

        printf("Bounding box info: mX:%f MX:%f mY:%f MY:%f mZ:%f MZ:%f\n",minx,maxx,miny,maxy,minz,maxz);
    }

    void generate()
    {
        vertices.resize(8);

        vertices[0]=SVertex().setPos(minx,miny,minz);
        vertices[1]=SVertex().setPos(minx+maxx,miny,minz);
        vertices[2]=SVertex().setPos(minx,miny,minz+maxz);
        vertices[3]=SVertex().setPos(minx+maxx,miny,minz+maxz);

        vertices[4]=SVertex().setPos(minx,miny+maxy,minz);
        vertices[5]=SVertex().setPos(minx+maxx,miny+maxy,minz);
        vertices[6]=SVertex().setPos(minx,miny+maxy,minz+maxz);
        vertices[7]=SVertex().setPos(minx+maxx,miny+maxy,minz+maxz);
    }

    void draw(uint32_t shader)
    {
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glDrawElements(GL_LINES,indices.size(),GL_UNSIGNED_INT,nullptr);
        glBindVertexArray(0);
        glUseProgram(0);
    }
protected:
};

#endif // AABB_H_INCLUDED
