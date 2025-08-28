#define GAME_BIOSHOCK_2 1

#define UPGRADE_SAMPLERS 1

#include "..\..\Core\core.hpp"

namespace
{
   ShaderHashesList pixel_shader_hashes_Tonemap;
   ShaderHashesList pixel_shader_hashes_AA;
   ShaderHashesList shader_hashes_Fog;
}

struct GameDeviceDataBioshock2 final : public GameDeviceData
{
   com_ptr<ID3D11Texture2D> upgraded_post_process_texture;
   com_ptr<ID3D11ShaderResourceView> upgraded_post_process_srv;
   com_ptr<ID3D11RenderTargetView> upgraded_post_process_rtv;
   bool drew_tonemap = false;
   bool drew_aa = false;

   bool infinite = false;

   // BS2/Infinite fog
   com_ptr<ID3D11Texture2D> scene_texture;
   com_ptr<ID3D11ShaderResourceView> scene_texture_srv;
};

class Bioshock2 final : public Game
{
public:
   static GameDeviceDataBioshock2& GetGameDeviceData(DeviceData& device_data)
   {
      return *static_cast<GameDeviceDataBioshock2*>(device_data.game);
   }

   void OnInit(bool async) override
   {
      std::vector<ShaderDefineData> game_shader_defines_data = {
         {"TONEMAP_TYPE", '1', false, false, "0 - SDR: Vanilla\n1 - HDR: Vanilla+\n2 - HDR: Untonemapped"},
      };
      shader_defines_data.append_range(game_shader_defines_data);
      GetShaderDefineData(POST_PROCESS_SPACE_TYPE_HASH).SetDefaultValue('0');
      GetShaderDefineData(VANILLA_ENCODING_TYPE_HASH).SetDefaultValue('1');
      GetShaderDefineData(GAMMA_CORRECTION_TYPE_HASH).SetDefaultValue('1');
      GetShaderDefineData(UI_DRAW_TYPE_HASH).SetDefaultValue('2');

		GetShaderDefineData(TEST_SDR_HDR_SPLIT_VIEW_MODE_NATIVE_IMPL_HASH).SetDefaultValue('1'); // The game just clipped, so HDR is an extension of SDR
   }

   void PrintImGuiAbout() override
   {
      ImGui::Text("Luma for \"BioShock 2 Remastered\" is developed by Pumbo and is open source and free.\nIf you enjoy it, consider donating.", "");

      const auto button_color = ImGui::GetStyleColorVec4(ImGuiCol_Button);
      const auto button_hovered_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
      const auto button_active_color = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(70, 134, 0, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(70 + 9, 134 + 9, 0, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(70 + 18, 134 + 18, 0, 255));
      static const std::string donation_link_pumbo = std::string("Buy Pumbo a Coffee ") + std::string(ICON_FK_OK);
      if (ImGui::Button(donation_link_pumbo.c_str()))
      {
         system("start https://buymeacoffee.com/realfiloppi");
      }
      ImGui::PopStyleColor(3);

      ImGui::NewLine();
      // Restore the previous color, otherwise the state we set would persist even if we popped it
      ImGui::PushStyleColor(ImGuiCol_Button, button_color);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_color);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
#if 0
      static const std::string mod_link = std::string("Nexus Mods Page ") + std::string(ICON_FK_SEARCH);
      if (ImGui::Button(mod_link.c_str()))
      {
         system("start https://www.nexusmods.com/prey2017/mods/149");
      }
#endif
      static const std::string social_link = std::string("Join our \"HDR Den\" Discord ") + std::string(ICON_FK_SEARCH);
      if (ImGui::Button(social_link.c_str()))
      {
         // Unique link for Luma by Pumbo (to track the origin of people joining), do not share for other purposes
         static const std::string obfuscated_link = std::string("start https://discord.gg/J9fM") + std::string("3EVuEZ");
         system(obfuscated_link.c_str());
      }
      static const std::string contributing_link = std::string("Contribute on Github ") + std::string(ICON_FK_FILE_CODE);
      if (ImGui::Button(contributing_link.c_str()))
      {
         system("start https://github.com/Filoppi/Luma");
      }
      ImGui::PopStyleColor(3);

      ImGui::NewLine();
      ImGui::Text("Credits:"
         "\n\nMain:"
         "\nPumbo"

         "\n\nThird Party:"
         "\nReShade"
         "\nImGui"
         "\nRenoDX"
         "\n3Dmigoto"
         "\nOklab"
         "\nDICE (HDR tonemapper)"
         , "");
   }

