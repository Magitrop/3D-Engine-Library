#include "pch.h"

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "LineRendererComponent.h"
#include "ObjectsManager.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "EventSystem.h"

void LineRendererComponent::OnComponentAdded()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    std::vector<Vector3> p = { Vector3(0), Vector3(0) };
    indices = {};
    SetPoints(p);
}

void LineRendererComponent::SetPoints(std::vector<Vector3>& points)
{
    this->points = points;
    glBindVertexArray(VAO);

    if (indices.size() != 0 && indices.size() == this->points.size())
    {
        // so there is no need to reallocate the buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        memcpy(
            glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY),
            &this->points[0],
            this->points.size() * sizeof(Vector3));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else
    {
        indices.clear();
        for (int i = 0; i < this->points.size(); i++)
            indices.push_back(i);

        // 1
        if (glIsBuffer(vertexBuffer))
            glDeleteBuffers(1, &vertexBuffer);
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, this->points.size() * sizeof(Vector3), &this->points[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer
        (
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // 2
        if (glIsBuffer(trianglesBuffer))
            glDeleteBuffers(1, &trianglesBuffer);
        glGenBuffers(1, &trianglesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, trianglesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LineRendererComponent::Render()
{
    Shader sh = *ResourceManager->GetShader("Simple");
	sh.Use();
    sh.setMat4("MVP", EventSystem->GetMainCamera()->GetProjectionViewMatrix());
    sh.setVec3("objPos", gameObject->transform->GetPosition());
    sh.setVec4("vertexColor", color);

    glBindVertexArray(VAO);
    glLineWidth(lineWidth);
    glDrawElements(connectLines ? GL_LINE_STRIP : GL_LINES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

    glBindVertexArray(0);
	glUseProgram(0);
}

void LineRendererComponent::Serialize(Json& writeTo)
{
    RendererComponentBase::Serialize(writeTo);
    writeTo[GetComponentName()]["lineWidth"] = lineWidth;
    writeTo[GetComponentName()]["connectLines"] = connectLines;
    writeTo[GetComponentName()]["color"]["r"] = color.r;
    writeTo[GetComponentName()]["color"]["g"] = color.g;
    writeTo[GetComponentName()]["color"]["b"] = color.b;
    writeTo[GetComponentName()]["color"]["a"] = color.a;
    if (points.size() > 0)
        for (int i = 0; i < points.size(); i++)
        {
            writeTo[GetComponentName()]["points"][i]["x"] = points[i].x;
            writeTo[GetComponentName()]["points"][i]["y"] = points[i].y;
            writeTo[GetComponentName()]["points"][i]["z"] = points[i].z;
        }
    else
    {
        writeTo[GetComponentName()]["points"][0]["x"] = 0;
        writeTo[GetComponentName()]["points"][0]["y"] = 0;
        writeTo[GetComponentName()]["points"][0]["z"] = 0;
    }
    //writeTo[GetComponentName()]["ABC"][0]["a"][0]["b"]["c"][0]["d"][0]["e"] = 0;
}

void LineRendererComponent::Deserialize(const Json& readFrom)
{
    RendererComponentBase::Deserialize(readFrom);
    lineWidth = readFrom[GetComponentName()]["lineWidth"];
    connectLines = readFrom[GetComponentName()]["connectLines"];
    color = Color(
        readFrom[GetComponentName()]["color"]["r"], 
        readFrom[GetComponentName()]["color"]["g"], 
        readFrom[GetComponentName()]["color"]["b"], 
        readFrom[GetComponentName()]["color"]["a"]);

    std::vector<Vector3> ps;
    if (readFrom[GetComponentName()].find("points") != readFrom[GetComponentName()].end())
        for (int index = 0; index < readFrom[GetComponentName()]["points"].size(); index++)
        {
            ps.push_back({
               readFrom[GetComponentName()]["points"][index]["x"],
               readFrom[GetComponentName()]["points"][index]["y"],
               readFrom[GetComponentName()]["points"][index]["z"] });
        }
    else ps.push_back(Vector3(0));
    SetPoints(ps);
}

std::string LineRendererComponent::GetComponentName()
{
    return "Line Renderer";
}