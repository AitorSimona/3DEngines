#include "ComponentCamera.h"


ComponentCamera::ComponentCamera(GameObject* ContainerGO) : Component(ContainerGO, Component::ComponentType::Camera)
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(0.1f, 2000.0f);
	frustum.SetPerspective(1.0f, 1.0f);
	SetAspectRatio(1.0f);
}

ComponentCamera::~ComponentCamera()
{
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

float ComponentCamera::GetFOV() const
{
	return frustum.VerticalFov() * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

float4x4 ComponentCamera::GetOpenGLViewMatrix()
{
	math::float4x4 matrix = frustum.ViewMatrix();
	return matrix.Transposed();
}

float4x4 ComponentCamera::GetOpenGLProjectionMatrix()
{
	math::float4x4 matrix = frustum.ProjectionMatrix();
	return matrix.Transposed();
}

void ComponentCamera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(distance, frustum.FarPlaneDistance());
}

void ComponentCamera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), distance);

}

void ComponentCamera::SetFOV(float fov)
{
	float aspect_ratio = frustum.AspectRatio();
	frustum.SetVerticalFovAndAspectRatio(fov * DEGTORAD, frustum.AspectRatio());
}

void ComponentCamera::SetAspectRatio(float ar)
{
	// When resizing window, adjusting aspect ratio to less than 1.0f and rotating editor camera made mathgeo crash
	if (ar < 1.0f)
		ar = 1.0f;

	frustum.SetHorizontalFovAndAspectRatio(frustum.HorizontalFov(), ar);
}


void ComponentCamera::Look(const float3 & position)
{
	float3 vector = position - frustum.Pos();

	float3x3 matrix = float3x3::LookAt(frustum.Front(), vector.Normalized(), frustum.Up(), float3::unitY);

	frustum.SetFront(matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(matrix.MulDir(frustum.Up()).Normalized());
}

void ComponentCamera::OnUpdateTransform(const float4x4 & global)
{
	frustum.SetFront(global.WorldZ());
	frustum.SetUp(global.WorldY());

	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat quat = Quat::identity;
	global.Decompose(position, quat, scale);

	frustum.SetPos(position);
}

bool ComponentCamera::ContainsAABB(const AABB & ref)
{
	float3 vCorner[8];
	int iTotalIn = 0;
	Plane Planes[8];
	frustum.GetPlanes(Planes);

	ref.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes

			if (Planes[p].IsOnPositiveSide(vCorner[i])) {
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if(iInCount == 0)
			return false;
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return true;
	// we must be partly in then otherwise
	return true;
}

json ComponentCamera::Save() const
{
	json node;
	//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["FOV"] = std::to_string(camera->GetFOV());
//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["NEARPLANE"] = std::to_string(camera->GetNearPlane());
//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["FARPLANE"] = std::to_string(camera->GetFarPlane());
//file[scene_gos[i]->GetName()]["Components"][std::to_string((uint)scene_gos[i]->GetComponents()[j]->GetType())]["ASPECTRATIO"] = std::to_string(camera->GetAspectRatio());

	return node;
}

void ComponentCamera::Load(json& node)
{
}

void ComponentCamera::CreateInspectorNode()
{
}