   void OnCreateDevice(ID3D11Device* native_device, DeviceData& device_data) override
   {
      device_data.game = new GameDeviceDataBioshock2;
   }

   bool OnDrawCustom(ID3D11Device* native_device, ID3D11DeviceContext* native_device_context, CommandListData& cmd_list_data, DeviceData& device_data, reshade::api::shader_stage stages, const ShaderHashesList<OneShaderPerPipeline>& original_shader_hashes, bool is_custom_pass, bool& updated_cbuffers) override
   {
      auto& game_device_data = GetGameDeviceData(device_data);
      auto game_device_data_prev = game_device_data;

#if DEVELOPMENT
      // Check what was the previous shader
      static ShaderHashesList<OneShaderPerPipeline> original_shader_hashes2;
#endif

      if (original_shader_hashes.Contains(shader_hashes_Fog))
      {
         com_ptr<ID3D11RenderTargetView> rtv;
         native_device_context->OMGetRenderTargets(1, &rtv, nullptr);

         if (rtv)
         {
            com_ptr<ID3D11Resource> rtr;
            rtv->GetResource(&rtr);

            uint4 size_a, size_b;
            DXGI_FORMAT format_a, format_b;
            GetResourceInfo(game_device_data.scene_texture.get(), size_a, format_a);
            GetResourceInfo(rtr.get(), size_b, format_b);
            if (size_a != size_b || format_a != format_b)
            {
               game_device_data.scene_texture = CloneTexture<ID3D11Texture2D>(native_device, rtr.get(), DXGI_FORMAT_UNKNOWN, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_BIND_UNORDERED_ACCESS, false, false);
               game_device_data.scene_texture_srv = nullptr;
               if (game_device_data.scene_texture)
               {
                  HRESULT hr = native_device->CreateShaderResourceView(game_device_data.scene_texture.get(), nullptr, &game_device_data.scene_texture_srv);
                  ASSERT_ONCE(SUCCEEDED(hr));
               }
            }

            native_device_context->CopyResource(game_device_data.scene_texture.get(), rtr.get());

            ID3D11ShaderResourceView* const scene_texture_srv_const = game_device_data.scene_texture_srv.get();
            native_device_context->PSSetShaderResources(1, 1, &scene_texture_srv_const);
         }
      }

      // Tonemapper
      if (!game_device_data.drew_tonemap && original_shader_hashes.Contains(pixel_shader_hashes_Tonemap))
      {
         game_device_data.drew_tonemap = true;

         // If we upgrade all R8G8B8A8 textures, there's no need to do this live texture format swap
         if (game_device_data.infinite && enable_swapchain_upgrade && swapchain_upgrade_type == 1 && (!enable_texture_format_upgrades || !texture_upgrade_formats.contains(reshade::api::format::r8g8b8a8_unorm)))
         {
            // We manually upgrade the R8G8B8A8 texture that is used as tonemapper output and AA input (after which the game uses the swapchain as RT), this avoids tampering too much with the game textures.
            // Note that this can be nullptr in some frames in this game (maybe in menus).
            com_ptr<ID3D11RenderTargetView> rtv;
            native_device_context->OMGetRenderTargets(1, &rtv, nullptr);
            if (rtv)
            {
               com_ptr<ID3D11Resource> target_resource;
               rtv->GetResource(&target_resource);

               // TODO: this is for infinite? not for 2
				   // If AA is disabled, this would already be writing to the upgraded swapchain, so we don't need to do anything
               if (!device_data.back_buffers.contains(reinterpret_cast<uint64_t>(target_resource.get())))
               {
                  if (!AreResourcesEqual(game_device_data_prev.upgraded_post_process_texture.get(), target_resource.get(), false))
                  {
                     game_device_data.upgraded_post_process_texture = nullptr;
                     game_device_data.upgraded_post_process_srv = nullptr;
                     game_device_data.upgraded_post_process_rtv = nullptr;

                     game_device_data.upgraded_post_process_texture = CloneTexture<ID3D11Texture2D>(native_device, target_resource.get(), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_BIND_UNORDERED_ACCESS, false, false, nullptr);
                     ASSERT_ONCE(game_device_data.upgraded_post_process_texture);
                     HRESULT hr = native_device->CreateShaderResourceView(game_device_data.upgraded_post_process_texture.get(), nullptr, &game_device_data.upgraded_post_process_srv);
                     ASSERT_ONCE(SUCCEEDED(hr));
                     hr = native_device->CreateRenderTargetView(game_device_data.upgraded_post_process_texture.get(), nullptr, &game_device_data.upgraded_post_process_rtv);
                     ASSERT_ONCE(SUCCEEDED(hr));
                  }
               }
               else
               {
                  game_device_data.upgraded_post_process_texture = nullptr;
                  game_device_data.upgraded_post_process_srv = nullptr;
                  game_device_data.upgraded_post_process_rtv = nullptr;
               }

               // Restoring the state isn't necessary in this game
               if (game_device_data.upgraded_post_process_rtv)
               {
                  ID3D11RenderTargetView* const upgraded_post_process_rtv_const = game_device_data.upgraded_post_process_rtv.get();
                  native_device_context->OMSetRenderTargets(1, &upgraded_post_process_rtv_const, nullptr);
               }
            }
         }
      }

#if DEVELOPMENT
      // We always expect AA immediately after TM (if AA is enabled), if there's any other shader in between, send an assert
      ASSERT_ONCE(!original_shader_hashes.Contains(pixel_shader_hashes_AA) || (game_device_data_prev.drew_tonemap && original_shader_hashes2.Contains(pixel_shader_hashes_Tonemap)));
      // TODO: make sure all shaders that run after sharpening are UI
#endif

      // FXAA
      if (game_device_data.drew_tonemap && !game_device_data.drew_aa && original_shader_hashes.Contains(pixel_shader_hashes_AA))
      {
         game_device_data.drew_aa = true;

         if (game_device_data.infinite && game_device_data.upgraded_post_process_srv.get())
         {
            ID3D11ShaderResourceView* const upgraded_post_process_srv_const = game_device_data.upgraded_post_process_srv.get();
            native_device_context->PSSetShaderResources(0, 1, &upgraded_post_process_srv_const);
         }
         // If we upgrade all R8G8B8A8 textures, there's no need to do this live texture format swap
         else if (!game_device_data.infinite && enable_swapchain_upgrade && swapchain_upgrade_type == 1 && (!enable_texture_format_upgrades || !texture_upgrade_formats.contains(reshade::api::format::r8g8b8a8_unorm)))
         {
            // TODO: BS2: if the SRV 0 is not an upgraded texture, it's a failed copy of the upgraded swapchain, so replace copy the swapchain and set it as SRV 0

            // We manually upgrade the R8G8B8A8 texture that is used as tonemapper output and AA input (after which the game uses the swapchain as RT), this avoids tampering too much with the game textures.
            // Note that this can be nullptr in some frames in this game (maybe in menus).
            com_ptr<ID3D11ShaderResourceView> srv;
            native_device_context->PSGetShaderResources(0, 1, &srv);
            if (srv)
            {
               com_ptr<ID3D11Resource> source_resource;
               srv->GetResource(&source_resource);

               // TODO: this is for infinite? not for 2
               // If AA is disabled, this would already be writing to the upgraded swapchain, so we don't need to do anything
               if (device_data.back_buffers.size() > 0 && !device_data.back_buffers.contains(reinterpret_cast<uint64_t>(source_resource.get())))
               {
                  ID3D11Texture2D* back_buffer = reinterpret_cast<ID3D11Texture2D*>((*device_data.back_buffers.begin())); // There's always only 1 swapchain buffer in DX11
                  source_resource = back_buffer;
                  if (!AreResourcesEqual(game_device_data_prev.upgraded_post_process_texture.get(), source_resource.get(), false))
                  {
                     game_device_data.upgraded_post_process_texture = nullptr;
                     game_device_data.upgraded_post_process_srv = nullptr;
                     game_device_data.upgraded_post_process_rtv = nullptr;

                     game_device_data.upgraded_post_process_texture = CloneTexture<ID3D11Texture2D>(native_device, source_resource.get(), DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE, D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, false, false, nullptr);
                     ASSERT_ONCE(game_device_data.upgraded_post_process_texture);
                     HRESULT hr = native_device->CreateShaderResourceView(game_device_data.upgraded_post_process_texture.get(), nullptr, &game_device_data.upgraded_post_process_srv);
                     ASSERT_ONCE(SUCCEEDED(hr));
                  }

                  native_device_context->CopyResource(game_device_data.upgraded_post_process_texture.get(), source_resource.get());
               }
               else
               {
                  game_device_data.upgraded_post_process_texture = nullptr;
                  game_device_data.upgraded_post_process_srv = nullptr;
                  game_device_data.upgraded_post_process_rtv = nullptr;
               }

               // Restoring the state isn't necessary in this game
               if (game_device_data.upgraded_post_process_srv)
               {
                  ID3D11ShaderResourceView* const upgraded_post_process_srv_const = game_device_data.upgraded_post_process_srv.get();
                  native_device_context->PSSetShaderResources(0, 1, &upgraded_post_process_srv_const);
               }
            }
         }
      }

#if DEVELOPMENT
		original_shader_hashes2 = original_shader_hashes;
#endif

      return false;
   }

