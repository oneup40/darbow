add_executable(test-read-source TestReadSource.cc)
target_link_libraries(test-read-source darbow)

add_executable(test-map-utf8-source TestMapUTF8Source.cc)
target_link_libraries(test-map-utf8-source darbow)
