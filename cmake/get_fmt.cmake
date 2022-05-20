# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")

include(FetchContent)

FetchContent_Populate(
    fmt
    # QUIET
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG b6f4ceaed0a0a24ccf575fab6c56dd50ccf6f1a9 # 8.1.1
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/fmt-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/fmt-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/fmt-build"
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
