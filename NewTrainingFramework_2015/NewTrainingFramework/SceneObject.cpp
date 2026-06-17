#include "stdafx.h"
#include "SceneObject.h"


float SceneObject::getKspec() const {
	return 1.0f;
}

float SceneObject::getKdiff() const {
	return 1.0f;
}
void SceneObject::Draw(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();

	sendCommonData(esContext);
	shader->SetFogParameters(sm->fogColor, sm->smallRadius, sm->largeRadius);
	sendSpecificData(esContext);


	int err = glGetError();
	glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_SHORT, 0);
	err = glGetError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneObject::sendCommonData(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();

	Matrix scaleMatrix, translationMatrix, modelMatrix, xRotationMatrix, yRotationMatrix, zRotationMatrix;
	scaleMatrix.SetScale(scale.x, scale.y, scale.z);
	translationMatrix.SetTranslation(position);
	xRotationMatrix.SetRotationX(rotation.x);
	yRotationMatrix.SetRotationY(rotation.y);
	zRotationMatrix.SetRotationZ(rotation.z);
	modelMatrix = scaleMatrix * xRotationMatrix * yRotationMatrix * zRotationMatrix * translationMatrix;

	Matrix MVP = modelMatrix * camera->viewMatrix * camera->perspectiveMatrix;
	ResourceManager* rm = ResourceManager::getInstance();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	for (int i = 0; i < textures.size() && i < MAX_TEXTURES; i++)
	{
		if (textures[i]->tr->type == "cube") {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i]->textureId);
			glUniform1i(shader->textureUniform[i], i);
			continue;
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->textureId);
		glUniform1i(shader->textureUniform[i], i);
	}

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shader->uv2Attribute != -1)
	{
		glEnableVertexAttribArray(shader->uv2Attribute);
		glVertexAttribPointer(shader->uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv2));
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (shader->MVP != -1)
	{
		glUniformMatrix4fv(shader->MVP, 1, GL_FALSE, (float*)MVP.m);
	}

	if (shader->modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(shader->modelMatrixUniform, 1, GL_FALSE, (float*)modelMatrix.m);
	}

	if (shader->normalAttribute != -1) {
		glEnableVertexAttribArray(shader->normalAttribute);
		glVertexAttribPointer(shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	}
	
	if (shader->cameraPosUniform != -1) {
		glUniform3f(shader->cameraPosUniform, camera->position.x, camera->position.y, camera->position.z);
	}

	glUniform3f(glGetUniformLocation(shader->program, "u_ambientColor"), sm->ambientColor.x, sm->ambientColor.y, sm->ambientColor.z);
	glUniform1f(glGetUniformLocation(shader->program, "u_ambientRatio"), sm->ambientRatio);

	glUniform1i(glGetUniformLocation(shader->program, "u_numLights"), sm->lights.size());
	for (int i = 0; i < sm->lights.size(); ++i) {
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightPositions[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->position.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightDirections[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->direction.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightDiffuse[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->colorDiffuse.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightSpecular[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->colorSpecular.x);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpecPower[" + std::to_string(i) + "]").c_str()), sm->lights[i]->specPower);
		glUniform1i(glGetUniformLocation(shader->program, ("u_lightTypes[" + std::to_string(i) + "]").c_str()), sm->lights[i]->type);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpotCutoff[" + std::to_string(i) + "]").c_str()), sm->lights[i]->spotCutoff);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpotExponent[" + std::to_string(i) + "]").c_str()), sm->lights[i]->spotExponent);
	}

	glUniform1f(glGetUniformLocation(shader->program, "u_kspec"), this->getKspec());
	glUniform1f(glGetUniformLocation(shader->program, "u_kdiff"), this->getKdiff());
}

void SceneObject::sendSpecificData(ESContext* esContext)
{

}

void SceneObject::Update(float deltaTime)
{
	return;
}
