////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass(TextClass* &text, D3DClass* &d3d)
{
	m_Text = text;
	m_D3D = d3d;

	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = -x;
	m_positionY = -y;
	m_positionZ = -z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = -1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::Follow(D3DXVECTOR3 followTarget)
{
	float distance = -25.0f; //Distance to keep from car
	float height = -4.0f; //Height from ground
	D3DXVECTOR3 startingForwardVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f); //Inital forward vector to calculate new angle
	D3DXVECTOR3 myTarget;
	D3DXVECTOR3 myPosition = GetPosition();

	//Find vector between position and followtarget
	D3DXVec3Subtract(&myTarget, &followTarget, &myPosition);

	//Find angle followtarget vector and starting forward vector
	float deltaAngle = atan2(myTarget.z, myTarget.x) - atan2(startingForwardVector.z, startingForwardVector.x);
	deltaAngle = deltaAngle * 57.2958f;

	D3DXVec3Normalize(&myTarget, &myTarget); //Normalise followtarget vector
	myTarget = myTarget * distance; //Multiply by distance
	myTarget.y = height; //Add height
	D3DXVec3Add(&myTarget, &myTarget, &followTarget); //Add to followtarget position

	SetPosition(myTarget.x, myTarget.y, myTarget.z); //Set position
	SetRotation(0.0f, -deltaAngle, 0.0f);

	//Debug Info
	m_Text->UpdateSentence(m_Text->m_sentence1, "2 : Test", 60, 50, 1.0f, 1.0f, 1.0f, m_D3D->GetDeviceContext());
	m_Text->UpdateSentence(m_Text->m_sentence2, "3: Test", 60, 70, 1.0f, 1.0f, 1.0f,  m_D3D->GetDeviceContext());
	m_Text->UpdateSentence(m_Text->m_sentence3, "4: Test", 60, 90, 1.0f, 1.0f, 1.0f,  m_D3D->GetDeviceContext());
	m_Text->UpdateSentence(m_Text->m_sentence4, "5: Test", 60, 110, 1.0f, 1.0f, 1.0f, m_D3D->GetDeviceContext());
	m_Text->UpdateSentence(m_Text->m_sentence5, "6: Test", 60, 130, 1.0f, 1.0f, 1.0f, m_D3D->GetDeviceContext());
	m_Text->UpdateSentence(m_Text->m_sentence6, "7: Test", 60, 150, 1.0f, 1.0f, 1.0f, m_D3D->GetDeviceContext());
}
