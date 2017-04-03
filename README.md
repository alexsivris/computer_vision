# Computer Vision / Image Processing #

This repository includes some common C++ snippets using OpenCV3. Moreover, some of my projects are included.

# Contents #

*  /Projects
	*  /integral_histograms: implementation of integral histograms as proposed in "Integral Histogram: A Fast Way to Extract Histograms in Cartesian Spaces" by Fatih Porikli. Integral histograms can be used for real-time object detection. Once the integral histogram of an image is computed, the histogram of any region can be computed in constant time (as with integral images).
	*  /homework\_computer\_vision\_TUM: Matlab homework of lecture "Computer Vision" (3D point reconstruction)

*  /Snippets
	*  /simple_trafo: simple image transforms like blurring (smoothing), pyramids, log-polar
	*  /paint: started implementing a small paint program 
	*  /mappings: perspective mappings; computation of affine/rotation matrix from parameters like angle, center and scale
	*  /kalman: implemented Kalman filter using OpenCV's KalmanFilter class
	*  /histograms: simple histogram calculation
	*  /functors: PCA and random number generator functors
	*  /kalman_tracking: tracking (click on object) using Kalman Filter

# Contributors #

Projects and Snippets:

*  Alexandros Sivris

MATLAB homework for the "Computer Vision" course:

*  Benjamin Braun
*  Juri Fedjaev
*  Philipp Schreiber
*  Alexandros Sivris
