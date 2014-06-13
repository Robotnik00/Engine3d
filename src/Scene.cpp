#include <Scene.h>

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <math.h>

using namespace Engine3d;

SceneObjectNode::SceneObjectNode()
{
    mParent = NULL;
    mTransform = glm::mat4(1);
    mDeltaScale = glm::vec3(1);
    mDeltaRotate = glm::vec3(0);
    mDeltaTranslate = glm::vec3(0);

}

SceneObjectNode::~SceneObjectNode()
{

}

void SceneObjectNode::Update()
{
    mTransform = glm::scale(mTransform, mDeltaScale);
    if(mDeltaRotate != glm::vec3(0))
    {
        mTransform = glm::rotate(mTransform, (float)pow(glm::dot(mDeltaRotate, mDeltaRotate),.5), glm::normalize(mDeltaRotate));
    }
    mTransform = glm::translate(mTransform, mDeltaTranslate);

    mDeltaScale = glm::vec3(1);
    mDeltaRotate = glm::vec3(0);
    mDeltaTranslate = glm::vec3(0);


    for(int i = 0; i < mActions.size(); i++)
    {
        mActions[i]->PerformAction();
    }
    for(int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->Update();
    }
}

void SceneObjectNode::Draw(float delta)
{
    for(int i = 0; i < mAssets.size(); i++)
    {
        mAssets[i]->Bind();
    }


    mInterpolator = mTransform;
    mInterpolator = GetGlobalInterpolator();
    glm::vec3 d_translate = mDeltaTranslate * delta;
    glm::vec3 d_rotate = mDeltaRotate * delta;
    glm::vec3 d_scale = glm::vec3(1,1,1) - (glm::vec3(1,1,1) - mDeltaScale) * delta;

    mInterpolator = glm::scale(mInterpolator, d_scale);
    if(mDeltaRotate != glm::vec3(0))
    {
        mInterpolator = glm::rotate(mInterpolator, (float)pow(glm::dot(d_rotate, d_rotate),.5), glm::normalize(d_rotate));
    }
    mInterpolator = glm::translate(mInterpolator, d_translate);

    for(int i = 0; i < mDrawInterfaces.size(); i++)
    {
        mDrawInterfaces[i]->Draw(&mInterpolator);
    }

    for(int i = 0; i < mChildren.size(); i++)
    {
        mChildren[i]->Draw(delta);
    }

    for(int i = 0; i < mAssets.size(); i++)
    {
        mAssets[i]->UnBind();
    }
}

void SceneObjectNode::Translate(glm::vec3 translation)
{
    //mTransform = glm::translate(mTransform, translation);
    mDeltaTranslate += translation;
}

void SceneObjectNode::Rotate(float angle, glm::vec3 axis)
{
    //mTransform = glm::rotate(mTransform, angle, glm::normalize(axis));
    mDeltaRotate += (angle * axis);
}

void SceneObjectNode::Scale(glm::vec3 scale)
{
    //mTransform = glm::scale(mTransform, scale);
    mDeltaScale *= scale;
}

void SceneObjectNode::SetLocalPosition(glm::vec3 loc)
{
    mTransform[3][0] = loc.x;
    mTransform[3][1] = loc.y;
    mTransform[3][2] = loc.z;
}


void SceneObjectNode::SetLocalTransform(glm::mat4 transform)
{
    mTransform = transform;
}

void SceneObjectNode::SetGlobalTransform(glm::mat4 transform)
{
    mDeltaTranslate = glm::vec3(0);
    mDeltaScale = glm::vec3(0);
    mDeltaRotate = glm::vec3(0);
    mTransform = glm::mat4(1);
    glm::mat4 glbtransform = GetGlobalTransform();
    glbtransform = glm::inverse(glbtransform);
    mTransform = glbtransform * transform;
}

glm::mat4 SceneObjectNode::GetLocalTransform()
{
    return mTransform;
}

glm::mat4 SceneObjectNode::GetGlobalTransform()
{
    if(mParent == NULL)
    {
        return mTransform;
    }

    return mParent->GetGlobalTransform() * mTransform;
}

glm::mat4 SceneObjectNode::GetGlobalInterpolator()
{
    if(mParent == NULL)
    {
        return mInterpolator;
    }

    return mParent->GetGlobalInterpolator() * mInterpolator;
}
glm::vec3 SceneObjectNode::GetGlobalPosition()
{
    glm::mat4 transform = GetGlobalTransform();
    glm::vec3 position = glm::vec3(0);
    position.x = transform[3][0];
    position.y = transform[3][1];
    position.z = transform[3][2];
    return position;
}
