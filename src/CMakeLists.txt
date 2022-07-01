add_library(XE_ENV_LIBRARY STATIC InternalArray.c FileStream.c Memory.c PathTraits.c Threading.c System.c Time.c PyWrapper.c Logger.c CLI.c CLICommands.c StringUtils.c Stack.c SubSystemCustomizer.c LibLoader.c Error.c)
#set_property(TARGET XE_ENV_LIBRARY PROPERTY C_STANDARD 23)

target_include_directories(XE_ENV_LIBRARY PUBLIC "${PROJECT_SOURCE_DIR}/include/" ${BNFT_INCLUDE_DIRS})
target_link_libraries(XE_ENV_LIBRARY ${Python_LIBRARIES})
