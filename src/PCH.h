#pragma once

#pragma warning(push)
#pragma warning(disable : 4200)
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#pragma warning(pop)

#include <unordered_map>
#include <atomic>
#include <yaml-cpp/yaml.h>

#pragma warning(push)
#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

namespace logger = SKSE::log;
namespace fs = std::filesystem;
using namespace std::literals;

// #include "Random.h"
#include "GameForms.h"
#include "Registry/Misc.h"
#include "Script.h"
#include "Singleton.h"
#include "UserData/Settings.h"

#define ESPNAME "SexLab.esm"
static constexpr auto CONFIGPATH = [](std::string file) -> std::string { return "Data\\SKSE\\SexLab\\"s + file; };

#ifndef NDEBUG
	template <class T>
	void ASSERTLOWERCASE(T string) { assert(std::find_if_not(string.begin(), string.end(), ::islower) == string.end()); }
#else
#define ASSERTLOWERCASE(expression) ((void)0)
#endif

	namespace stl
	{
		using namespace SKSE::stl;

		constexpr std::uint32_t version_pack(REL::Version a_version) noexcept
		{
			return static_cast<std::uint32_t>(
				(a_version[0] & 0x0FF) << 24u |
				(a_version[1] & 0x0FF) << 16u |
				(a_version[2] & 0xFFF) << 4u |
				(a_version[3] & 0x00F) << 0u);
		}

		template <class T>
		void write_thunk_call(std::uintptr_t a_src)
		{
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(14);

			T::func = trampoline.write_call<5>(a_src, T::thunk);
		}

		template <class F, class T>
		void write_vfunc()
		{
			REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
			T::func = vtbl.write_vfunc(T::size, T::thunk);
		}
}

namespace Papyrus
{
#define REGISTERFUNC(func, class, dont_delay) a_vm->RegisterFunction(#func##sv, class, func, dont_delay)

	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
}

#define DLLEXPORT __declspec(dllexport)

#include "Plugin.h"
