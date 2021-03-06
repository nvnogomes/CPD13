include_directories(
    ${PROJECT_SOURCE_DIR}
    )

add_executable( __sizeof sizeof.cpp )      
add_executable( test1 test1.cpp )  
add_executable( test2 test2.cpp )
add_executable( test3 test3.cpp )
add_executable( test3b test3b.cpp )
add_executable( test4 test4.cpp )
add_executable( test5 test5.cpp )
add_executable( test6 test6.cpp )
add_executable( test7 test7.cpp )
add_executable( perf_test1 perf_test1.cpp )
add_executable( perf_test2 perf_test2.cpp )
add_executable( test_accelerator test_accelerator.cpp )
add_executable( test_accelerator2 test_accelerator2.cpp )
add_executable( test_accelerator3 test_accelerator3.cpp )
add_executable( test_accelerator_farm+pipe test_accelerator_farm+pipe.cpp )
add_executable( test_accelerator_pipe test_accelerator_pipe.cpp )
add_executable( test_accelerator_pipe+farm test_accelerator_pipe+farm.cpp )
add_executable( test_freeze test_freeze.cpp )
add_executable( test_masterworker test_masterworker.cpp )
add_executable( test_multi_masterworker test_multi_masterworker.cpp )
add_executable( test_pipe+masterworker test_pipe+masterworker.cpp )
add_executable( test_scheduling test_scheduling.cpp )
add_executable( test_dt test_dt.cpp )
add_executable( perf_test_alloc1 perf_test_alloc1.cpp )
add_executable( perf_test_alloc2 perf_test_alloc2.cpp )
add_executable( perf_test_alloc3 perf_test_alloc3.cpp )
add_executable( perf_test_noalloc perf_test_noalloc.cpp )
add_executable( test_uBuffer test_uBuffer.cpp )
add_executable( test_torus test_torus.cpp )
add_executable( test_map test_map.cpp )
add_executable( test_map2 test_map2.cpp )
add_executable( test_mapreduce test_mapreduce.cpp) 
add_executable( test_spinBarrier test_spinBarrier.cpp )
add_executable( test_multi_input test_multi_input.cpp )
add_executable( test_dotprod test_dotprod.cpp )
add_executable( test_ofarm test_ofarm.cpp )
add_executable( test_accelerator_ofarm test_accelerator_ofarm.cpp )
add_executable( test_accelerator_ofarm_multiple_freezing test_accelerator_ofarm_multiple_freezing.cpp )

#if (NOT(GCC_TOO_OLD))
#  add_executable( test_mpmc test_mpmc.cpp)
#  if (NOT(CMAKE_SYSTEM_NAME MATCHES "Windows"))
#    SET_TARGET_PROPERTIES(test_mpmc PROPERTIES COMPILE_FLAGS " -Wno-strict-aliasing -O3  -finline-functions ")
#  endif (NOT(CMAKE_SYSTEM_NAME MATCHES "Windows"))
#endif (NOT(GCC_TOO_OLD))
 

set( TESTS test1 test2 test3 test3b test4 test5 test6 test7
perf_test1 perf_test2 test_accelerator test_accelerator2 test_accelerator3
test_accelerator_pipe+farm test_accelerator_farm+pipe test_accelerator_pipe
test_freeze test_masterworker test_multi_masterworker test_pipe+masterworker
test_scheduling test_dt perf_test_alloc1 perf_test_alloc2
perf_test_alloc3 perf_test_noalloc test_uBuffer test_torus
test_map test_map2 test_mapreduce test_dotprod 
test_spinBarrier test_multi_input test_ofarm test_accelerator_ofarm
test_accelerator_ofarm_multiple_freezing )
#if (NOT(GCC_TOO_OLD))
#  set(TESTS ${TESTS}  test_mpmc)
#endif (NOT(GCC_TOO_OLD))

foreach( t ${TESTS} )
    target_link_libraries( ${t} ${CMAKE_THREAD_LIBS_INIT} )
endforeach( t )

