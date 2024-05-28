import scripts.shared
from sys import platform

scripts.shared.init_global_vars()
from scripts.shared import *

curDir = os.getcwd()

modules_dir = curDir +"/modules"
os.chdir(modules_dir)
get_submodule("interfaces","https://github.com/Silverlan/pragma_interfaces.git","a9bb5d6117bc6f7a812ff72a1844d6400808e5fc","feat/cxx_modules")
get_submodule("pr_audio_dummy","https://github.com/Silverlan/pr_audio_dummy.git","f944362b82ef8d1c80c1ae2fe18785eb92ad32fe")
get_submodule("pr_curl","https://github.com/Silverlan/pr_curl.git","b5834184b8e2b26f601ba9fcd2bf28543e49c581","feat/cxx_modules")
get_submodule("pr_prosper_opengl","https://github.com/Silverlan/pr_prosper_opengl.git","a268347")
get_submodule("pr_prosper_vulkan","https://github.com/Silverlan/pr_prosper_vulkan.git","4e1f498386511a1370b4da1d1f466d7784b03cba")

os.chdir(curDir)
