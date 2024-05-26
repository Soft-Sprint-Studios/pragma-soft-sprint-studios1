import scripts.shared
from sys import platform

scripts.shared.init_global_vars()
from scripts.shared import *

curDir = os.getcwd()

external_libs_dir = curDir +"/external_libs"
os.chdir(external_libs_dir)
get_submodule("alsoundsystem","https://github.com/Silverlan/alsoundsystem.git","74da07bd3c40a41dc9f8fb302815e9375653f393","feat/cxx_modules")
get_submodule("datasystem","https://github.com/Silverlan/datasystem.git","44219f77b46a9e3ba6aa96e891c72e86c9b16c0e","feat/cxx_modules")
get_submodule("iglfw","https://github.com/Silverlan/iglfw.git","2644d7184bf468676ff184aabff30c35740b79b9","feat/cxx_modules")
get_submodule("luasystem","https://github.com/Silverlan/luasystem.git","b26d26fd1cbf30a8137d95fb5c373bb6eb3076c7","feat/cxx_modules")
get_submodule("materialsystem","https://github.com/Silverlan/materialsystem.git","15975559fa01e89533b836e720532df736e5aa36","feat/cxx_modules")
get_submodule("mathutil","https://github.com/Silverlan/mathutil.git","923b01eec3df0a3e330a7662aff91a3bc03f6f5e","feat/cxx_modules")
get_submodule("networkmanager","https://github.com/Silverlan/networkmanager.git","9f074d1bb5a8a8019e817cd8e04b122143eee99b","feat/cxx_modules")
get_submodule("panima","https://github.com/Silverlan/panima.git","a4cb0b5c961d7548e0a8f9f2729ce0fbefc121c1","feat/cxx_modules")
get_submodule("prosper","https://github.com/Silverlan/prosper.git","d8794054bcab5c527840439fcf70c46e4db63d52","feat/cxx_modules")
get_submodule("sharedutils","https://github.com/Silverlan/sharedutils.git","eb34bd98f572e642885769914299240fab1b8982","feat/cxx_modules")
get_submodule("util_bsp","https://github.com/Silverlan/util_bsp.git","cf0875a0b1d7c297660cd5529641dc4e97a2cc86","feat/cxx_modules")
get_submodule("util_formatted_text","https://github.com/Silverlan/util_formatted_text.git","f53cfef4381c4d55b48b14257598a19f87fc736f","feat/cxx_modules")
get_submodule("util_image","https://github.com/Silverlan/util_image.git","43341428237d88195a06136da6887b6830105b09","feat/cxx_modules")
get_submodule("util_pad","https://github.com/Silverlan/util_pad.git","938f5a7164ae46ec9287cebcc47a98227737623f","feat/cxx_modules")
get_submodule("util_pragma_doc","https://github.com/Silverlan/util_pragma_doc.git","85670d4cace12a333d03c1a023926ce990e50eaa","feat/cxx_modules")
get_submodule("util_smdmodel","https://github.com/Silverlan/util_smdmodel.git","a2df5aae57aa3687b234e703ff2b6ddd203e6504","feat/cxx_modules")
get_submodule("util_sound","https://github.com/Silverlan/util_sound.git","e3fb0368ba8f4d8fc0042c0492e7a4c03b8eee8e","feat/cxx_modules")
get_submodule("util_source2","https://github.com/Silverlan/util_source2.git","2ede071e00a50fca3b3d4b78023fdcdbe9e87109","feat/cxx_modules")
get_submodule("util_source_script","https://github.com/Silverlan/util_source_script.git","0013e8fbc8ae8c20932877f028a52741f7d2894c","feat/cxx_modules")
get_submodule("util_timeline_scene","https://github.com/Silverlan/util_timeline_scene.git","6e717d05b8d435d7b8deb4be1bdb0b5994a910ec","feat/cxx_modules")
get_submodule("util_udm","https://github.com/Silverlan/util_udm.git","7f0c812333eff138127a56f82fa27579b8473726","feat/cxx_modules")
get_submodule("util_versioned_archive","https://github.com/Silverlan/util_versioned_archive.git","885b06cffdaf46e492c0011434036d6036f79957","feat/cxx_modules")
get_submodule("util_vmf","https://github.com/Silverlan/util_vmf.git","cd97e47cbfdc4fc65e52bc6cd172ffcc6bd850c3","feat/cxx_modules")
get_submodule("util_zip","https://github.com/Silverlan/util_zip.git","fef8bb16b34f3644f3354506578cbc2ed2338533","feat/cxx_modules")
get_submodule("vfilesystem","https://github.com/Silverlan/vfilesystem.git","a233c7f5d44dc3e39d7de6765bff0efe4c09122b","feat/cxx_modules")
get_submodule("wgui","https://github.com/Silverlan/wgui.git","ec346af89895860e0603cc1855f04d81dbe822b7","feat/cxx_modules")
get_submodule("util_unicode","https://github.com/Silverlan/util_unicode.git","09267d38c8485976de299eb3e8bf46eb0ad3bb5f","feat/cxx_modules")

os.chdir(curDir)
