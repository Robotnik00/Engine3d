#pragma once

#include <Model.h>
#include <glm/glm.hpp>
#define NDEBUG
#include <PxPhysicsAPI.h>

namespace Engine3d
{
    class Action
    {
    public:
        virtual void PerformAction() = 0;
    };

    class DrawInterface
    {
    public:
        virtual void Draw(glm::mat4* interpolator) = 0;
    };

    class CameraLocationUpdater : public DrawInterface
    {
    public:

        CameraLocationUpdater(GLuint programid)
        {
            mCameraLocationUpdater = new Engine3d::UniformMat4("cameratransform", programid);

        }

        virtual void Draw(glm::mat4 *interpolator)
        {
            mCameraLocationUpdater->SetVal(*interpolator);
            mCameraLocationUpdater->Bind();

        }

    protected:
        UniformMat4* mCameraLocationUpdater;
    };

    class DrawModel : public DrawInterface
    {
    public:
        DrawModel(Model* model) { mModel = model; mOffset = glm::mat4(1); }
        virtual void Draw(glm::mat4 *interpolator)
        {
            glm::mat4 transform = mOffset * *interpolator;
            mModel->Draw(&transform);
        }

        virtual void SetOffset(glm::mat4 offset) { mOffset = offset; }

    protected:
        Model* mModel;
        glm::mat4 mOffset;
    };

    class DrawMesh : public DrawInterface
    {
    public:
        DrawMesh(ModelMeshBase* mesh) { mMesh = mesh; }
        virtual void Draw(glm::mat4* interpolator) { mMesh->Draw(interpolator); }

    protected:
        ModelMeshBase* mMesh;
    };

    class SceneObjectNode
    {
    public:
        SceneObjectNode();
        ~SceneObjectNode();

        void AddAction(Action* action) { mActions.push_back(action); }
        void AddDrawInterface(DrawInterface* drawinterface) { mDrawInterfaces.push_back(drawinterface); }
        void AddAsset(Asset* asset) { mAssets.push_back(asset); }
        void AddChild(SceneObjectNode* child) { mChildren.push_back(child); child->SetParent(this); }

        void RemoveAction(Action* action) {}
        void RemoveDrawInterface(DrawInterface* drawinterface) {}
        void RemoveAsset(Asset* asset) {}
        void RemoveChild(SceneObjectNode* child) {}


        void Update();
        void Draw(float delta);

        void Translate(glm::vec3 translation);
        void Rotate(float angle, glm::vec3 axis);
        void Scale(glm::vec3 scale);

        void SetLocalPosition(glm::vec3 loc);

        void SetLocalTransform(glm::mat4 transform);
        void SetGlobalTransform(glm::mat4 transform);

        glm::mat4 GetGlobalTransform();
        glm::mat4 GetLocalTransform();

        glm::vec3 GetGlobalPosition();
        glm::vec3 GetGlobalOrientation();

        void SetParent(SceneObjectNode* parent) { mParent = parent; }
        SceneObjectNode* GetParent() { return mParent; }


        glm::mat4 GetGlobalInterpolator();

        glm::mat4 GetPrevGlobalTransform();
    protected:



        std::vector<Action*> mActions;
        std::vector<DrawInterface*> mDrawInterfaces;
        std::vector<Asset*> mAssets;
        SceneObjectNode* mParent;
        std::vector<SceneObjectNode*> mChildren;
        glm::mat4 mNextTranslation;
        glm::mat4 mNextRotation;
        glm::mat4 mNextScale;
        glm::mat4 mTransform;
        glm::mat4 mPrevTransform;
        glm::mat4 mInterpolator;

        bool mNewTransform;

    };

}
