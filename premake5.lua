solution "VT"
configurations { "Debug", "Release" }
language "C++"

floatingpoint "Fast"

configuration { "Debug" }
defines { "DEBUG=1", "RELEASE=0" }
symbols "On"
targetdir "target/Debug"

configuration { "Release*" }
defines { "DEBUG=0", "RELEASE=1" }
optimize "Speed"
targetdir "target/Release"

configuration {}

project "vstgui-tests"
kind "WindowedApp"
targetname "VstguiTests"

flags
{
   "StaticRuntime"
}

includedirs
{
   "deps/vstgui.surge"
}

defines
{
   "MAC=1"
}

buildoptions
{
   "-std=c++17",
   "-stdlib=libc++",
   "-ferror-limit=0",
}

files
{
   "src/main.cpp",

   -- a bit sloppy - deps are *ed in
   "deps/vstgui.surge/vstgui/vstgui_mac.mm",
   "deps/vstgui.surge/vstgui/vstgui_uidescription_mac.mm",

   "deps/vstgui.surge/vstgui/standalone/source/*cpp",
   "deps/vstgui.surge/vstgui/standalone/source/helpers/*cpp",
   "deps/vstgui.surge/vstgui/standalone/source/platform/mac/*cpp",
   "deps/vstgui.surge/vstgui/standalone/source/platform/mac/*mm",

   -- This one should be sucked in but isn't so make it explicit
   "deps/vstgui.surge/vstgui/uidescription/compresseduidescription.cpp"

}

links
{
   "Accelerate.framework",
   "ApplicationServices.framework",
   "AudioUnit.framework",
   "Carbon.framework",
   "CoreAudio.framework",
   "CoreAudioKit.framework",
   "CoreServices.framework",
   "CoreText.framework",
   "Cocoa.framework",
   "CoreFoundation.framework",
   "OpenGL.framework",
   "QuartzCore.framework",
}
