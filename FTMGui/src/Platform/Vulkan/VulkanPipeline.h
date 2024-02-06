#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"

#include <filesystem>
#include <unordered_map>



namespace FTMGui {

	enum class ShaderType
	{
		VertexShader = 0, FragmentShader
	};

	class VulkanPipeline
	{
	public:
		VulkanPipeline(const Ref<VulkanDevice>& vkDevice, 
					   const VulkanSwapchain& Swapchain,
					   const VulkanRenderPass& RenderPass,
					   const std::unordered_map<ShaderType, std::filesystem::path>& ShaderPaths);

		~VulkanPipeline();

		inline const VkPipeline GetHandle() const { return m_Pipeline; }


	private:
		VkShaderModule CreateShaderModule(const std::filesystem::path& path);

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		Ref<VulkanDevice> m_VkDevice;
		std::vector<VkPipelineShaderStageCreateInfo> m_PipelineStagesCreateInfos;

	};
}