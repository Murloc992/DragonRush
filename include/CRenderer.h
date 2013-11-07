#ifndef CRENDERER_H
#define CRENDERER_H

class CCamera;
class CRenderer
{
    public:
        CRenderer(glm::vec2 res);
        virtual ~CRenderer();

        GLFWwindow* getWindow()
        {
            return window;
        }

        glm::mat4 getPerspective()
        {
            return P;
        }
    protected:
    private:
        //the app's window
        GLFWwindow* window;
        glm::vec2 resolution;

        CCamera* activeCam;
        std::vector<CCamera*> cameras;
        glm::mat4 P;
};

#endif // CRENDERER_H
