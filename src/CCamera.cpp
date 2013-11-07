#include "CCamera.h"
#include "CRenderer.h"
#include "util.h"

CCamera::CCamera(CRenderer *renderer,const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up,bool FPS)
{
    this->renderer=renderer;
    this->position=pos;
    this->target=target;
    this->up=up;
    this->isFPS=FPS;


    if(isFPS)
    {
        glfwSetInputMode(renderer->getWindow(),GLFW_CURSOR,GLFW_CURSOR_HIDDEN);

        glm::vec3 d=target-pos;
        glm::vec3 t=cartesianToSpherical(d);

        verticalAngle=t.y;
        horizontalAngle=t.z;
    }
    else
    {
        verticalAngle=0.0f;
        horizontalAngle=glm::pi<float>();
    }

    speed=3.0;
    mouseSpeed=1.0;
}

void CCamera::update(double elapsed)
{
    if(isFPS)
    {
        double xpos,ypos;

        glfwGetCursorPos(renderer->getWindow(),&xpos,&ypos);
        glfwSetCursorPos(renderer->getWindow(),1280.0/2.0,720.0/2.0);

        horizontalAngle+=mouseSpeed * elapsed * double(1280.0/2.0 - xpos);
        verticalAngle+=mouseSpeed * elapsed * double(720.f/2.0 - ypos);
        glm::vec3 direction(glm::cos(verticalAngle)*glm::sin(horizontalAngle),
                            glm::sin(verticalAngle),
                            glm::cos(verticalAngle)*glm::cos(horizontalAngle));

        glm::vec3 right(glm::sin(horizontalAngle - 3.14f/2.0f),
                        0,
                        glm::cos(horizontalAngle - 3.14f/2.0f));

        target=position+direction;
        up = glm::cross(right,direction);

        int32_t kp=0;
        if(glfwGetKey(renderer->getWindow(),GLFW_KEY_W))
        {
            kp++;
            position+=direction*(float)(elapsed * speed/kp);
        }
        if(glfwGetKey(renderer->getWindow(),GLFW_KEY_S))
        {
            kp++;
            position-=direction*(float)(elapsed * speed/kp);
        }
        if(glfwGetKey(renderer->getWindow(),GLFW_KEY_A))
        {
            kp++;
            position-=right*(float)(elapsed * speed/kp);
        }
        if(glfwGetKey(renderer->getWindow(),GLFW_KEY_D))
        {
            kp++;
            position+=right*(float)(elapsed * speed/kp);
        }

        V=glm::lookAt(position,target,up);
    }
    else
    {
        V=glm::lookAt(position,target,up);
    }
}
