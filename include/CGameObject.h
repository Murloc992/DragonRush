#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H


class CGameObject
{
    public:
        CGameObject();
        virtual ~CGameObject();
    protected:
        glm::mat4 M; //Pos,Rot,Scale

        CMesh *mesh; //The mesh
    private:
};

#endif // CGAMEOBJECT_H
