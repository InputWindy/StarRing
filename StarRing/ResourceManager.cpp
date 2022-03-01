#include "pch.h"
#include "ResourceManager.h"
#include "Buffer.h"
#include "Log.h"
#include "Mesh.h"
#include "Material.h"
#define LogMat4(_mat4) \
for(int i=0;i<4;++i){\
	for (int j = 0; j < 4; ++j) {\
		cout<<_mat4[i][j]<<" ";\
	}\
cout<<endl;\
}

#define LogVec3(_vec3)    \
cout<<"("<<_vec3.x<<","<<_vec3.y<<","<<_vec3.z<<")"<<endl;

#define aiMat4ToMat4(aimat4,mat4) \
for (int i = 0; i < 4; i++)\
{\
    for (int j = 0; j < 4; j++)\
    {\
        mat4[i][j] = aimat4[i][j];\
    }\
}

using namespace std;
namespace StarRing {
    struct Vertex
    {
        vec3 aPosition;
        vec3 aNormal;
        vec3 aTangent;
        vec3 aBiTangent;

        vec2 aTexCoord;
    };


    //���ض�������ʹ�õ��н�
    static vector<Vertex>       g_Vertices;
    static vector<unsigned int> g_indices;

    //����һ��Mesh���������Ƥ����
    VertexArrayRef ResourceManager::ProcessMesh(const aiNode* node,const aiScene*scene, EntityRef& this_node)
    {
        //�������ݸ�ʽ
        BufferLayout layout = {
            {ShaderDataType::Float3,"aPosition"},
            {ShaderDataType::Float3,"aNormal"},
            {ShaderDataType::Float3,"aTangent"},
            {ShaderDataType::Float3,"aBiTangent"},
            {ShaderDataType::Float2,"aTexCoord"},
        };

        VertexArrayRef m_VertexArray = VertexArray::Create();
        m_MeshResource.insert(pair<string, MeshRef>(node->mName.data,Mesh::Create(node->mName.data,m_VertexArray,this_node.get())));
        
        int mesh_num = node->mNumMeshes;
        //�����������е�Mesh
        for (int k = 0; k < mesh_num; k++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[k]];
            // ����vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector;
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.aPosition = vector;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.aTangent = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.aBiTangent = vector;

                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.aNormal = vector;
                }

