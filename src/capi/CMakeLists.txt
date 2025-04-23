target_sources(libmahjong
    PRIVATE
        capi.cc
    PUBLIC
        FILE_SET HEADERS
        FILES
            capi.h
)