add_test( test1 ${CMAKE_CURRENT_BINARY_DIR}/test1 3 1000 )
add_test( test2 ${CMAKE_CURRENT_BINARY_DIR}/test2 3 1000 )
add_test( test3 ${CMAKE_CURRENT_BINARY_DIR}/test3 1000 )
add_test( test3b ${CMAKE_CURRENT_BINARY_DIR}/test3b 0 1000 0 1 2 )
add_test( test4 ${CMAKE_CURRENT_BINARY_DIR}/test4 1000 3 )
add_test( test5 ${CMAKE_CURRENT_BINARY_DIR}/test5 1000 )
add_test( test6 ${CMAKE_CURRENT_BINARY_DIR}/test6 1000 )
add_test( test7 ${CMAKE_CURRENT_BINARY_DIR}/test7 1000 )
add_test( test_accelerator ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator 3 1000 )
add_test( test_accelerator2 ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator2 3 1000 )
add_test( test_accelerator3 ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator3 3 1000 )
add_test( test_accelerator_farm+pipe ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator_farm+pipe 1000 3 3 )
add_test( perf_test1 ${CMAKE_CURRENT_BINARY_DIR}/perf_test1 3 100 1 10 1000 )
add_test( perf_test2 ${CMAKE_CURRENT_BINARY_DIR}/perf_test2 3 100 1 10 1000 )
add_test( perf_test_alloc1 ${CMAKE_CURRENT_BINARY_DIR}/perf_test_alloc1 3 100 1 10 1000 )
add_test( perf_test_alloc2 ${CMAKE_CURRENT_BINARY_DIR}/perf_test_alloc2 3 100 1 10 1000 )
#add_test( perf_test_alloc3 ${CMAKE_CURRENT_BINARY_DIR}/perf_test_alloc3 1000000 80 2 ) # to be fixed in win
add_test( perf_test_noalloc ${CMAKE_CURRENT_BINARY_DIR}/perf_test_noalloc 1000000 1000 2 100000 )
add_test( test_freeze ${CMAKE_CURRENT_BINARY_DIR}/test_freeze 3 1000 10 )
add_test( test_dt ${CMAKE_CURRENT_BINARY_DIR}/test_dt 3 50 )
add_test( test_uBuffer ${CMAKE_CURRENT_BINARY_DIR}/test_uBuffer 1024 1000000 0 0)
add_test( test_scheduling ${CMAKE_CURRENT_BINARY_DIR}/test_scheduling 3 10 )
add_test( test_masterworker ${CMAKE_CURRENT_BINARY_DIR}/test_masterworker 3 1000 10 )
add_test( test_multi_masterworker ${CMAKE_CURRENT_BINARY_DIR}/test_multi_masterworker 3 1000 )
add_test( test_pipe+masterworker ${CMAKE_CURRENT_BINARY_DIR}/test_pipe+masterworker 2 3 1000 )
add_test( test_accelerator_pipe ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator_pipe 3 1000 )
add_test( test_accelerator_pipe+farm ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator_pipe+farm 1000 3 )
add_test( test_torus ${CMAKE_CURRENT_BINARY_DIR}/test_torus 4 1000 )
#if (NOT(GCC_TOO_OLD))
#  add_test( test_mpmc ${CMAKE_CURRENT_BINARY_DIR}/test_mpmc 10000 3 5 )
#endif (NOT(GCC_TOO_OLD))
add_test( test_map ${CMAKE_CURRENT_BINARY_DIR}/test_map 65536 3 )
add_test( test_map2 ${CMAKE_CURRENT_BINARY_DIR}/test_map2 65536 3 10 )
add_test( test_spinBarrier ${CMAKE_CURRENT_BINARY_DIR}/test_spinBarrier 3 100)
add_test( test_multi_input ${CMAKE_CURRENT_BINARY_DIR}/test_multi_input 3 10000 )
add_test( test_mapreduce ${CMAKE_CURRENT_BINARY_DIR}/test_mapreduce 1000000 3 )
add_test( test_dotprod ${CMAKE_CURRENT_BINARY_DIR}/test_dotprod 10485760 3 )
add_test( test_ofarm ${CMAKE_CURRENT_BINARY_DIR}/test_ofarm 3 1000 )
add_test( test_accelerator_ofarm ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator_ofarm 3 1000 )
add_test( test_accelerator_ofarm_multiple_freezing ${CMAKE_CURRENT_BINARY_DIR}/test_accelerator_ofarm_multiple_freezing 3 1000 )

#layer2 tests

add_subdirectory( layer2-tests-HAL )

# Distributed

find_package(ZeroMQ)
if(NOT ZeroMQ_FOUND)
  message (WARNING " 0mq not found: cannot configure distributed mode")
else(NOT ZeroMQ_FOUND)
  add_subdirectory( d )
endif(NOT ZeroMQ_FOUND)