                // texture coordinates
                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.aTexCoord = vec;
                }
                else {
                    vertex.aTexCoord = glm::vec2(0.0f, 0.0f);
                }
                g_Vertices.push_back(vertex);
            }

            //����indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    g_indices.push_back(face.mIndices[j]);
            }

            VertexBufferRef m_VertexBuffer = VertexBuffer::Create(&(g_Vertices.data()->aPosition.x), g_Vertices.size() * sizeof(Vertex));
            m_VertexBuffer->Bind();
            m_VertexBuffer->SetLayout(layout);
            m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        }
        
        IndexBufferRef m_IndexBuffer = IndexBuffer::Create(g_indices.data(),g_indices.size());
        m_IndexBuffer->Bind();
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        g_Vertices.clear();
        g_indices.clear();
        return m_VertexArray; 
    }

    void ResourceManager::AnalyzeNode(const aiNode* node,const aiScene*scene,EntityRef& parent) {
        
        //���ⴴ����ʵ�壬�뿪��������ͱ�GCɾ���ˣ�Ҫ�Ҹ��ط�������
        EntityRef this_node = Entity::Create(node->mName.data,nullptr, parent.get());
        parent->AddChildEntity(this_node);
        mat4 inv = inverse(this_node->m_TransformToParent);
        //��ȡ�����任����
        aiMat4ToMat4(node->mTransformation, this_node->m_TransformToParent)
        this_node->m_InvTransformToParent = inv;
        this_node->AddTagComponent(TagComponent(node->mName.data));
        this_node->AddTransformComponent(TransformComponent());

        //��ȡMesh(��ǰ�ڵ���Mesh��ʱ��Ž���)
        if (node->mNumMeshes) {
            VertexArrayRef vao = ProcessMesh(node, scene, this_node);
            this_node->AddMeshFilterComponent(MeshFilterComponent(node->mName.data, vao, this_node.get()));
            this_node->AddMeshRendererComponent(MeshRendererComponent(this_node.get(), make_shared<Mesh>(node->mName.data, vao, this_node.get()),Material::GetDefault()));
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            AnalyzeNode(node->mChildren[i], scene, this_node);
        }
        m_PrefabResource.insert(pair<string, EntityRef>(node->mName.data, this_node));
    }

    //ִ�������������ģ�ͱ����ؽ���prefab���ϣ���Ӧ������meshҲ���뵽��VAO����
    void ResourceManager::LoadModel(const string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // ������ʾ
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        EntityRef prefab = Entity::Create(string(" "), nullptr, nullptr);
        prefab->m_FilePath = path;
        prefab->AddTagComponent(TagComponent(" "));
        prefab->AddTransformComponent(TransformComponent());
        aiNode* rootNode = nullptr;
        if (scene->mRootNode->mName == aiString("RootNode")) {
            rootNode = scene->mRootNode->mChildren[0];
        }
        else
        {
            rootNode = scene->mRootNode;
        }
        for (int i = 0; i < rootNode->mNumChildren; i++)
        {
            //�����ڵ㣬��ȡ������Ϣ����������,���ص���һ���ڵ�
            AnalyzeNode(rootNode->mChildren[i],scene,prefab);
        }
        m_PrefabResource.insert(pair<string, EntityRef>(path, prefab));
        
    }

    void ResourceManager::LoadMaterial(const string& vpath, const string& fpath)
    {
        MaterialRef material = Material::Create(Uniform::GetDefault(),Sampler2D::GetDefault(),Shader::Create(vpath,fpath),RenderState::GetDefault());
        m_MaterialResource.insert(pair<string, MaterialRef>(vpath, material));
    }

    EntityRef ResourceManager::Clone(const EntityRef& entt)
    {
        EntityRef ret = Entity::Clone(*entt);
        ret->m_Children.clear();
        EntityRef child;
        for (auto iter : entt->m_Children) {
            child = Clone(iter);
            child->SetParent(ret.get());
        }
        return ret;
    }

    MaterialRef ResourceManager::Clone(const MaterialRef& material)
    {
        MaterialRef ret = Material::Clone(*material);
        return ret;
    }

    ////////////////////////////////////////////
    ///�����ǵ���VertexArrayʹ�õĹ��ߺ���//////
    ////////////////////////////////////////////

    EntityRef ResourceManager::GetPrefab(const string& filepath)
    {
        auto prefab = m_PrefabResource.find(filepath);
        if (prefab != m_PrefabResource.end()) {
            return Clone(prefab->second);
        }
        else {
            LoadModel(filepath);
            return Clone(m_PrefabResource[filepath]);
        }
    }
    MaterialRef ResourceManager::GetMaterial(const string& vpath, const string& fpath)
    {
        auto material = m_MaterialResource.find(vpath);
        if (material != m_MaterialResource.end()) {
            return Clone(m_MaterialResource[vpath]);
        }
        else {
            LoadMaterial(vpath, fpath);
            return Clone(m_MaterialResource[vpath]);
        }
    }
    void ResourceManager::LogResource()
    {
        cout << "Ԥ������Դ������" << m_PrefabResource.size() << endl;
        for (auto prefab : m_PrefabResource)
        {
            SR_CORE_INFO("Ԥ��������:{0}", prefab.second->GetName())
            SR_CORE_INFO("Ԥ�������:Tag:{0},Transform:{1},Mesh:{2}", prefab.second->HasTagComponent, prefab.second->HasTransformComponent, prefab.second->HasMeshFilterComponent)
            SR_CORE_INFO("Ԥ����������Ŀ:{0}", prefab.second.use_count())
                

        }
    }
}