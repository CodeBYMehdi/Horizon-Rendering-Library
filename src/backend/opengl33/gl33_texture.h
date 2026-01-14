#ifndef GL33_TEXTURE
#define GL33_TEXTURE

#include "../../hrl.h"
#include <glad/glad.h>

//a l'avenir, donner plus de possibilités aux textures, changer l'encodage couleur, la mode de filtrage, etc...

class GL33_Texture final {
public:
  GL33_Texture()=default;

  int GL33_Create(const HRL_uint _type, const char* _imageContent, const size_t _imageSize);

  ~GL33_Texture();

  void Bind();

  HRL_uint GetWidth() const;
  HRL_uint GetHeight() const;

  void Resize(HRL_uint width, HRL_uint height);

private:
  GLuint glID_;
  GLint width_, height_, nr_channels_;

  HRL_uint type_;
};

#endif