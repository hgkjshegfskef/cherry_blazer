# Intended to be run in script mode with cmake -P.

# Expected defined variables:
#
# * CHERRY_BLAZER_SOURCE_DIR
# * CHERRY_BLAZER_BOOST_VERSION
# * CHERRY_BLAZER_BOOST_LIBRARIES

message(DEBUG "CHERRY_BLAZER_SOURCE_DIR: '${CHERRY_BLAZER_SOURCE_DIR}'")
message(DEBUG "CHERRY_BLAZER_BOOST_VERSION: '${CHERRY_BLAZER_BOOST_VERSION}'")

include(ProcessorCount)
ProcessorCount(n)
if(n EQUAL 0)
    set(submodule_fetchJobs "")
else()
    set(submodule_fetchJobs "submodule.fetchJobs=${n}")
endif()

include(FetchContent)
FetchContent_Populate(
    boost
    # QUIET
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG "boost-${CHERRY_BLAZER_BOOST_VERSION}"
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    GIT_CONFIG advice.detachedHead=false ${submodule_fetchJobs}
    SUBBUILD_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost-subbuild"
    SOURCE_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost-src"
    BINARY_DIR "${CHERRY_BLAZER_SOURCE_DIR}/_deps/boost-build"
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
