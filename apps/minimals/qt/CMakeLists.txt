#--------------------------------------------------------------------
# Inviwo Qt Application
ivw_project(inviwo_qtminimum)

find_package(Qt5Core QUIET REQUIRED)
find_package(Qt5Gui QUIET REQUIRED)
find_package(Qt5Widgets QUIET REQUIRED)

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    qtminimum.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})
        
#--------------------------------------------------------------------
# Define libraries that should be linked
ivw_retrieve_all_modules(package_list)
list(APPEND package_list InviwoCore)
list(APPEND package_list InviwoQtApplicationBase)

#--------------------------------------------------------------------
# Only include one OpenGL context creation module
list(REMOVE_ITEM package_list InviwoGLFWModule)
list(REMOVE_ITEM package_list InviwoGLUTModule)
list(REMOVE_ITEM package_list InviwoSDLModule)

#--------------------------------------------------------------------
# Remove modules not used by this app
list(REMOVE_ITEM package_list InviwoPython3QtModule)
list(REMOVE_ITEM package_list InviwoUnitTestsModule)

#--------------------------------------------------------------------
# Register the use of modules
ivw_register_use_of_modules(${package_list})

#--------------------------------------------------------------------
# Need to add dependent directories before creating application
ivw_add_dependency_directories(${package_list})

#--------------------------------------------------------------------
# Create application
add_executable(inviwo_qtminimum MACOSX_BUNDLE WIN32 ${SOURCE_FILES})

#--------------------------------------------------------------------
# Add qt dependencies to this list
ivw_add_dependencies(Qt5Core Qt5Gui Qt5Widgets)    
ivw_define_qt_definitions()

#--------------------------------------------------------------------
# Define defintions
ivw_define_standard_definitions(inviwo_qtminimum inviwo_qtminimum) 
#--------------------------------------------------------------------
# Define standard properties
ivw_define_standard_properties(inviwo_qtminimum)

#--------------------------------------------------------------------
# Move to folder
ivw_folder(inviwo_qtminimum minimals)

#--------------------------------------------------------------------
# Add application to pack
if(IVW_PACKAGE_PROJECT)
if(WIN32)
   install(TARGETS inviwo_qtminimum
            RUNTIME DESTINATION bin
            COMPONENT qt_app)
else()
    install(TARGETS inviwo_qtminimum
            RUNTIME DESTINATION bin
            BUNDLE DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            COMPONENT qt_app)
endif()
endif()

#--------------------------------------------------------------------
# Add dependencies
ivw_add_dependencies(${package_list})

#--------------------------------------------------------------------
# Optimize compilation with pre-compilied headers based on inviwo-core
ivw_compile_optimize_inviwo_core()
