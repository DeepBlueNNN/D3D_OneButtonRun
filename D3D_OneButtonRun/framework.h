// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//
#pragma once

#define  _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG          // console을 attach, 조건은 iostream include위에
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#endif

#pragma warning (disable: 4477)		// printf() : 자료형(Type) - %서식지정자(Variadic Argument) 표기 오류 
#pragma warning (disable: 4467)		// ATL특성은 사용되지 않습니다 : attributed ATL code 방식이 사장될 예정 (uuid 입력방식 / COM 사용)
#pragma warning (disable: 4244)		// 자료형 변환 시 데이터손실 가능 경고
#pragma warning (disable: 4267)		// size_t 자료형 변환 시 데이터손실 가능 경고
#pragma warning (disable: 4018)		// signed <-> unsigned 경고
#pragma warning (disable: 26451)	// 산술오버플로 : (size_t)8바이트 -> (UINT)4바이트

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN			// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>

// C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++
#include <iostream>
#include <windows.h>
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <functional>

using namespace std;

// 3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// 2D
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// DirectXTex
#include <DirectXTex.h>

#pragma comment(lib, "DirectXTex.lib")

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#pragma comment(lib, "ImGui64.lib")

// Assimp - SkeletalMesh, StaticMesh, Material 3D모델 정보 사용
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp-vc142-mtd.lib")

// namespace
using namespace DirectX;
using namespace DirectX::TriangleTests;

// typrdef
typedef XMFLOAT4	Float4;
typedef XMFLOAT3	Float3;
typedef XMFLOAT2	Float2;
typedef XMVECTOR	Vector4;
typedef XMMATRIX	Matrix;
typedef XMFLOAT4X4	MatrixF;
typedef XMFLOAT4X4	Float4x4;

// BONE
#define MAX_LIGHT    10
#define MAX_FRAME    512
#define MAX_BONE	 512
#define MAX_INSTANCE 512

// SAFE DELETE
#define SAFE_DELETE(p)           {if(p) { delete(p); (p)=NULL;}}
#define SAFE_RELEASE(p)          {if(p) {(p)->Release(); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p)     {if(p) { delete [] (p); (p)=NULL;}}

// TIME
#define DELTA                    ImGui::GetIO().DeltaTime

// KEY
#define KEY_PRESS(k)             Keyboard::GetInstance()->Press(k)
#define KEY_DOWN(k)              Keyboard::GetInstance()->Down(k)
#define KEY_UP(k)                Keyboard::GetInstance()->Up(k)

// SINGLETON
#define MAIN			MainWindow::GetInstance()
#define DEVICE			MainWindow::GetInstance()->GetDevice()
#define DC				MainWindow::GetInstance()->GetDC()
#define SCENEMANAGER	SceneManager::Get()
#define ENV				Environment::Get()
#define CAMERA			SaveLoadManager::Get()->GetMainCamera()
#define SAVELOAD		SaveLoadManager::Get()

// MainWindow
#include "Program/MainWindow.h"

// Buffers
#include "FrameWork/Buffer/ConstBuffer.h"
#include "FrameWork/Buffer/GlobalBuffer.h"
#include "FrameWork/Buffer/VertexBuffer.h" 
#include "FrameWork/Buffer/indexBuffer.h" 
#include "Framework/Buffer/StructuredBuffer.h"
#include "FrameWork/Buffer/VertexLayouts.h" 

// Shaders
#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"

// Utilities
#include "Framework/Utilities/FileDialog.h"
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Font.h"
#include "Framework/Utilities/StringPath.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Keyboard.h"

// Math
#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/GameMath.h"

using namespace GameMath;

// Scenes
#include "FrameWork/Base/Scene.h"
#include "FrameWork/Base/SceneManager.h"

// Environments
#include "Framework/State/RasterizerState.h"
#include "Framework/State/SamplerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Environment/Transform.h"
#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"

// Renders
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

// Models
#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModeClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"

// Objects
#include "Framework/Object/GameObject.h"
#include "Framework/Object/Basic/Cube.h"
#include "Framework/Object/Basic/Sphere.h"
#include "Framework/Object/Basic/Cylinder.h"
#include "Framework/Object/Landscape/Sky.h"
#include "Framework/Object/Landscape/Terrain.h"

// Colliders
#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

// UIs
#include "Framework/Object/Basic/Quad.h"
#include "Framework/Object/UI/ProgressBar.h"
#include "Framework/Object/UI/Button.h"
#include "Framework/Object/UI/UITexture.h"
#include "Framework/Object/UI/UIPanel.h"
#include "Framework/Object/UI/SubMenu.h"
#include "Framework/Object/UI/HowToPlay.h"

// Particles
#include "Framework/Object/Particle/Particle.h"
#include "Framework/Object/Particle/SpriteParticle.h"
#include "Framework/Object/Particle/Rain.h"
#include "Framework/Object/Particle/Snow.h"
#include "Framework/Object/Particle/Spark.h"

// Gameplay
#include "Framework/Object/Gameplay/GameActor.h"
#include "Framework/Object/Gameplay/InstancingActor.h"
#include "Framework/Object/Gameplay/GamePlayer.h"
#include "Framework/Object/Gameplay/TargetActor.h"
#include "Framework/Base/SaveLoadManager.h"

extern  Vector3 mousePos;