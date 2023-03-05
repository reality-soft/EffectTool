workspace "EffectTool"
	architecture "x86_64"
	toolset "v143"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
project "EffectTool"
	location "EffectTool"
	kind "WindowedApp"
	language "C++"

	targetdir("../output/bin/" .. outputdir .. "/%{prj.name}")
	objdir("../output/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"premake5.lua"
	}

	includedirs
	{
		"../reality_TeamProj/Engine/vendor/spdlog/include",
		"../reality_TeamProj/Engine/src",
		"../reality_TeamProj/Engine/src/Engine",
		"../reality_TeamProj/Engine/src/ECS",
		"../reality_TeamProj/Engine/src/Engine/SingletonClass",
		"../SDK/DirectXTK/include",
		"../SDK/FBXSDK/include",
		"../SDK/FMOD/include",
		"../SDK/IMGUI/include",
		"../SDK/RP3D/include"
	}

	libdirs
	{
		"../output/bin/Debug-windows-x86_64/Engine/",
		"../SDK/DirectXTK/lib",
		"../SDK/FBXSDK/lib/debug",
		"../SDK/FMOD/lib/debug",
		"../SDK/IMGUI/lib",
		"../SDK/RP3D/lib/debug"
	}

	links
	{
		"Engine",
		"libfbxsdk-md",
		"libxml2-md",
		"zlib-md",
		"fmod_vc",
		"fmodL_vc",
		"ImGui_Win32_Dx11_D",
		"reactphysics3d"
	}

	filter "files:**VS.hlsl"
		shadertype "Vertex"
		shaderentry "VS"
	    shadermodel "5.0"

	filter "files:**PS.hlsl"
	    shadertype "Pixel"
		shaderentry "PS"
	    shadermodel "5.0"
		
	filter "files:**GS.hlsl"
	    shadertype "Geometry"
		shaderentry "GS"
	    shadermodel "5.0"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"
		runtime "Debug"

		defines
		{
			"PLATFORM_WINDOWS"
		}

		prebuildcommands
		{
			"copy \"..\\..\\output\\bin\\Debug-windows-x86_64\\Engine\\*.dll\" \"..\\..\\output\\bin\\Debug-windows-x86_64\\EffectTool\\*.dll\""
		}

		postbuildcommands
		{
			"copy \"..\\..\\output\\bin\\Debug-windows-x86_64\\EffectTool\\*.cso\" \"..\\..\\Contents\\Shader\\*.cso\"",
			"copy \"..\\..\\output\\bin\\Release-windows-x86_64\\EffectTool\\*.cso\" \"..\\..\\Contents\\Shader\\*.cso\""
		}

	filter "configurations:Debug"
		defines "_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "_DIST"
		optimize "On"
