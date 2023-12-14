perf stat -x ',' -I 5000 -e \
instructions:u,\
cycles:u,\
dTLB-loads:u,\
dtlb_load_misses.pde_cache_miss:u,\
dtlb_load_misses.miss_causes_a_walk:u,\
dtlb_load_misses.stlb_hit_2m:u,\
dtlb_store_misses.stlb_hit_4k:u,\
page_walker_loads.dtlb_l1:u,\
page_walker_loads.dtlb_l2:u,\
page_walker_loads.dtlb_l3:u,\
page_walker_loads.dtlb_memory:u \
./test_prefetcher 9
