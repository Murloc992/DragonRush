#ifndef CCUBE_H
#define CCUBE_H

#include <vector>
#include <glm.hpp>

class CCube
{
    public:
        CCube();
        virtual ~CCube();
        std::vector<glm::vec3> Getvbo() { return vbo; }
        void Setvbo(std::vector<glm::vec3> val) { vbo = val; }
    protected:
    private:
        std::vector<glm::vec3> vbo;
};

#endif // CCUBE_H
