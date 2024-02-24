#include "VulkanBuffer.h"

namespace FTMGui {
	VulkanBuffer::VulkanBuffer(const VulkanBufferDescription& Description, 
							   UsageFlags Flags, MemoryFlags MemFlags, 
							   size_t ByteSize, const void* Data)
		: m_BufferHandle(nullptr), 
		  m_DeviceMemoryHandle(nullptr), 
		  m_VkDevice(Description.LogicalDevice),
		  m_BindingDesc(std::nullopt)
	{
		VkBufferUsageFlags VulkanUsageFlags = 0;

		if (Flags & USAGE_VERTEX_BUFFER_BIT)
		{
			VulkanUsageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			m_BindingDesc = VkVertexInputBindingDescription();

			uint32_t i = 0;
			uint32_t CurrentOffset = 0;

			for (auto attrib : Description.Attributes)
			{
				VkVertexInputAttributeDescription AttribDesc{};
				AttribDesc.binding = 0;

				switch (attrib)
				{
				case Attribute::FLOAT:
					AttribDesc.location = i;
					AttribDesc.format = VK_FORMAT_R32_SFLOAT;
					AttribDesc.offset = CurrentOffset;
					CurrentOffset += sizeof(float);
					break;
				case Attribute::FLOAT_2:
					AttribDesc.location = i;
					AttribDesc.format = VK_FORMAT_R32G32_SFLOAT;
					AttribDesc.offset = CurrentOffset;
					CurrentOffset += sizeof(float) * 2;
					break;
				case Attribute::FLOAT_3:
					AttribDesc.location = i;
					AttribDesc.format = VK_FORMAT_R32G32B32_SFLOAT;
					AttribDesc.offset = CurrentOffset;
					CurrentOffset += sizeof(float) * 3;
					break;
				case Attribute::FLOAT_4:
					AttribDesc.location = i;
					AttribDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
					AttribDesc.offset = CurrentOffset;
					CurrentOffset += sizeof(float) * 4;
					break;
				default:
					break;
				}

				m_InputAttribDesc.push_back(AttribDesc);

				i++;
			}

			m_BindingDesc.value().binding = 0;
			m_BindingDesc.value().stride = CurrentOffset;
			m_BindingDesc.value().inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		if (Flags & USAGE_TRANSFER_DST_BIT)
			VulkanUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		if (Flags & USAGE_TRANSFER_SRC_BIT)
			VulkanUsageFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VkMemoryPropertyFlags VulkanMemoryFlags = 0;

		if (MemFlags & MEMORY_DEVICE_LOCAL_BIT)
			VulkanMemoryFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		if (MemFlags & MEMORY_HOST_VISIBLE_BIT)
			VulkanMemoryFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

		if (MemFlags & MEMORY_HOST_COHERENT_BIT)
			VulkanMemoryFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBufferCreateInfo BufferCreateInfo{};
		BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		BufferCreateInfo.size = ByteSize;
		BufferCreateInfo.usage = VulkanUsageFlags;
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		vkCall(vkCreateBuffer, m_VkDevice->GetHandle(), &BufferCreateInfo, nullptr, &m_BufferHandle);

		VkMemoryRequirements MemRequirements;
		vkGetBufferMemoryRequirements(m_VkDevice->GetHandle(), m_BufferHandle, &MemRequirements);

		VkMemoryAllocateInfo MemoryAllocInfo{};
		MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		MemoryAllocInfo.allocationSize = MemRequirements.size;
		MemoryAllocInfo.memoryTypeIndex = FindMemoryIndex(MemRequirements.memoryTypeBits,
														  VulkanMemoryFlags, 
														  Description.PhysicalDevice);
						
		vkCall(vkAllocateMemory, m_VkDevice->GetHandle(), &MemoryAllocInfo, nullptr, &m_DeviceMemoryHandle);
	
		vkBindBufferMemory(m_VkDevice->GetHandle(), m_BufferHandle, m_DeviceMemoryHandle, 0);

		if (Data && !(MemFlags & MEMORY_DEVICE_LOCAL_BIT))
		{
			void* Memory = nullptr;
			vkMapMemory(m_VkDevice->GetHandle(), m_DeviceMemoryHandle, 0, ByteSize, 0, &Memory);
			memcpy(Memory, Data, ByteSize);
			vkUnmapMemory(m_VkDevice->GetHandle(), m_DeviceMemoryHandle);
		}
	}
	VulkanBuffer::~VulkanBuffer()
	{
		if (m_DeviceMemoryHandle)
			vkFreeMemory(m_VkDevice->GetHandle(), m_DeviceMemoryHandle, nullptr);

		if (m_BufferHandle)
			vkDestroyBuffer(m_VkDevice->GetHandle(), m_BufferHandle, nullptr);
	}
}