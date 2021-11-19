include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY git@github.com:google/googletest.git
	GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # refs/tags/release-1.11.0
)

set(INSTALL_GTEST OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(googletest)
