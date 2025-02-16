#pragma once

#include <Eigen/Dense>

#include "Registry/NiNode/Node.h"

namespace Registry::Collision
{
	using Schlong = Node::NodeData::SchlongData;

	struct RotationData
	{
		static inline constexpr float fMaxAngleStepSize = 0.035f;
		RotationData(std::shared_ptr<Schlong> a_schlong, const RE::NiPoint3& a_ideal) :
			schlong(a_schlong) { Update(a_ideal); }

		void ApplyRotation();
		void Update(const RE::NiPoint3& a_newIdeal);

	private:
		std::shared_ptr<Schlong> schlong;
		Eigen::Vector3f vIdeal;

	public:
		bool operator==(const RotationData& a_rhs) const { return this->schlong == a_rhs.schlong; }
		bool operator==(const std::shared_ptr<Schlong>& a_rhs) const { return this->schlong == a_rhs; }
	};

	struct NodeUpdate
	{
		static void Install();

		static void AddOrUpdateSkew(const std::shared_ptr<Schlong>& a_node, const RE::NiPoint3& vIdeal);
		static void DeleteSkew(const std::shared_ptr<Schlong>& a_node);
		static void DeleteSkews()
		{
			std::scoped_lock lk{ _m };
			skews.clear();
		}

	private:
		friend void stl::write_thunk_call<NodeUpdate>(std::uintptr_t);
		static void thunk(RE::NiAVObject* a_obj, RE::NiUpdateData* updateData);
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 5 };

		static inline std::mutex _m{};
		static inline std::vector<RotationData> skews{};
	};

}	 // namespace Registry::Collision
