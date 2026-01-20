//HRL single header
#include <hrl/hrl.h>

//Window
#include <iosfwd>
#include <glfw/glfw3.h>

//Files
#include <fstream>

//Print
#include <iostream>


//frame time
double dt;
double currentTime;
double lastTime;
void CalculateDeltaTime()
{
  currentTime = glfwGetTime();

  // delta time en secondes
  dt = currentTime - lastTime;

  // mettre à jour lastTime pour la prochaine frame
  lastTime = currentTime;
}


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


//variables de déplacement de la camera
float cameraSpeed = 5.0f;   // unités par seconde
float mouseSensitivity = 0.1f;

float yaw = 0.0f;    // rotation autour de Y
float pitch = 0.0f;  // rotation autour de X

float camX = 0.0f;
float camY = 0.0f;
float camZ = 5.0f;

double lastMouseX = 0.0;
double lastMouseY = 0.0;
bool firstMouse = true;

void ProcessCameraMovement(GLFWwindow* win)
{
  auto deltaTime = (float)dt;
  float velocity = cameraSpeed * deltaTime;

  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
    camZ -= velocity;
  if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
    camZ += velocity;
  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    camX -= velocity;
  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    camX += velocity;
}

void ProcessCameraRotation(GLFWwindow* win)
{
  double mouseX, mouseY;
  glfwGetCursorPos(win, &mouseX, &mouseY);

  if (firstMouse)
  {
    lastMouseX = mouseX;
    lastMouseY = mouseY;
    firstMouse = false;
  }

  float offsetX = (float)(mouseX - lastMouseX) * mouseSensitivity;
  float offsetY = (float)(lastMouseY - mouseY) * mouseSensitivity; // Y inversé

  lastMouseX = mouseX;
  lastMouseY = mouseY;

  yaw   += offsetX;
  pitch += offsetY;

  // clamp pitch pour éviter de regarder trop haut/bas
  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;
}

int main()
{
  //on init HRL avec l'api cible
  HRL_Init(HRL_OpenGL33);

  //GLFW WINDOW//

  //(la gestion de glfw est mauvaise : il faudrait ajouter des logs en cas de crash, mais la ca ne nous interesse pas)
  //on init glfw
  glfwInit();

  //on crée la fenetre
  GLFWwindow* win = glfwCreateWindow(1280, 720, "HRL Example" , nullptr, nullptr);

  //important! : le contexte doit etre actif avant HRL_InitContext
  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

  //cacher le curseur
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  //Verouiller la souris au centre
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //désactiver la v-sync
  glfwSwapInterval(0);


  //on appelle initcontext avec le loader glfw (qui renvoie l'adresse opaque de la fenetre en gros)
  HRL_InitContext(1280, 720, (void*)glfwGetProcAddress);
  HRL_CheckErrors();

  //on ouvre la texture
  size_t texSize;
  std::string texString = OpenFile("canada.jpg", &texSize);
  //crée la texture et le material
  HRL_id tex = HRL_CreateTexture(HRL_Tex_Albedo, texString.c_str(), texSize);
  //material
  HRL_id mat = HRL_CreateMaterial(HRL_SpriteShader);
  HRL_MaterialSetTexture(mat, "Albedo", tex);

  //mesh 1 : canada flag
  HRL_id sprite = HRL_CreateMesh(HRL_Sprite);
  HRL_SetMeshMaterial(sprite, mat);
  HRL_SetMeshScale(sprite, 100, 100, 100);

  //Mesh 2 : portugal flag
  float ptZRot = 0.f; //pour faire tourner le sprite

  size_t ptSize;
  std::string ptString = OpenFile("portugal.jpg", &ptSize);
  HRL_id ptTex = HRL_CreateTexture(HRL_Tex_Albedo, ptString.c_str(), ptSize);
  HRL_id ptMat = HRL_CreateMaterial(HRL_SpriteShader);
  HRL_MaterialSetTexture(ptMat, "Albedo", ptTex);
  HRL_id sprite2 = HRL_CreateMesh(HRL_Sprite);
  HRL_SetMeshMaterial(sprite2, ptMat);
  HRL_SetMeshScale(sprite2, 80, 80, 80);


  //viewport 0 (default) (uses by default camera id 0)
  HRL_SetViewportRect(0,0.f,0.f,1.f, 0.5f);

  //camera 0 (default)
  HRL_SetCameraType(0, HRL_Perspective);
  HRL_SetCameraPerspectiveFov(0, 40.f);
  HRL_SetCameraFarPlane(0, 10000.f);

  //other camera and viewport
  HRL_id cam1 = HRL_CreateCamera(HRL_Perspective);
  HRL_SetCameraPerspectiveFov(cam1, 130.f);
  HRL_id viewport = HRL_CreateViewport(cam1, 0.f, 0.5f, 1.f, 0.5f);




  //laisser
  HRL_CheckErrors();


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

    //calculer le deltatime
    CalculateDeltaTime();

    //printf("FPS : %f\n", 1/dt);

    //movement de la camera
    ProcessCameraMovement(win);
    ProcessCameraRotation(win);
    HRL_SetCameraPosition(0, camX, camY, camZ);
    HRL_SetCameraRotation(0, 0.f, pitch, yaw);

    HRL_SetCameraPosition(cam1, camX, camY, camZ);
    HRL_SetCameraRotation(cam1, 0.f, pitch, yaw);

    ptZRot += 1.f * (float)dt;
    HRL_SetMeshRotation(sprite2, 0.f, 0.f, ptZRot);

    //debug keys
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(win, true);
    if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  //on libere les ressources HRL
  HRL_Shutdown();
}