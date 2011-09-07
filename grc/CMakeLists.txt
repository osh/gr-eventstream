file(GLOB xml_files "*.xml")
set(GRC_BLOCKS_DIR      ${GR_PKG_DATA_DIR}/grc/blocks)
install(FILES ${xml_files}
    es_patterned_interleaver.xml DESTINATION ${grcdir} COMPONENT "eventstream_grc")

