# Formula-Recoginition

This C++ code performs following Tasks:

1. Opening a video capture and creating a video writer.
2. Initializing the Tesseract OCR engine.
3. Processing each frame of the video:
   - Resizing and flipping the frame.
   - Converting to grayscale and binarizing the image.
   - Calculating the distance transform and normalizing it.
   - Performing morphological operations (closing) on the normalized distance transform.
   - Finding contours in the closed image.
   - Processing each contour:
     - Drawing bounding boxes around contours.
     - Performing OCR on the contour region using Tesseract.
     - Storing the recognized text in a vector.
   - Checking if "OK" is present in the detected texts.
   - Drawing a rectangle for displaying the equation.
   - If "OK" is not present, displaying "No Equation".
   - If "OK" is present:
     - Finding the contour representing the equation.
     - Drawing the equation contour.
     - Performing OCR on the equation region using Tesseract.
     - Plotting the equation using the recognized text.
     - Drawing bounding boxes for all parts.
4. Writing the processed frame to the output video.
5. Releasing resources (video capture, video writer, and Tesseract OCR engine).
