#include "framework.h"
#include "S00_TestScene.h"

S00_TestScene::S00_TestScene()
{
	// Object
	m_cube = new Cube();
	m_cube->SetTag("Cube");

	m_sphere = new Sphere();
	m_sphere->SetTag("Sphere");

	m_cylinder = new Cylinder();
	m_cylinder->SetTag("Cylinder");

	// Collisions
	m_colliders.push_back(new BoxCollider());
	m_colliders.back()->SetTag("Box1");
	m_colliders.push_back(new BoxCollider());
	m_colliders.back()->SetTag("Box2");

	m_colliders.push_back(new SphereCollider());
	m_colliders.back()->SetTag("Sphere1");
	m_colliders.push_back(new SphereCollider());
	m_colliders.back()->SetTag("Sphere2");

	m_colliders[2]->Pos() = Vector3(3.0f, 0.0f, 0.0f);

	m_colliders.push_back(new CapsuleCollider());
	m_colliders.back()->SetTag("Capsule1");
	m_colliders.push_back(new CapsuleCollider());
	m_colliders.back()->SetTag("Capsule2");

	// model animation
	m_modelAnimator = new ModelAnimator("Jin");
	m_modelAnimator->SetTag("Jin");
	m_modelAnimator->ReadClip("Idle");
	m_modelAnimator->ReadClip("Walk");
	m_modelAnimator->ReadClip("Dancing");
	m_modelAnimator->Scale() = Vector3(0.05f, 0.05f, 0.05f);

	m_modelInstanced = new ModelInstancing("Jin");
	m_modelInstanced->Scale() = Vector3(0.05f, 0.05f, 0.05f);

	for (int z = 0; z < 10; z++)
	{
		for (int x = 0; x < 10; x++)
		{
			Transform* transform = m_modelInstanced->Add();
			transform->Scale() = Vector3(0.05f, 0.05f, 0.05f);
			transform->Pos() = { (float)x * 10.0f, 0.0f, (float)z * 10.0f };
		}
	}

	m_hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");

//	m_particle = new SpriteParticle(L"Textures/Effect/fire_8x2.png", 5, 20, 8, 2, true);
//	m_particle = new Rain();
//	m_particle = new Snow();
	m_particle = new Spark(L"Textures/Effect/star.png", true);
}

S00_TestScene::~S00_TestScene()
{
	SAFE_DELETE(m_particle);
	SAFE_DELETE(m_hpBar);

	SAFE_DELETE(m_modelInstanced);
	SAFE_DELETE(m_modelAnimator);

	for (Collider* collider : m_colliders)
	{
		SAFE_DELETE(collider);
	}
	m_colliders.erase(m_colliders.begin(), m_colliders.end());

	SAFE_DELETE(m_cylinder);
	SAFE_DELETE(m_sphere);
	SAFE_DELETE(m_cube);
}

void S00_TestScene::Update()
{
	m_cube->UpdateWorld();
	m_sphere->UpdateWorld();
	m_cylinder->UpdateWorld();

	Ray ray = CAMERA->ScreenPointToRay(mousePos);
	Contact contact;

	// Box x Ray Contact

	if (m_colliders[0]->IsRayCollision(ray, &contact))
	{
		m_colliders[0]->SetColor(1.0f, 0.0f, 0.0f);
		//m_colliders[1]->Pos() = contact.hitPosition;
	}
	else
	{
		m_colliders[0]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Box x Box Collision

	if (m_colliders[0]->IsCollision(m_colliders[1]))
	{
		m_colliders[0]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[1]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[0]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[1]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Sphere x Ray Contact

	if (m_colliders[2]->IsRayCollision(ray, &contact))
	{
		m_colliders[2]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[2]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Sphere x Sphere Collision

	if (m_colliders[2]->IsCollision(m_colliders[3]))
	{
		m_colliders[2]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[3]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[2]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[3]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Box x Sphere Collision

	if (m_colliders[0]->IsCollision(m_colliders[2]))
	{
		m_colliders[0]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[2]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[0]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[2]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Capsule x Ray Collision

	if (m_colliders[4]->IsRayCollision(ray, &contact))
	{
		m_colliders[4]->SetColor(1.0f, 0.0f, 0.0f);
		//m_colliders[1]->Pos() = contact.hitPosition;
	}
	else
	{
		m_colliders[4]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Capsule x Box Collision

	if (m_colliders[0]->IsCollision(m_colliders[4]))
	{
		m_colliders[0]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[4]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[0]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[4]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Capsule x Sphere Collision

	if (m_colliders[4]->IsCollision(m_colliders[2]))
	{
		m_colliders[4]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[2]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[4]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[2]->SetColor(0.0f, 1.0f, 0.0f);
	}

	// Capsule x Capsule Collision

	if (m_colliders[4]->IsCollision(m_colliders[5]))
	{
		m_colliders[4]->SetColor(1.0f, 0.0f, 0.0f);
		m_colliders[5]->SetColor(1.0f, 0.0f, 0.0f);
	}
	else
	{
		m_colliders[4]->SetColor(0.0f, 1.0f, 0.0f);
		m_colliders[5]->SetColor(0.0f, 1.0f, 0.0f);
	}

	for (Collider* collider : m_colliders)
	{
		collider->UpdateWorld();
	}

	if (KEY_DOWN('1'))
		m_modelAnimator->PlayClip(0);
	if (KEY_DOWN('2'))
		m_modelAnimator->PlayClip(1);
	if (KEY_DOWN('3'))
		m_modelAnimator->PlayClip(2);

	m_modelAnimator->Update();

	m_modelInstanced->Update();
	m_modelInstanced->UpdateWorld();

	Vector3 pos = m_modelAnimator->Pos();
//	m_hpBar->Pos() = CAMERA->WorldToScreen(pos);
	m_hpBar->Pos() = Vector3(150.0f, 100.0f, 1.0f);
	m_hpBar->UpdateWorld();

	if (KEY_DOWN(VK_LBUTTON))
	{
		if (m_colliders[2]->IsRayCollision(ray, &contact))
		{
			m_particle->Play(contact.hitPosition);
		}
	}

	m_particle->Update();
}

void S00_TestScene::Render()
{
	m_cube->Render();
	m_sphere->Render();
	m_cylinder->Render();

	for (Collider* collider : m_colliders)
	{
		collider->Render();
	}

	m_modelAnimator->Render();

	m_modelInstanced->Render();

	m_particle->Render();
}

void S00_TestScene::PreRender()
{
}

void S00_TestScene::PostRender()
{
	ENV->PostSet();
	m_hpBar->Render();
	ENV->Set();
}

void S00_TestScene::GUIRender()
{
	m_cube->GUIRender();
	m_sphere->GUIRender();
	m_cylinder->GUIRender();

	for (Collider* collider : m_colliders)
	{
		collider->GUIRender();
	}

	m_modelAnimator->GUIRender();

	m_hpBar->GUIRender();

	m_particle->GUIRender();
}
