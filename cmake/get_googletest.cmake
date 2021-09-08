# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")

include(FetchContent)

FetchContent_Populate(
    googletest
    # QUIET
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/googletest-build"
    USES_TERMINAL_DOWNLOAD
    ON
    USES_TERMINAL_UPDATE
    ON
    USES_TERMINAL_CONFIGURE
    ON
    USES_TERMINAL_BUILD
    ON
    USES_TERMINAL_INSTALL
    ON
    USES_TERMINAL_TEST
    ON)
