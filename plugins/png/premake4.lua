newoption {
	trigger			= "with-png",
	description		= "Enable the Minko PNG plugin."
}

PROJECT_NAME = path.getname(os.getcwd())

minko.project.library("minko-plugin-" .. PROJECT_NAME)
	kind "StaticLib"
	language "C++"
	files { "**.hpp", "**.h", "**.cpp", "**.c", "include/**.hpp" }
	includedirs {
		"include",
		"src",
		"lib/lodepng/src"
	}
	defines
	{
		--"LODEPNG_NO_COMPILE_ENCODER"
		--"LODEPNG_NO_COMPILE_DISK"
		"_CRT_SECURE_NO_WARNINGS"
	}
