#include "gl33_texture.h"

/** Librarie donnant acces aux fonctions OpenGL */
#include <glad/glad.h>

/** Librarie permettant de décoder les images png, jpg, jpeg, ... */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <string>
#include <unordered_map>

extern void SetErrorCode(const std::string& e);

static std::unordered_map<HRL_uint, GLuint> glSlots = {
{HRL_Tex_Albedo, GL_TEXTURE0},
{HRL_Tex_Normal, GL_TEXTURE1},
{HRL_Tex_Specular, GL_TEXTURE2},
{HRL_Tex_Roughness, GL_TEXTURE3},
{HRL_Tex_Metalic, GL_TEXTURE4},
{HRL_Tex_Alpha, GL_TEXTURE5},
{HRL_Tex_ShadowMap, GL_TEXTURE6},
{HRL_Tex_CubeMap, GL_TEXTURE7},
{HRL_Tex_Custom0, GL_TEXTURE8},
{HRL_Tex_Custom1, GL_TEXTURE9},
{HRL_Tex_Custom2, GL_TEXTURE10},
{HRL_Tex_Custom3, GL_TEXTURE11},
{HRL_Tex_Custom4, GL_TEXTURE12},
{HRL_Tex_Custom5, GL_TEXTURE13},
{HRL_Tex_Custom6, GL_TEXTURE14},
};

GL33_Texture::GL33_Texture(const char *imageContent, const size_t imageSize, const HRL_uint type) : type_(type)
{
  //on crée la texture OpenGL
  glGenTextures(1, &glID_);
  glBindTexture(GL_TEXTURE_2D, glID_);

  //configuration des parametres de la texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //filtrage trilinéaire
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);

  // Convertir std::vector<char>::data() en unsigned char* et sa taille en int
  unsigned char *data = stbi_load_from_memory(
      (stbi_uc const*)imageContent,        //Pointeur vers les données
      imageSize,            //Taille totale du tampon
      &width_,
      &height_,
      &nr_channels_,
      STBI_rgb_alpha
  );

  if (data)
  {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    //on libere la mémoire allouée par stb
    stbi_image_free(data);
  }
  else
  {
    SetErrorCode("Texture failed to load");

    //on libere la memoire allouée par opengl, stb et la texture elle meme
    glDeleteTextures(1, &glID_);

    //on libere la mémoire allouée par stb (normalement c'est pas nécéssaire mais on le fait quand meme)
    stbi_image_free(data);
  }
}

GL33_Texture::~GL33_Texture()
{
  glDeleteTextures(1, &glID_);
}

void GL33_Texture::Bind()
{
  //on bind le bon slot opengl
  glActiveTexture(glSlots[type_]);
  glBindTexture(GL_TEXTURE_2D, glID_);
}

HRL_uint GL33_Texture::GetWidth() const
{
  return width_;
}

HRL_uint GL33_Texture::GetHeight() const
{
  return height_;
}

void GL33_Texture::Resize(HRL_uint width, HRL_uint height)
{
  //fonction pas encore implémentée
}