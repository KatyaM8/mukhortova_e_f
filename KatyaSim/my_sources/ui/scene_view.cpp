#include "../pch.h"
#include "scene_view.h"


#include <imgui.h>

namespace nui
{
  void SceneView::resize(int32_t width, int32_t height)
  {
    mSize.x = width;
    mSize.y = height;

    mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t) mSize.y);
  }

  void SceneView::on_mouse_move(double x, double y, nelems::EInputButton button)
  {
    mCamera->on_mouse_move(x, y, button);
  }

  void SceneView::on_mouse_wheel(double delta)
  {
    mCamera->on_mouse_wheel(delta);
  }

  void SceneView::load_mesh(const std::string& filepath)
  {
    if(!mMesh)
      mMesh = std::make_shared<nelems::Mesh>();
    
    mMesh->load(filepath);
    

  }

  void SceneView::render()
  {

    mShader->use();

    mLight->update(mShader.get());

    mFrameBuffer->bind();

    if (mMesh)
    {
      mMesh->update(mShader.get());
      mMesh->render();
    }

    mFrameBuffer->unbind();

    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    mSize = { viewportPanelSize.x, viewportPanelSize.y };

    mCamera->set_aspect(mSize.x / mSize.y);
    mCamera->update(mShader.get());
    //igl::opengl::glfw::Viewer viewer;

    // add rendered texture to ImGUI scene window
    uint64_t textureID = mFrameBuffer->get_texture();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
  }
}
