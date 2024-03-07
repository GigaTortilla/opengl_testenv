#include <camera/cam.h>

void CALL updateCam(Camera* cam, GLFWwindow* window, float frameDiff) {
	cam->speed = CAM_SPEED * frameDiff;
	vec3s moveVec = {{ 0.0f, 0.0f, 0.0f }};
	if (glfwGetKey(window, GLFW_KEY_W))
		moveVec = glms_vec3_add(moveVec, moveForward(cam));
	if (glfwGetKey(window, GLFW_KEY_S))
		moveVec = glms_vec3_add(moveVec, moveBackward(cam));
	if (glfwGetKey(window, GLFW_KEY_D))
		moveVec = glms_vec3_add(moveVec, moveRight(cam));
	if (glfwGetKey(window, GLFW_KEY_A))
		moveVec = glms_vec3_add(moveVec, moveLeft(cam));
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		moveVec = glms_vec3_add(moveVec, moveDown(cam));
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		moveVec = glms_vec3_add(moveVec, moveUp(cam));
	moveVec = glms_normalize(moveVec);

	cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(moveVec, cam->speed));

	vec3s direction = {{
		direction.x = cos(glm_rad(cam->yawAngle)) * cos(glm_rad(cam->pitchAngle)),
		direction.y = sin(glm_rad(cam->pitchAngle)),
		direction.z = sin(glm_rad(cam->yawAngle)) * cos(glm_rad(cam->pitchAngle))
	}};
	cam->front = glms_normalize(direction);
	cam->right = glms_normalize(glms_cross(cam->front, (vec3s){{ 0.0f, 1.0f, 0.0f }}));
}

vec3s CALL moveForward(Camera* cam) {
	vec3s toScale = {{ cam->front.x, 0.0f, cam->front.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s CALL moveBackward(Camera* cam) {
	vec3s toScale = {{ -cam->front.x, 0.0f, -cam->front.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s CALL moveRight(Camera* cam) {
	vec3s toScale = {{ cam->right.x, 0.0f, cam->right.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s CALL moveLeft(Camera* cam) {
	vec3s toScale = {{ -cam->right.x, 0.0f, -cam->right.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s CALL moveDown(Camera* cam) {
	vec3s toScale = {{ 0.0f, -cam->up.y, 0.0f }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s CALL moveUp(Camera* cam) {
	vec3s toScale = {{ 0.0f, cam->up.y, 0.0f }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}
