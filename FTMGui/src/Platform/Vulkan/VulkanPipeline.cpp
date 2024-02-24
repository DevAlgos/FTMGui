#include "VulkanPipeline.h"

#include "Utils/File.h"


namespace FTMGui {
	
	VulkanPipeline::VulkanPipeline(const Ref<VulkanDevice>& vkDevice, 
								   const VulkanSwapchain& Swapchain,
								   const VulkanRenderPass& RenderPass,
								   const std::unordered_map<ShaderType, std::filesystem::path>& ShaderPaths,
								   VulkanBuffer* VertexBuffer, uint32_t nBuffers)
		: m_Pipeline(nullptr), m_PipelineLayout(nullptr), m_VkDevice(vkDevice)
	{
		std::vector<VkShaderModule> ShaderModules;

		for (auto& [Type, Path] : ShaderPaths)
		{
			ShaderModules.push_back(CreateShaderModule(Path));

			VkPipelineShaderStageCreateInfo  CreateInfo{};
			CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

			switch (Type)
			{
			case ShaderType::VertexShader: CreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; break;
			case ShaderType::FragmentShader: CreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
			default:
				FTMGUI_ASSERT(false, "Invalid shader type ", (int)Type);
				break;
			}
			
			CreateInfo.module = ShaderModules.back();
			CreateInfo.pName = "main";
			CreateInfo.pSpecializationInfo = nullptr; //TODO come back to this may be useful

			m_PipelineStagesCreateInfos.push_back(CreateInfo);
		}

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo DynamicState{};
		DynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		DynamicState.dynamicStateCount = (uint32_t)(dynamicStates.size());
		DynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo ViewportState{};
		ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		ViewportState.viewportCount = 1;
		ViewportState.scissorCount = 1;

		VkPipelineVertexInputStateCreateInfo VertexInputInfo{};
		VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		VertexInputInfo.vertexBindingDescriptionCount = 0;
		VertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		VertexInputInfo.vertexAttributeDescriptionCount = 0;
		VertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		if (VertexBuffer)
		{
			m_BindingDesc.push_back({});
			m_AttribDesc.push_back({});

			auto& BindingDesc = m_BindingDesc.back();
			auto& AttribDesc = m_AttribDesc.back();

			for (size_t i = 0; i < nBuffers; i++)
			{
				BindingDesc.push_back(VertexBuffer[i].GetBindingDescription().value());

				for(auto& Desc : VertexBuffer[i].GetInputDescription())
					AttribDesc.push_back(Desc);
			}
			
			VertexInputInfo.vertexBindingDescriptionCount = (uint32_t)BindingDesc.size();
			VertexInputInfo.pVertexBindingDescriptions = BindingDesc.data();
			
			VertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)AttribDesc.size();
			VertexInputInfo.pVertexAttributeDescriptions = AttribDesc.data(); 

		}

		VkPipelineInputAssemblyStateCreateInfo InputAssembly{};
		InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		InputAssembly.primitiveRestartEnable = VK_FALSE;

		//TODO come back may want multiple viewports.
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)Swapchain.GetImageExtent().width;
		viewport.height = (float)Swapchain.GetImageExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Swapchain.GetImageExtent();

		VkPipelineRasterizationStateCreateInfo Rasterizer{};
		Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		Rasterizer.depthClampEnable = VK_FALSE;
		Rasterizer.rasterizerDiscardEnable = VK_FALSE;
		Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		Rasterizer.lineWidth = 1.0f;
		Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		Rasterizer.depthBiasEnable = VK_FALSE;
		Rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		Rasterizer.depthBiasClamp = 0.0f; // Optional
		Rasterizer.depthBiasSlopeFactor = 0.0f; // Optional


		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional


		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		vkCall(vkCreatePipelineLayout, m_VkDevice->GetHandle(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout);


		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = m_PipelineStagesCreateInfos.data();
		pipelineInfo.pVertexInputState = &VertexInputInfo;
		pipelineInfo.pInputAssemblyState = &InputAssembly;
		pipelineInfo.pViewportState = &ViewportState;
		pipelineInfo.pRasterizationState = &Rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &DynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = RenderPass.GetHandle();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = nullptr; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		vkCall(vkCreateGraphicsPipelines, m_VkDevice->GetHandle(), nullptr, 1, &pipelineInfo, nullptr, &m_Pipeline);

		for (auto Module : ShaderModules)
			vkDestroyShaderModule(m_VkDevice->GetHandle(), Module, nullptr);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		if (m_PipelineLayout && m_Pipeline)
		{
			vkDestroyPipelineLayout(m_VkDevice->GetHandle(), m_PipelineLayout, nullptr);
			vkDestroyPipeline(m_VkDevice->GetHandle(), m_Pipeline, nullptr);
		}
		
	}

	VkShaderModule VulkanPipeline::CreateShaderModule(const std::filesystem::path& path)
	{
		VkShaderModule Module{};

		auto ShaderFile = FileReader::ReadAlignedBinaryFile(path);

		VkShaderModuleCreateInfo CreateInfo{};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		CreateInfo.codeSize = ShaderFile.size();
		CreateInfo.pCode = reinterpret_cast<const uint32_t*>(ShaderFile.data());
		
		vkCall(vkCreateShaderModule, m_VkDevice->GetHandle(), &CreateInfo, nullptr, &Module);

		return Module;
	}

}