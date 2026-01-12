#include <hrl/hrl.h>
#include <glfw/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  HRL_WindowResizeCallback(width, height);
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

  //on crée la texture
  HRL_id tex = HRL_CreateTexture(HRL_Tex_Albedo, , );

  //on crée le material
  HRL_id mat = HRL_CreateMaterial(HRL_SpriteShader);
  HRL_MaterialSetTexture(mat, "Albedo", tex);

  HRL_id sprite = HRL_CreateMesh(HRL_Sprite);
  HRL_SetMeshMaterial(sprite, mat);

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