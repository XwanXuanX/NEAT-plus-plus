set(main_src main.cpp)
set(utility_src utility.cpp)
set(genotype_src genotype.cpp)
set(prob_src prob.cpp)

# Form the full path to the source files...
PREPEND(main_src)
PREPEND(utility_src)
PREPEND(genotype_src)
PREPEND(prob_src)

# ... and pass the variable to the parent scope.
set(main_src ${main_src}  PARENT_SCOPE)
set(utility_src ${utility_src}  PARENT_SCOPE)
set(genotype_src ${genotype_src}  PARENT_SCOPE)
set(prob_src ${prob_src} PARENT_SCOPE)
