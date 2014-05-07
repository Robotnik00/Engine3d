
#include <Primitives.h>

#include <Model.h>

#include <math.h>

#include <iostream>

#define PI 3.141592654

using namespace Engine3d;


ModelMesh<SimpleVertex>* Primitives::MakeSphere(float r, int vsects, int hsects)
{
    ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>("sphere");

    float thetastep = PI/(hsects+1);
    std::cout << thetastep << std::endl;
    float theta = 0;
    float phistep = 2*PI/vsects;
    float phi = 0;

    Float3f* zero = new Float3f(0,0,0);

    SimpleVertex* top = new SimpleVertex(new Float3f(0,0,r), new Float3f(0,0,r), zero);
    mesh->AddVertex(top);

    for(int i = 0; i < hsects; i++)
    {
        theta += thetastep;
        for(int j = 0; j < vsects; j++)
        {
            float x = cos(phi)*sin(theta) * r;
            float y = sin(phi)*sin(theta) * r;
            float z = cos(theta) * r;
            std::cout << x << " " << " " << y << " " << z << std::endl;
            Float3f* f = new Float3f(x,y,z);
            SimpleVertex* v = new SimpleVertex(f,f,zero);
            mesh->AddVertex(v);
            phi += phistep;
        }
    }
    SimpleVertex* bottom = new SimpleVertex(new Float3f(0,0,-r), new Float3f(0,0,-r), zero);
    mesh->AddVertex(bottom);

    std::cout << "verts: " << mesh->GetVertices().size() << std::endl;



    //add top indices
    for(int i = 2; i < vsects+1; i++)
    {
        mesh->AddIndex(0);
        mesh->AddIndex(i-1);
        mesh->AddIndex(i);
    }
    mesh->AddIndex(0);
    mesh->AddIndex(1);
    mesh->AddIndex(vsects);



    for(int i = 0; i < hsects-1; i++)
    {
        for(int j = 0; j < vsects+1; j++)
        {
            mesh->AddIndex(vsects*i+j);
            mesh->AddIndex(vsects*i+j+1);
            mesh->AddIndex(vsects*(i+1)+j);
        }
        for(int j = 0; j < vsects+1; j++)
        {
            mesh->AddIndex(vsects*i+j+1);
            mesh->AddIndex(vsects*(i+1)+j+1);
            mesh->AddIndex(vsects*(i+1)+j);
        }
    }

    //add bottom indices
    for(int i = 1; i < vsects; i++)
    {
        mesh->AddIndex(mesh->GetVertices().size() - 1);
        mesh->AddIndex(mesh->GetVertices().size() - 1 - i);
        mesh->AddIndex(mesh->GetVertices().size() - 2 - i);
    }
    mesh->AddIndex(mesh->GetVertices().size() - 1);
    mesh->AddIndex(mesh->GetVertices().size() - 2);
    mesh->AddIndex(mesh->GetVertices().size() - vsects-1);



    return mesh;
}

ModelMesh<SimpleVertex>* Primitives::MakeBox(float l, float w, float h)
{

}