   void OnPresent(ID3D11Device* native_device, DeviceData& device_data) override
   {
      auto& game_device_data = GetGameDeviceData(device_data);
      game_device_data.drew_tonemap = false;
      game_device_data.drew_aa = false;
      if (device_data.cloned_pipeline_count == 0)
      {
         game_device_data.upgraded_post_process_texture = nullptr;
         game_device_data.upgraded_post_process_srv = nullptr;
         game_device_data.upgraded_post_process_rtv = nullptr;
      }
   }
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
   if (ul_reason_for_call == DLL_PROCESS_ATTACH)
   {
      Globals::SetGlobals(PROJECT_NAME, "BioShock 2 Remastered Luma mod"); //TODOFT: + Bioshock Infinite
      Globals::VERSION = 1;

      luma_settings_cbuffer_index = 13;
      luma_data_cbuffer_index = 12;

      enable_swapchain_upgrade = true;
      swapchain_upgrade_type = 1;
#if 0 //TODOFT5: check if BS2 and 3 need R11G11B10F to R16G16B16A16F upgrades. Mix the mods? UseLowPrecisionColorBuffer=False FloatingPointRenderTargets = True
      enable_texture_format_upgrades = true;
      texture_upgrade_formats = {
            reshade::api::format::r8g8b8a8_unorm,
            reshade::api::format::r8g8b8a8_unorm_srgb,
            reshade::api::format::r8g8b8a8_typeless,
            reshade::api::format::r8g8b8x8_unorm,
            reshade::api::format::r8g8b8x8_unorm_srgb,
            reshade::api::format::b8g8r8a8_unorm,
            reshade::api::format::b8g8r8a8_unorm_srgb,
            reshade::api::format::b8g8r8a8_typeless,
            reshade::api::format::b8g8r8x8_unorm,
            reshade::api::format::b8g8r8x8_unorm_srgb,
            reshade::api::format::b8g8r8x8_typeless,

            reshade::api::format::r11g11b10_float,
      };
      texture_format_upgrades_2d_size_filters = 0 | (uint32_t)TextureFormatUpgrades2DSizeFilters::SwapchainResolution | (uint32_t)TextureFormatUpgrades2DSizeFilters::SwapchainAspectRatio;
#endif
#if !DEVELOPMENT // TODO: investigate more if it happens (thus disabled in dev mode)
      enable_upgraded_texture_resource_copy_redirection = false; // Resources are upgraded manually, so this isn't needed (it would happen between tonemapping and FXAA)
#endif

      bool infinite = false;
      if (infinite)
      {
         pixel_shader_hashes_Tonemap.pixel_shaders = { Shader::Hash_StrToNum("29D570D8") };
         pixel_shader_hashes_AA.pixel_shaders = { Shader::Hash_StrToNum("27BD2A2E"), Shader::Hash_StrToNum("5CDD5AB1") };
      }
      else
      {
         pixel_shader_hashes_Tonemap.pixel_shaders = { Shader::Hash_StrToNum("6F92E3E3") };
         pixel_shader_hashes_AA.pixel_shaders = { Shader::Hash_StrToNum("EC834D82") };
         shader_hashes_Fog.pixel_shaders.emplace(std::stoul("FC0B307B", nullptr, 16));
      }

      game = new Bioshock2();
   }

   CoreMain(hModule, ul_reason_for_call, lpReserved);

   return TRUE;
}