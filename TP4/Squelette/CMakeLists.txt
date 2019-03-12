# Version de cmake demandée.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
# Chemin des répertoires contenant les fichiers entêtes.
INCLUDE_DIRECTORIES( src/include )

# Chemin du répertoire contenant les binaires.
SET ( EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# Option du compilateur pour supporter C 2011.
SET( CMAKE_C_FLAGS "-std=c11")

# Création des exécutables.
ADD_EXECUTABLE( dry_run       src/bench.c src/matvec.c       )
ADD_EXECUTABLE( bench        src/bench.c src/matvec.c        )
ADD_EXECUTABLE( bench_r4     src/bench.c src/matvec_r4.c     )
ADD_EXECUTABLE( bench_sse_r4 src/bench.c src/matvec_sse_r4.c )

# Symboles pré-processeur nécessaires à la génération des exécutables.
TARGET_COMPILE_DEFINITIONS( dry_run      PRIVATE RAW PRIVATE DRY_RUN )
TARGET_COMPILE_DEFINITIONS( bench        PRIVATE RAW                 )
TARGET_COMPILE_DEFINITIONS( bench_r4     PRIVATE R4                  )
TARGET_COMPILE_DEFINITIONS( bench_sse_r4 PRIVATE SSE_R4              )

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )

