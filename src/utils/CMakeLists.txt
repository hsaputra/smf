add_library (smf_utils STATIC
  ${PROJECT_SOURCE_DIR}/src/utils/random.cc
  ${PROJECT_SOURCE_DIR}/src/utils/checks/cpu.cc
  ${PROJECT_SOURCE_DIR}/src/utils/checks/memory.cc
  ${PROJECT_SOURCE_DIR}/src/utils/checks/disk.cc
  )
target_link_libraries (smf_utils
  ${SEASTAR_LDFLAGS}
  xxhash_static
  zstd.a
  )
install(TARGETS smf_utils DESTINATION lib)
