#include "CameraInput.hpp"
#include <glm\gtc\matrix_transform.hpp>

void CameraInput::init(glm::mat4 * view)
{
	viewMat = view;
	input = Input::getInput();

	mouseSpeed = 0.2f;

	start = vec3(-1, 0, 0);
	dir = start;

	setCam(vec3(0, 0, 5), vec3(0, 0, -1));

}

void CameraInput::update(float dt)
{
	if (input->consoleIsActive())
		return;
	float x = 0.0f;
	float y = 0.0f;
	
	input->getCursorDelta(x, y);

	KeyBind kb;
	kb.code = 0;
	kb.mod = 0;
	kb.mouse = 1;
	if (input->isKeyBindPressed(kb)) {
		//printf("Mouse pan\n");
		mousepan(x, y);
	}

	keypan(dt);
	*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
	
	//input->centerCursor(centerX, centerY);

}

void CameraInput::mousepan(float x, float y)
{
	angleH -= x * (mouseSpeed / 5.0f * playbackSpeed);
	angleV -= y * (mouseSpeed / 5.0f * playbackSpeed);
	if (angleV > 89)
		angleV = 89;
	if (angleV < -89)
		angleV = -89;

	//rotate vertically around x
	float rotateRad;
	vec3 view = start;
	rotateRad = toRADIAN * angleV;

	rotV = mat3(cos(rotateRad), -sin(rotateRad), 0.0f,
		sin(rotateRad), cos(rotateRad), 0.0f,
		0.0f, 0.0f, 1.0f);
	view = rotV * view;
	view = normalize(view);

	//rotate horizontally around y
	rotateRad = toRADIAN * angleH;
	rotH = mat3(cos(rotateRad), 0.0f, -sin(rotateRad),
		0.0f, 1.0f, 0.0f,
		sin(rotateRad), 0.0f, cos(rotateRad));
	view = rotH * view;
	dir = normalize(view);
}

void CameraInput::keypan(float dt)
{
	float speedorig = camSpeed;
	camSpeed /= playbackSpeed;

	//if (i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
	//	camSpeed *= 9;
	camSpeed *= dt;

	if (input->isKeyBindPressed(KeyBindings[KEYBIND_FORWARD]))
		pos += dir * camSpeed;

	if (input->isKeyBindPressed(KeyBindings[KEYBIND_BACKWARD]))
		pos -= dir * camSpeed;

	if (input->isKeyBindPressed(KeyBindings[KEYBIND_LEFT]))
	{
		vec3 left = cross(vec3(0, 1, 0), dir);
		left = normalize(left);
		pos += left * camSpeed;
	}
	if (input->isKeyBindPressed(KeyBindings[KEYBIND_RIGHT]))
	{
		vec3 left = cross(dir, vec3(0, 1, 0));
		left = normalize(left);
		pos += left * camSpeed;
	}
	//if (i->getKeyInfo(GLFW_KEY_SPACE))
	//	pos.y += camSpeed;
	//
	//if (i->getKeyInfo(GLFW_KEY_LEFT_CONTROL))
	//	pos.y -= camSpeed;

	camSpeed = speedorig;
}

void CameraInput::setCam(vec3 _pos, vec3 _dir)
{
	_dir = normalize(_dir);

	float angleXZ = atan2(_dir.x, _dir.z) - atan2(dir.x, dir.z);
	angleH += angleXZ * toDEGREE;


	float angleYold = dot(dir, vec3(0, 1, 0));
	float angleYnew = dot(_dir, vec3(0, 1, 0));

	float diff = angleYold - angleYnew;
	diff *= toDEGREE;

	angleV -= diff;

	pos = _pos;
	dir = _dir;

	*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
}