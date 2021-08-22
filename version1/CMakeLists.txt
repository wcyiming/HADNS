cmake_minimum_required(VERSION 3.17)
project(hadns C)

set(CMAKE_C_STANDARD 11)

add_executable(
dns 
dns.h dns.c
global.h
hashlist.c hashlist.h
hashmap.c hashmap.h
log.c log.h
LRU.c LRU.h
operator.c
)