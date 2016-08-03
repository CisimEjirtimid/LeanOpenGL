#include "GLManager.h"

using namespace std;
using namespace gl;

GLManager::GLManager(GLRenderer& renderer)
    : _renderer(renderer)
{
}

GLManager::~GLManager()
{
}

void GLManager::createGrid(int width, int height, float step_x, float step_y)
{
    for (int i = 0; i <= width - 1; i++)
    {
        for (int j = 0; j <= height - 1; j++)
        {
            patch_vector.push_back(i*step_x);
            patch_vector.push_back(j*step_y);

            patch_vector.push_back(i*step_x);
            patch_vector.push_back(j*step_y + step_y);

            patch_vector.push_back(i*step_x + step_x);
            patch_vector.push_back(j*step_y);

            patch_vector.push_back(i*step_x + step_x);
            patch_vector.push_back(j*step_y + step_y);
        }
    }

    _renderer.addObject(patch_vector.size() * sizeof(float), patch_vector.data(), GL_PATCHES, 2);
}
