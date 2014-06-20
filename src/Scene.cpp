#include <Scene.h>

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <math.h>

using namespace Engine3d;

SceneObjectNode::SceneObjectNode()
{
    mParent = NULL;
    mTransform = glm::mat4(1);
    mPrevTransform = glm::mat4(1);

}

SceneObjectNode::~SceneObjectNode()
{

}

void SceneObjectNode::Update()
{
    mPrevTransform = mTransform;

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

    glm::mat4 prevtran = GetPrevGlobalTransform();
    glm::mat4 tran = GetGlobalTransform();
    mInterpolator = prevtran + (tran - prevtran) * delta;

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
    mTransform = glm::translate(mTransform, translation);
}

void SceneObjectNode::Rotate(float angle, glm::vec3 axis)
{
    mTransform = glm::rotate(mTransform, angle, glm::normalize(axis));
}

void SceneObjectNode::Scale(glm::vec3 scale)
{
    mTransform = glm::scale(mTransform, scale);
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
glm::mat4 SceneObjectNode::GetPrevGlobalTransform()
{
    if(mParent == NULL)
    {
        return mPrevTransform;
    }

    return mParent->GetPrevGlobalTransform() * mPrevTransform;

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
