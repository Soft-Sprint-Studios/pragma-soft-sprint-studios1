function(add_module_references TARGET_NAME MODULE_LIST)
  string(REPLACE ";" "\\\;" MODULE_LIST_STR "${MODULE_LIST}")
  target_compile_options(${TARGET_NAME} PRIVATE /reference ${MODULE_LIST_STR})
endfunction()
