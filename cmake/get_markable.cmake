# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")

include(FetchContent)

FetchContent_Populate(
    markable
    # QUIET
    GIT_REPOSITORY https://github.com/akrzemi1/markable.git
    GIT_TAG origin/master
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/markable-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/markable-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/markable-build"
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
