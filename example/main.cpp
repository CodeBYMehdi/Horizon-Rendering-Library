//HRL single header
#include <hrl/hrl.h>

//Window
#include <iosfwd>
#include <glfw/glfw3.h>

//Files
#include <fstream>

//Print
#include <iostream>

//GLFW window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  HRL_WindowResizeCallback(width, height);
}


std::string OpenFile(const char* _path, size_t* _size)
{
  FILE* f = fopen(_path, "rb");
  if (!f)
  {
    std::cout << "Erreur de lecture du fichier" << std::endl;
    return "";
  }

#ifdef WIN32
  //windows
  _fseeki64(f, 0, SEEK_END);
  __int64 size = _ftelli64(f);
#else
  //compaptibilité
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
#endif

  if (size <= 0 || size > INT_MAX)
  {
    fclose(f);
    std::cout << "Erreur de taille (<= 0 ou superieur a INT_MAX)" << std::endl;
    return "";
  }
#ifdef WIN32
  _fseeki64(f, 0, SEEK_SET);
#else
  fseek(f, 0, SEEK_SET);
#endif

  std::string content((size_t)size, '\0');   // alloue buffer
  if (fread(&content[0], 1, (size_t)size, f) != (size_t)size)
  {
    fclose(f);
    std::cout << "Erreur de lecture complète" << std::endl;
    return "";
  }

  fclose(f);
  //passer la size
  if (_size)
  {
    *_size = (size_t)size;
  }
  return content;
}

int main()
{
  //on init HRL avec l'api cible
  HRL_Init(HRL_OpenGL33);

  //(la gestion de glfw est mauvaise : il faudrait ajouter des logs en cas de crash, mais la ca ne nous interesse pas)
  //on init glfw
  glfwInit();

  //on crée la fenetre
  GLFWwindow* win = glfwCreateWindow(1280, 720, "HRL Example" , NULL, NULL);

  //important! : le contexte doit etre actif avant HRL_InitContext
  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

  //on appelle initcontext avec le loader glfw (qui renvoie l'adresse opaque de la fenetre en gros)
  HRL_InitContext(1280, 720, (void*)glfwGetProcAddress);

  //on ouvre la texture
  size_t texSize;
  const char* texContent = OpenFile("canada.jpg", &texSize).c_str();

  //crée la texture et le material
  HRL_id tex = HRL_CreateTexture(HRL_Tex_Albedo, texContent, texSize);
  HRL_id mat = HRL_CreateMaterial(HRL_SpriteShader);
  HRL_MaterialSetTexture(mat, "Albedo", tex);

  //boucle principale
  while (!glfwWindowShouldClose(win))
  {
    //efface la frame précedente
    HRL_BeginFrame();
    //dessine les objets à l'ecran
    HRL_EndFrame();

    //update classique glfw
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  //on libere les ressources HRL
  HRL_Shutdown();
}