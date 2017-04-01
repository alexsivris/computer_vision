cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11)
add_definitions(-DDBG)
project( LearningOpenCV )

find_package( OpenCV REQUIRED )


# 1. program - display picture
add_executable( display_picture display_picture/main.cpp )
target_link_libraries( display_picture ${OpenCV_LIBS} )

# 2. program - display video
add_executable(display_video display_video/main.cpp)
target_link_libraries( display_video ${OpenCV_LIBS} )

# 3. program - simple trafo
add_executable(simple_trafo simple_trafo/main.cpp)
target_link_libraries( simple_trafo ${OpenCV_LIBS} )

# 4. program - pyramid trafo
add_executable(pyramid_trafo pyramid_trafo/main.cpp)
target_link_libraries( pyramid_trafo ${OpenCV_LIBS} )

# 5. program - find contours
add_executable(find_contours contours/find_contours.cpp)
target_link_libraries( find_contours ${OpenCV_LIBS} )

# 6. program - find contours 2
add_executable(find_contours2 contours/find_contours2.cpp)
target_link_libraries( find_contours2 ${OpenCV_LIBS} )

# 7. program - connected component analysis
add_executable(cpa connected_component_analysis/cpa.cpp)
target_link_libraries( cpa ${OpenCV_LIBS} )

# 8. program - shape example
add_executable(shape_example contours/shape_example.cpp)
target_link_libraries( shape_example ${OpenCV_LIBS} )

# 9. program - shape simple
add_executable(shape_simple contours/shape_simple.cpp)
target_link_libraries( shape_simple ${OpenCV_LIBS} )

# 10. program - find contours (old version)
add_executable(find_contours_old contours/find_contours_old.cpp)
target_link_libraries( find_contours_old ${OpenCV_LIBS} )


#### 6 ####

# 1 drawing
add_executable(drawing drawing/drawing.cpp)
target_link_libraries(drawing ${OpenCV_LIBS})

#### 7 ####

# 
add_executable(functors functors/functors.cpp)
target_link_libraries(functors ${OpenCV_LIBS})

add_executable( files files/files.cpp )
target_link_libraries( files ${OpenCV_LIBS} )

add_executable( paint paint/paint.cpp )
target_link_libraries( paint ${OpenCV_LIBS} )

add_executable( mappings mappings/mappings.cpp )
target_link_libraries( mappings ${OpenCV_LIBS} )

add_executable( persp mappings/persp.cpp)
target_link_libraries( persp ${OpenCV_LIBS})
add_executable( analysis analysis/dft.cpp)
target_link_libraries( analysis ${OpenCV_LIBS})

add_executable( hist histograms/histograms.cpp)
target_link_libraries( hist ${OpenCV_LIBS})

add_executable( kalman kalman/kalman.cpp)
target_link_libraries( kalman ${OpenCV_LIBS})

add_executable( lkdemo ch17ex/lkdemo.cpp)
target_link_libraries( lkdemo ${OpenCV_LIBS})

add_executable(kf ch17ex/kalman.cpp)
target_link_libraries( kf ${OpenCV_LIBS})
