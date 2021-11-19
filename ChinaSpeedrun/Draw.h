#pragma once

#include "Mathf.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace cs
{
	class Mesh;

	struct DrawItem
	{
		Mesh* mesh;
		std::vector<VkDescriptorSet*> descriptorSetsRef;

		DrawItem();

		void SetDescriptorRefs(std::vector<VkDescriptorSet>& sets);
	};

	class Draw
	{
	public:
		friend class VulkanEngineRenderer;

		static void Setup();

		static void Update();

		static void DebugGrid();
		static void Line(const std::vector<Vector3>& points, const std::vector<class Color>& colors);
		static void Rectangle(const Vector3& extents, const Vector3& position);
		static void Circle(const float& radius, const Vector3& position, const Color& color, const uint32_t& resolution = 16);
		static void Image(const class Texture* texture, const Vector3& position, const Vector3& size = Vector3(1.0f));
		static void VulkanDraw(VkCommandBuffer& commandBuffer, const size_t& index, VkBuffer& vertexBuffer, VkBuffer& indexBuffer);
		static void CreateDescriptorSets();

	private:
		static class Material* material;
		static bool updateVertexIndexBuffers;
		static std::vector<DrawItem> debugItems;
		static VkDescriptorPool descriptorPool;
		static std::vector<VkDescriptorSet> descriptorSets;
		static struct UniformBufferObject ubo;
	};
}
