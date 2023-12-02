#include "framework.h"

Material::Material()
{
	m_buffer      =  new MaterialBuffer();
	// Model에서 처음에 png파일이 assin안되어 있는 경우를 고려
	m_diffuseMap  = Texture::Add(L"Textures/Color/White.png",L"DM");
	m_specularMap = Texture::Add(L"Textures/Color/White.png",L"SM");
	m_normalMap   = Texture::Add(L"Textures/Color/White.png",L"NM");
}

Material::Material(wstring shaderFile)
{
	SetShader(shaderFile);
	m_buffer        = new MaterialBuffer();
    m_diffuseMap    = Texture::Add(L"Textures/Color/White.png", L"DM");
    m_specularMap   = Texture::Add(L"Textures/Color/White.png", L"SM");
    m_normalMap     = Texture::Add(L"Textures/Color/White.png", L"NM");
}

Material::~Material()
{
	SAFE_DELETE(m_buffer);
}

void Material::Set()
{
    m_diffuseMap->PSSet(0);
    m_specularMap->PSSet(1);
    m_normalMap->PSSet(2);
    
    m_buffer->SetPS(1);
    m_vertexShader->Set();
    m_pixelShader->Set();
}

void Material::GUIRender()
{
    string title = m_name + "_Material";

    if (ImGui::TreeNode(title.c_str()))
    {
        ImTextureID textureID = m_diffuseMap->GetSRV();

        if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
        {
			FileDialog dlg;
			wstring selectFile;
			if (dlg.Open(selectFile,FileDialog::TEXTURE))
			{
				MessageBox(NULL, selectFile.c_str(), L"텍스처파일 변경", MB_OK);
				SetDiffuseMap(selectFile);
			}
        }

        ImGui::TreePop();
    }
}

void Material::SetShader(wstring shaderFile)
{
	m_vertexShader = Shader::AddVS(shaderFile);
	m_pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	if (textureFile.length() > 0)
		m_diffuseMap = Texture::Add(textureFile);
	else
		m_diffuseMap = Texture::Add(L"Textures/Color/White.png");
}

void Material::SetSpecularMap(wstring textureFile)
{
	if (textureFile.length() > 0)
		m_specularMap = Texture::Add(textureFile);
	else
		m_specularMap = Texture::Add(L"Textures/Color/White.png");
}

void Material::SetNormalMap(wstring textureFile)
{
    if (textureFile.length() > 0)
    {
        m_normalMap = Texture::Add(textureFile);
        m_buffer->Get().hasNormalMap = 1;
    }
    else
    {
        m_normalMap = Texture::Add(L"Textures/Color/White.png");
        m_buffer->Get().hasNormalMap = 0;
    }
}

