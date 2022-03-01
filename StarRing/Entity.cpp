#include "pch.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"
namespace StarRing {
	
	Entity::Entity(const string& name, Scene* scene, Entity* parent):m_Scene(scene), m_EntityName(name),m_Parent(parent)
	{
	}
	Entity::~Entity()
	{
		m_Parent = nullptr;
		for (auto child : m_Children) {
			child = NULL;
		}
		m_Children.clear();
	}
	vector<NativeScriptComponent>& Entity::GetNativeScriptComponents()
	{
		return m_NativeScriptComponents;
	}
	
	//不能在第一次创建时使用
	void Entity::SetParent(Entity* parent)
	{
		if (m_Parent) {
			m_Parent->DeleteChildEntity(make_shared<Entity>(*this));
		}

		if (parent) {
			parent->AddChildEntity(make_shared<Entity>(*this));
		}
		m_Parent = parent;
	}
	const Entity* Entity::GetParent()const {
		return m_Parent;
	};
	const vector<EntityRef>& Entity::GetChildren()const {
		return m_Children;
	};
	const string& Entity::GetName()const {
		return m_EntityName;
	};
	void Entity::SetName(const string& name) {
		m_EntityName = name;
	}
	const string& Entity::GetFilePath() const
	{
		return m_FilePath;
	}
	void Entity::SetScene(Scene* scene)
	{
		m_Scene = scene;
	};
	void Entity::AddChildEntity(EntityRef child)
	{
		assert(child, "添加的子Entity是空的！");
		m_Children.push_back(child);
	}
	void Entity::DeleteChildEntity(EntityRef child)
	{
		assert(child.get(), "删除的子Entity是空的！");
		for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) {
			if (*iter == child) {
				*iter = NULL;
				m_Children.erase(iter);
				return;
			}
		}
	}
	bool Entity::operator==(const Entity& entity) {
		return (this->m_EntityName == entity.GetName())&&(this->m_Scene == entity.m_Scene);
	};
	bool Entity::operator!=(const Entity& entity) {
		return !((this->m_EntityName == entity.GetName()) && (this->m_Scene == entity.m_Scene));
	}
	int Entity::GetChildCount() const
	{
		return m_Children.size();
	}
	EntityRef Entity::Create(const string& name, Scene* scene, Entity* parent) {
		return make_shared<Entity>(name,scene,parent);
	}
	EntityRef Entity::Clone(const Entity& entity)
	{
		Entity ret = *new Entity(entity);
		return make_shared<Entity>(ret);
	}
	;
}