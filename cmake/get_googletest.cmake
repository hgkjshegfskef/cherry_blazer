# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR
# * CHERRY_BLAZER_CI

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")
message(DEBUG "CHERRY_BLAZER_CI: '${CHERRY_BLAZER_CI}'")

if(CHERRY_BLAZER_CI)
    set(quiet "")
else()
    set(quiet QUIET)
endif()

include(FetchContent)

message(STATUS "Downloading googletest...")

FetchContent_Populate(
    googletest
    ${quiet}
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # 1.11.0
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-build")