void Material::Save(string file)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* material = document->NewElement("Material");
    material->SetAttribute("Name", m_name.c_str());
    document->InsertFirstChild(material);

    tinyxml2::XMLElement* shader = document->NewElement("Shader");
    string shaderFile;
    if (m_vertexShader)
        StringPath::ToString(m_vertexShader->GetFile());
    shader->SetAttribute("Vertex", shaderFile.c_str());
    if (m_pixelShader)
        shaderFile = StringPath::ToString(m_pixelShader->GetFile());
    shader->SetAttribute("Pixel", shaderFile.c_str());
    material->InsertFirstChild(shader);

    tinyxml2::XMLElement* texture = document->NewElement("Texture");
    string textureFile = StringPath::ToString(m_diffuseMap->GetFile());
    texture->SetAttribute("Diffuse", textureFile.c_str());
    textureFile = StringPath::ToString(m_specularMap->GetFile());
    texture->SetAttribute("Specular", textureFile.c_str());
    textureFile = StringPath::ToString(m_normalMap->GetFile());
    texture->SetAttribute("Normal", textureFile.c_str());
    material->InsertEndChild(texture);

    tinyxml2::XMLElement* property = document->NewElement("Property");
    tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
    diffuse->SetAttribute("R", m_buffer->Get().diffuse.x);
    diffuse->SetAttribute("G", m_buffer->Get().diffuse.y);
    diffuse->SetAttribute("B", m_buffer->Get().diffuse.z);
    diffuse->SetAttribute("A", m_buffer->Get().diffuse.w);
    property->InsertEndChild(diffuse);

    tinyxml2::XMLElement* specular = document->NewElement("Specular");
    specular->SetAttribute("R", m_buffer->Get().specular.x);
    specular->SetAttribute("G", m_buffer->Get().specular.y);
    specular->SetAttribute("B", m_buffer->Get().specular.z);
    specular->SetAttribute("A", m_buffer->Get().specular.w);
    property->InsertEndChild(specular);

    tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
    ambient->SetAttribute("R", m_buffer->Get().ambient.x);
    ambient->SetAttribute("G", m_buffer->Get().ambient.y);
    ambient->SetAttribute("B", m_buffer->Get().ambient.z);
    ambient->SetAttribute("A", m_buffer->Get().ambient.w);
    property->InsertEndChild(ambient);

    tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
    emissive->SetAttribute("R", m_buffer->Get().emissive.x);
    emissive->SetAttribute("G", m_buffer->Get().emissive.y);
    emissive->SetAttribute("B", m_buffer->Get().emissive.z);
    emissive->SetAttribute("A", m_buffer->Get().emissive.w);
    property->InsertEndChild(emissive);

    property->SetAttribute("Shininess", m_buffer->Get().shininess);
    property->SetAttribute("HasNormalMap", m_buffer->Get().hasNormalMap);

    material->InsertEndChild(property);

    document->SaveFile(file.c_str());

    SAFE_DELETE(document);
}

void Material::Load(string file)
{
    m_file = file;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* material = document->FirstChildElement();
    m_name = material->Attribute("Name");

    tinyxml2::XMLElement* shader = material->FirstChildElement();
    wstring shaderFile = StringPath::ToWString(shader->Attribute("Vertex"));
    shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    m_vertexShader = Shader::AddVS(shaderFile);
    shaderFile = StringPath::ToWString(shader->Attribute("Pixel"));
    shaderFile = shaderFile.substr(shaderFile.find_first_of('/') + 1, shaderFile.length());
    m_pixelShader = Shader::AddPS(shaderFile);

    tinyxml2::XMLElement* texture = shader->NextSiblingElement();
    wstring textureFile = StringPath::ToWString(texture->Attribute("Diffuse"));
    SetDiffuseMap(textureFile);
    textureFile = StringPath::ToWString(texture->Attribute("Specular"));
    SetSpecularMap(textureFile);
    textureFile = StringPath::ToWString(texture->Attribute("Normal"));
    SetNormalMap(textureFile);

    tinyxml2::XMLElement* property = texture->NextSiblingElement();
    tinyxml2::XMLElement* diffuse = property->FirstChildElement();
    m_buffer->Get().diffuse.x = diffuse->FloatAttribute("R");
    m_buffer->Get().diffuse.y = diffuse->FloatAttribute("G");
    m_buffer->Get().diffuse.z = diffuse->FloatAttribute("B");
    m_buffer->Get().diffuse.w = diffuse->FloatAttribute("A");

    tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
    m_buffer->Get().specular.x = specular->FloatAttribute("R");
    m_buffer->Get().specular.y = specular->FloatAttribute("G");
    m_buffer->Get().specular.z = specular->FloatAttribute("B");
    m_buffer->Get().specular.w = specular->FloatAttribute("A");

    tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
    m_buffer->Get().ambient.x = ambient->FloatAttribute("R");
    m_buffer->Get().ambient.y = ambient->FloatAttribute("G");
    m_buffer->Get().ambient.z = ambient->FloatAttribute("B");
    m_buffer->Get().ambient.w = ambient->FloatAttribute("A");

    tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
    m_buffer->Get().emissive.x = emissive->FloatAttribute("R");
    m_buffer->Get().emissive.y = emissive->FloatAttribute("G");
    m_buffer->Get().emissive.z = emissive->FloatAttribute("B");
    m_buffer->Get().emissive.w = emissive->FloatAttribute("A");

    m_buffer->Get().shininess = property->FloatAttribute("Shininess");
    m_buffer->Get().hasNormalMap = property->IntAttribute("HasNormalMap");

    SAFE_DELETE(document);
}