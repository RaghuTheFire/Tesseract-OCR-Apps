Tesseract OCR (Optical Character Recognition) is an open-source library primarily used to recognize and extract text from images. It was originally developed by HP in the 1980s and later released as open-source software by Google in 2006. Since then, it has been maintained by Google and a community of developers.

Here’s how Tesseract OCR generally works:

    Image Preprocessing: Before performing OCR, the image may need preprocessing steps like resizing, noise reduction, and contrast enhancement to improve recognition accuracy.

    Text Detection: Tesseract locates regions in the image that potentially contain text using techniques like connected component analysis and edge detection.

    Text Recognition: Once text regions are identified, Tesseract analyzes the shapes of individual characters and attempts to recognize them using machine learning algorithms, typically based on neural networks.

    Post-processing: After recognition, post-processing steps may be applied to improve the accuracy of the extracted text. This could involve spell checking, language model integration, or formatting adjustments.

Tesseract supports various input image formats, including JPEG, PNG, and TIFF, and it can recognize text in multiple languages. Additionally, it provides options for configuring OCR settings to optimize performance for specific use cases.

Many applications and services use Tesseract OCR for tasks such as converting scanned documents into editable text, extracting text from images for indexing or searching, and assisting visually impaired users by reading text aloud from images.



# FormulaRecoginition.cpp

This project is about recognizing a hand written maths formula on a board of chart showing the camera towards what your write. 

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
