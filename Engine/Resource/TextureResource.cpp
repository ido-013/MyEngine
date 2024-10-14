#include "TextureResource.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureResource::~TextureResource()
{
    if (data != nullptr)
    {
        glDeleteTextures(1, static_cast<GLuint*>(data));
        delete data;
    }

    data = nullptr;
}

void TextureResource::LoadData(const std::string& filename)
{
    GLint width, height, bytes_per_texel;
    stbi_set_flip_vertically_on_load(1);

    GLubyte* ptr_texels = stbi_load(("Assets/" + filename).c_str(), &width, &height, &bytes_per_texel, 4);
    GLuint* texobj = new GLuint;

    glCreateTextures(GL_TEXTURE_2D, 1, texobj);

    glTextureStorage2D(*texobj, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(*texobj, 0, 0, 0, width, height,
        GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);

    glTextureSubImage2D(*texobj, 0, 0, 0, 1, 1,
        GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);

    stbi_image_free(ptr_texels);

    data = texobj;
}