#pragma once

#include <Model.h>
#include <glm/glm.hpp>

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

    class DrawModel : public DrawInterface
    {
    public:
        DrawModel(Model* model) { mModel = model; }
        virtual void Draw(glm::mat4 *interpolator)
        {
            mModel->Draw(interpolator);
        }

    protected:
        Model* mModel;
    };

    template <class T> class DrawMesh : public DrawInterface
    {
    public:
        DrawMesh(ModelMesh<T>* mesh) { mMesh = mesh; }
        virtual void Draw(glm::mat4* interpolator) { mMesh->Draw(interpolator); }

    protected:
        ModelMesh<T>* mMesh;
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
        void SetLocalOrientation(float angle, glm::vec3 axis);
        void SetLocalScale(glm::vec3 scale);

        void SetGlobalPosition(glm::vec3 loc);
        void SetGlobalOrientation(float angle, glm::vec3 axis);

        void SetLocalTransform(glm::mat4 transform);
        void SetGlobalTransform(glm::mat4 transform);

        glm::mat4 GetGlobalTransform();
        glm::mat4 GetLocalTransform();

        void SetParent(SceneObjectNode* parent) { mParent = parent; }
        SceneObjectNode* GetParent() { return mParent; }

    protected:

        glm::mat4 GetGlobalInterpolator();


        std::vector<Action*> mActions;
        std::vector<DrawInterface*> mDrawInterfaces;
        std::vector<Asset*> mAssets;
        SceneObjectNode* mParent;
        std::vector<SceneObjectNode*> mChildren;
        glm::mat4 mTransform;
        glm::mat4 mInterpolator;


        glm::vec3 mDeltaTranslate;
        glm::vec3 mDeltaRotate;
        glm::vec3 mDeltaScale;
    };

}
