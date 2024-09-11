# Get all .cpp files in this dir
file(GLOB neat_src CONFIGURE_DEPENDS "*.cpp")

# Pass the variable to the parent scope
set(neat_src ${neat_src} PARENT_SCOPE)
