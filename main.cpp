#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/meshobject.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>

#include <iostream>

using namespace std;
using namespace VART;

class Terrain: public VART::MeshObject {
  public:
    Terrain(){
      
      std::vector<double> vertices;
      int lines = 20;
      int columns = 20;
      
      for(int line = 0; line < lines; line++){
        for(int column = 0; column < columns; column++){
          cout << line << column << endl;
          vertices.push_back(line);
          vertices.push_back(column-10);
          vertices.push_back(0);
        }
      }

      vertCoordVec = vertices;

      for (int i =0; i < lines - 1; i++){
        vector<unsigned int> f1;
        for (int j = 0; j < columns; j++){
          f1.push_back(i * columns + j);
          f1.push_back((i+1) * columns + j);
        }

        Mesh floor;
        floor.type = Mesh::TRIANGLE_STRIP;
        floor.material = Material::PLASTIC_GREEN();
        floor.indexVec = f1;

        meshList.push_back(floor);
      }

      ComputeVertexNormals();
      ComputeBoundingBox();
      ComputeRecursiveBoundingBox();
    }

    double randomHeight(){
      int random = rand();
      return (random % (int)(0.5 * tan(M_PI/6)*101 ) / 100.0);
    }
};

class House : public VART::MeshObject {
  public:
    House(){
      double vertices[] = {
        0,0,0,
        4,0,0,
        4,4,0,
        0,4,0,
        0,0,2,
        4,0,2,
        4,4,2,
        0,4,2,
        2,2,4, // Roof
        4.1,0.7,0.1, // Begin Door
        4.1,1.4,0.1,
        4.1,0.7,1.1,
        4.1,1.4,1.1, // End Door
        4.1,2,0.1, // Begin Window1
        4.1,3.7,0.1,
        4.1,2,1.1,
        4.1,3.7,1.1, // End Window1
        -12,-30,20,//Begin Back panel // 17
        -12,30,20, // 18
        -12,-30,-20, // 19
        -12,30,-20 //End back panel //20
      };
      double* endVertices = vertices + sizeof(vertices)/sizeof(double);

      double baseVertices[] = {0,3,2,1};
      double* endBaseVertices = baseVertices + sizeof(baseVertices)/sizeof(double);

      double walls[] = {
        6,2,3,7,
        5,1,2,6,
        4,7,3,0,
        5,4,0,1
      };
      double* endWalls = walls + sizeof(walls)/sizeof(double);

      double ceiling[] = {4,5,6,7};
      double* endCeiling = ceiling + sizeof(ceiling)/sizeof(double);

      double roof[] = {
        8,6,7,
        8,4,5,
        4,8,7,
        8,5,6
      };
      double* endRoof = roof + sizeof(roof)/sizeof(double);
      cout << "ROOF" << endl;

      double door[] = {12,11,9,10};
      double* endDoor = door + sizeof(door)/sizeof(double);

      double windows[] ={
        16,15,13,14
      };
      double* endWindows = windows + sizeof(windows)/sizeof(double);

      double backPanel[] = {
        18,17,19,20
      };
      double* endBackPanel = backPanel + sizeof(backPanel)/sizeof(double);




      vertCoordVec.assign(vertices,endVertices);
      
      Mesh baseMesh;
      baseMesh.type = Mesh::QUADS;
      baseMesh.indexVec.assign(baseVertices, endBaseVertices);
      baseMesh.material = Material::PLASTIC_GREEN();
      meshList.push_back(baseMesh);

      Mesh wallsMesh;
      wallsMesh.type = Mesh::QUADS;
      wallsMesh.indexVec.assign(walls, endWalls);
      wallsMesh.material = Material::LIGHT_PLASTIC_GRAY();
      meshList.push_back(wallsMesh);


      Mesh CeilingMesh;
      CeilingMesh.type = Mesh::QUADS;
      CeilingMesh.indexVec.assign(ceiling, endCeiling);
      CeilingMesh.material = Material::PLASTIC_RED();
      meshList.push_back(CeilingMesh);

      Mesh roofMesh;
      roofMesh.type = Mesh::TRIANGLES;
      roofMesh.indexVec.assign(roof,endRoof);
      roofMesh.material = Material::PLASTIC_RED();
      meshList.push_back(roofMesh);

      // cout << "BEGIN MESH DOOR" << endl;
      Mesh doorMesh;
      doorMesh.type = Mesh::QUADS;
      doorMesh.indexVec.assign(door, endDoor);
      doorMesh.material = Material::PLASTIC_WHITE();
      meshList.push_back(doorMesh);
      // cout << "END MESH DOOR" << endl;

      Mesh windowsMesh;
      windowsMesh.type = Mesh::QUADS;
      windowsMesh.indexVec.assign(windows,endWindows);
      windowsMesh.material = Material::PLASTIC_WHITE();
      meshList.push_back(windowsMesh);

      Mesh backPanelMesh;
      backPanelMesh.type = Mesh::QUADS;
      backPanelMesh.indexVec.assign(backPanel,endBackPanel);
      backPanelMesh.material = Material::PLASTIC_BLUE();
      meshList.push_back(backPanelMesh);




      ComputeVertexNormals();
      ComputeBoundingBox();
      ComputeRecursiveBoundingBox(); 

    }
};

int main(int argc, char* argv[]){
  VART::ViewerGlutOGL::Init(&argc, argv);
  static VART::Scene scene;
  static VART::ViewerGlutOGL viewer;

  VART::Camera camera(VART::Point4D(15,2,2), VART::Point4D(2,2,2), VART::Point4D::Z());

  House house;
  Terrain terrain;

  VART::Transform sunScale;
  VART::Transform sunTranslation;
  VART::Sphere sun;
  VART::Material material;
  VART::Texture sunTexture;

  if(sunTexture.LoadFromFile("sol.jpg")){
    material.SetTexture(sunTexture);
    sun.SetMaterial(material);
    sun.SetDescription("Sun");

    sunScale.MakeScale(2,2,2);
    sunScale.AddChild(sun);

    sunTranslation.MakeTranslation(-6, 6, 8);
    sunTranslation.AddChild(sunScale);

  }

  scene.AddObject(&sunTranslation);

  scene.AddObject(&house);
  scene.AddObject(&terrain);
  scene.AddLight(VART::Light::BRIGHT_AMBIENT());
  scene.AddCamera(&camera);

  viewer.SetTitle("House example");
  viewer.SetScene(scene);

  scene.DrawLightsOGL();
  VART::ViewerGlutOGL::MainLoop();

  return 0;
}