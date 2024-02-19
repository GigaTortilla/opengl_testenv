#include <cam.h>

void updatePos(Camera* cam, GLFWwindow* window, float frameDiff) {
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
	moveVec = glms_normalize(moveVec);

	cam->pos = glms_vec3_add(cam->pos, glms_vec3_scale(moveVec, cam->speed));
}

vec3s moveForward(Camera* cam) {
	vec3s toScale = {{ cam->front.x, 0.0f, cam->front.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s moveBackward(Camera* cam) {
	vec3s toScale = {{ -cam->front.x, 0.0f, -cam->front.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s moveRight(Camera* cam) {
	vec3s toScale = {{ cam->right.x, 0.0f, cam->right.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}

vec3s moveLeft(Camera* cam) {
	vec3s toScale = {{ -cam->right.x, 0.0f, -cam->right.z }};
	toScale = glms_vec3_normalize(toScale);
	return toScale;
}
