#ifndef CCAMERA_H
#define CCAMERA_H

class CRenderer;
//the provider of the View Matrix
class CCamera
{
public:
    CCamera(CRenderer *renderer,const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up,bool FPS);
    ~CCamera()
    {
    };

    void update(double elapsed);

    void setFPS(bool b)
    {
        isFPS=b;
    }

    const glm::mat4 getView()
    {
        return V;
    }
protected:
private:
    //the owner renderer (to get cursor pos and all nifty stuff)
    CRenderer *renderer;

    bool isFPS;

    glm::vec3 position,target,up;
    double horizontalAngle,verticalAngle,speed,mouseSpeed;

    glm::mat4 V; //view matrix
};

#endif // CCAMERA_H
