#pragma once

#include "VulkanBase.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"

#include <glm/glm.hpp>

namespace FTMGui {

	typedef uint32_t UsageFlags;
	typedef uint32_t MemoryFlags;

	constexpr UsageFlags USAGE_VERTEX_BUFFER_BIT = 0b00000000000000000000000000000001;
	constexpr UsageFlags USAGE_TRANSFER_SRC_BIT  = 0b00000000000000000000000000000010;
	constexpr UsageFlags USAGE_TRANSFER_DST_BIT  = 0b00000000000000000000000000000100;

	constexpr MemoryFlags MEMORY_DEVICE_LOCAL_BIT  = 0b00000000000000000000000000000001;
	constexpr MemoryFlags MEMORY_HOST_VISIBLE_BIT  = 0b00000000000000000000000000000010;
	constexpr MemoryFlags MEMORY_HOST_COHERENT_BIT = 0b00000000000000000000000000000100;


	#define STAGING_BUFFER_USAGE				USAGE_TRANSFER_SRC_BIT
	#define VERTEX_BUFFER_STAGING_BUFFER_USAGE  USAGE_TRANSFER_DST_BIT | USAGE_VERTEX_BUFFER_BIT

	#define STAGING_BUFFER_MEMORY MEMORY_HOST_VISIBLE_BIT | MEMORY_HOST_COHERENT_BIT
	#define VERTEX_BUFFER_STAGING_BUFFER_MEMORY MEMORY_DEVICE_LOCAL_BIT
	
	static uint32_t FindMemoryIndex(uint32_t TypeFilter,
							 VkMemoryPropertyFlags Properties,
							 const VulkanPhysicalDevice& PhysicalDevice)
	{
		VkPhysicalDeviceMemoryProperties MemProperties;
		vkGetPhysicalDeviceMemoryProperties(PhysicalDevice.Get(), &MemProperties);

		for (uint32_t i = 0; i < MemProperties.memoryTypeCount; i++)
		{
			if ((TypeFilter & (1 << i)) && (MemProperties.memoryTypes[i].propertyFlags & Properties) == Properties)
				return i;
		}

		FTMGUI_ASSERT(false, "Memory index not supported on device");

		return 0;
	}

	//TODO more to add
	enum class Attribute
	{
		FLOAT, FLOAT_2, FLOAT_3, FLOAT_4
	};

	struct VulkanBufferDescription
	{
		Ref<VulkanDevice> LogicalDevice;
		VulkanPhysicalDevice PhysicalDevice;
		
		std::vector<Attribute> Attributes;

		VulkanBufferDescription() {};
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	class VulkanBuffer
	{
	public:
		VulkanBuffer(const VulkanBufferDescription& Description,
			UsageFlags Flags, MemoryFlags MemFlags,
			size_t ByteSize, const void* Data = nullptr);

		~VulkanBuffer();


		void SetData(const void* Data, size_t ByteSize, size_t Offset)
		{
			if (Data)
			{
				void* Memory = nullptr;
				vkCall(vkMapMemory, m_VkDevice->GetHandle(), m_DeviceMemoryHandle, Offset, ByteSize, 0, &Memory);
				memcpy(Memory, Data, ByteSize);
				vkUnmapMemory(m_VkDevice->GetHandle(), m_DeviceMemoryHandle);
			}
		}

		inline void Bind(const VulkanCommandBuffer& CmdBuffer) const
		{
			VkBuffer Buffers[] = { m_BufferHandle };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(CmdBuffer.GetHandle(), 0, 1, Buffers, offsets);
		}

		inline const VkBuffer GetHandle() const { return m_BufferHandle; }
		inline const VkDeviceMemory GetMemoryHandle() const { return m_DeviceMemoryHandle; }

		inline const std::optional<VkVertexInputBindingDescription> GetBindingDescription() const
		{
			return m_BindingDesc.has_value() ? m_BindingDesc : std::nullopt;
		}

		inline const std::vector<VkVertexInputAttributeDescription>& GetInputDescription() const
		{
			return m_InputAttribDesc;
		}

	private:
		VkBuffer m_BufferHandle;
		VkDeviceMemory m_DeviceMemoryHandle;

		Ref<VulkanDevice> m_VkDevice;

		std::optional<VkVertexInputBindingDescription> m_BindingDesc;
		std::vector<VkVertexInputAttributeDescription> m_InputAttribDesc;
	};

}