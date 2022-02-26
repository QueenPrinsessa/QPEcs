workspace "QPEcs"
    architecture "x64"
    startproject "QPEcs"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group ""

include "QPEcs"
