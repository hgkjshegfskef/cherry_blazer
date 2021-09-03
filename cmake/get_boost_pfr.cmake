# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")

include(FetchContent)

FetchContent_Populate(
    boost_pfr
    QUIET
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG da12b52759ea52c3e2341690f208fea898643bbb # 1.77.0
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost_pfr-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost_pfr-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost_pfr-build")
