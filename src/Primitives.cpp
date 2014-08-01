
#include <Primitives.h>

#include <Model.h>

#include <math.h>

#include <iostream>

#include <stdio.h>

#define PI 3.141592654

using namespace Engine3d;


ModelMesh<SimpleVertex>* Primitives::MakeSphere(float r, int vsects, int hsects)
{
    std::string name = "sphere";
    char buffer[50];
    sprintf(buffer, "%f%d%d",r,vsects,hsects);
    name += buffer;
    ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(name);

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



    physx::PxGeometry *g = new physx::PxSphereGeometry(r);
    mesh->SetBounds(g);

    return mesh;
}

ModelMesh<SimpleVertex>* Primitives::MakeBox(float w, float h, float l)
{
    std::string name = "box";
    char buffer[50];
    sprintf(buffer, "%f%f%f",l,w,h);
    name += buffer;

    ModelMesh<SimpleVertex>* mesh = new ModelMesh<SimpleVertex>(name);

    Float3f* f1 = new Float3f(-w/2,h/2,l/2);
    Float3f* f2 = new Float3f(w/2,h/2,l/2);
    Float3f* f3 = new Float3f(-w/2,-h/2,l/2);
    Float3f* f4 = new Float3f(w/2,-h/2,l/2);
    Float3f* n1 = new Float3f(0,0,1);

    Float3f* f5 = new Float3f(-w/2,h/2,l/2);
    Float3f* f6 = new Float3f(-w/2,-h/2,l/2);
    Float3f* f7 = new Float3f(-w/2,-h/2,-l/2);
    Float3f* f8 = new Float3f(-w/2,h/2,-l/2);
    Float3f* n2 = new Float3f(-1,0,0);

    Float3f* f9 = new Float3f(-w/2,-h/2,-l/2);
    Float3f* f10 = new Float3f(-w/2,h/2,-l/2);
    Float3f* f11 = new Float3f(w/2,-h/2,-l/2);
    Float3f* f12 = new Float3f(w/2,h/2,-l/2);
    Float3f* n3 = new Float3f(0,0,-1);

    Float3f* f13 = new Float3f(w/2,h/2,-l/2);
    Float3f* f14 = new Float3f(w/2,h/2,l/2);
    Float3f* f15 = new Float3f(w/2,-h/2,l/2);
    Float3f* f16 = new Float3f(w/2,-h/2,-l/2);
    Float3f* n4 = new Float3f(1,0,0);


    Float3f* f17 = new Float3f(-w/2,h/2,l/2);
    Float3f* f18 = new Float3f(-w/2,h/2,-l/2);
    Float3f* f19 = new Float3f(w/2,h/2,-l/2);
    Float3f* f20 = new Float3f(w/2,h/2,l/2);
    Float3f* n5 = new Float3f(0,1,0);

    Float3f* f21 = new Float3f(-w/2,-h/2,l/2);
    Float3f* f22 = new Float3f(-w/2,-h/2,-l/2);
    Float3f* f23 = new Float3f(w/2,-h/2,-l/2);
    Float3f* f24 = new Float3f(w/2,-h/2,l/2);
    Float3f* n6 = new Float3f(0,-1,0);

    Float3f* zero = new Float3f(0,0,0);

    mesh->AddVertex(new SimpleVertex(f1,n1,zero));
    mesh->AddVertex(new SimpleVertex(f2,n1,zero));
    mesh->AddVertex(new SimpleVertex(f3,n1,zero));
    mesh->AddVertex(new SimpleVertex(f4,n1,zero));

    mesh->AddVertex(new SimpleVertex(f5,n2,zero));
    mesh->AddVertex(new SimpleVertex(f6,n2,zero));
    mesh->AddVertex(new SimpleVertex(f7,n2,zero));
    mesh->AddVertex(new SimpleVertex(f8,n2,zero));

    mesh->AddVertex(new SimpleVertex(f9,n3,zero));
    mesh->AddVertex(new SimpleVertex(f10,n3,zero));
    mesh->AddVertex(new SimpleVertex(f11,n3,zero));
    mesh->AddVertex(new SimpleVertex(f12,n3,zero));

    mesh->AddVertex(new SimpleVertex(f13,n4,zero));
    mesh->AddVertex(new SimpleVertex(f14,n4,zero));
    mesh->AddVertex(new SimpleVertex(f15,n4,zero));
    mesh->AddVertex(new SimpleVertex(f16,n4,zero));

    mesh->AddVertex(new SimpleVertex(f17,n5,zero));
    mesh->AddVertex(new SimpleVertex(f18,n5,zero));
    mesh->AddVertex(new SimpleVertex(f19,n5,zero));
    mesh->AddVertex(new SimpleVertex(f20,n5,zero));

    mesh->AddVertex(new SimpleVertex(f21,n6,zero));
    mesh->AddVertex(new SimpleVertex(f22,n6,zero));
    mesh->AddVertex(new SimpleVertex(f23,n6,zero));
    mesh->AddVertex(new SimpleVertex(f24,n6,zero));


    mesh->AddIndex(0);
    mesh->AddIndex(1);
    mesh->AddIndex(2);
    mesh->AddIndex(1);
    mesh->AddIndex(3);
    mesh->AddIndex(2);

    mesh->AddIndex(4);
    mesh->AddIndex(5);
    mesh->AddIndex(6);
    mesh->AddIndex(4);
    mesh->AddIndex(6);
    mesh->AddIndex(7);

    mesh->AddIndex(8);
    mesh->AddIndex(9);
    mesh->AddIndex(10);
    mesh->AddIndex(9);
    mesh->AddIndex(10);
    mesh->AddIndex(11);

    mesh->AddIndex(12);
    mesh->AddIndex(13);
    mesh->AddIndex(14);
    mesh->AddIndex(12);
    mesh->AddIndex(14);
    mesh->AddIndex(15);

    mesh->AddIndex(16);
    mesh->AddIndex(17);
    mesh->AddIndex(18);
    mesh->AddIndex(16);
    mesh->AddIndex(18);
    mesh->AddIndex(19);

    mesh->AddIndex(20);
    mesh->AddIndex(21);
    mesh->AddIndex(22);
    mesh->AddIndex(20);
    mesh->AddIndex(22);
    mesh->AddIndex(23);


    physx::PxGeometry *g = new physx::PxBoxGeometry(w/2,h/2,l/2);
    mesh->SetBounds(g);

    return mesh;

}
