#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;

int main() 
{
  // Open video capture
  VideoCapture cap(""); // Path to your video

  // Check if video capture is opened successfully
  if (!cap.isOpened()) {
    cout << "Unable to read camera feed" << endl;
    return -1;
  }

  // Create video writer
  VideoWriter out("equation_detection.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 20, Size(540, 380));

  // Initialize Tesseract OCR
  tesseract::TessBaseAPI ocr;
  ocr.Init(nullptr, "eng", tesseract::OEM_DEFAULT);
  ocr.SetPageSegMode(tesseract::PSM_SINGLE_WORD);

  // Define kernel for morphological operations
  Mat rectKernel = getStructuringElement(MORPH_RECT, Size(30, 10));

  while (true) 
  {
    Mat frame;
    bool ret = cap.read(frame);

    if (!ret) 
    {
      break;
    }

    // Resize and flip the frame
    resize(frame, frame, Size(540, 380), 0, 0, INTER_CUBIC);
    flip(frame, frame, 0);
    flip(frame, frame, 1);

    // Convert to grayscale
    Mat gray;
    cvtColor(frame, gray, COLOR_RGB2GRAY);

    // Binarize the image
    Mat thresh;
    threshold(gray, thresh, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

    // Calculate distance transform
    Mat dist;
    distanceTransform(thresh, dist, DIST_L2, 5);

    // Normalize the distance transform
    Mat dist_norm;
    normalize(dist, dist_norm, 0, 1.0, NORM_MINMAX);
    dist_norm = (dist_norm * 255).astype("uint8");

    // Binarize the normalized distance transform
    threshold(dist_norm, dist_norm, 0, 255, THRESH_BINARY | THRESH_OTSU);

    // Perform closing operation
    Mat dist_closing;
    morphologyEx(dist_norm, dist_closing, MORPH_CLOSE, rectKernel);

    // Find contours in the closed image
    vector < vector < Point >> cnts;
    findContours(dist_closing, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Process each contour
    vector < string > texts;
    for (const auto & cnt: cnts) 
    {
      Rect rect = boundingRect(cnt);
      if (rect.width >= 13 && rect.height >= 13) 
      {
        rectangle(frame, rect, Scalar(0, 0, 255), 1);
        Mat detected_part = dist_norm(Rect(max(rect.x - 13, 0), max(rect.y - 13, 0),min(rect.width + 26, dist_norm.cols - rect.x + 13),min(rect.height + 26, dist_norm.rows - rect.y + 13)));
        ocr.SetImage(detected_part.data, detected_part.cols, detected_part.rows, 1, detected_part.step);
        string part_pred = string(ocr.GetUTF8Text());
        if (!part_pred.empty()) 
        {
          texts.push_back(part_pred);
        }
      }
    }

    // Check if "OK" is present in the detected texts
    bool yes_OK = false;
    for (const auto & text: texts) 
    {
      if (text == "OK") 
      {
        yes_OK = true;
        break;
      }
    }

    // Draw a rectangle for displaying the equation
    rectangle(frame, Point(0, 0), Point(136, 136), Scalar(255, 255, 255), -1);
    rectangle(frame, Point(0, 0), Point(135, 135), Scalar(0, 0, 0), 2);

    if (!yes_OK) 
    {
      putText(frame, "No Equation", Point(13, 65), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
    } 
    else 
    {
      // Find the contour representing the equation
      vector < vector < Point >> all_parts;
      vector < int > cnts_y_axis;
      for (const auto & cnt: cnts) 
      {
        Rect rect = boundingRect(cnt);
        if (rect.width >= 15 && rect.height >= 15) 
        {
          all_parts.push_back(cnt);
          cnts_y_axis.push_back(rect.y);
        }
      }
      vector < int > idx = sort_indexes(cnts_y_axis);
      vector < vector < Point >> equation(all_parts.begin() + idx.size(), all_parts.end());

      // Draw the equation contour
      vector < Point > hull;
      convexHull(equation, hull);
      Rect rect = boundingRect(hull);
      rectangle(frame, rect, Scalar(0, 0, 255), 1);

      // Perform OCR on the equation
      Mat detected_part = dist_norm(Rect(max(rect.x - 10, 0), max(rect.y - 10, 0),min(rect.width + 20, dist_norm.cols - rect.x + 10),min(rect.height + 20, dist_norm.rows - rect.y + 10)));
      ocr.SetImage(detected_part.data, detected_part.cols, detected_part.rows, 1, detected_part.step);
      string part_pred = string(ocr.GetUTF8Text());

      // Plot the equation
      rectangle(frame, Point(0, 0), Point(68, 68), Scalar(0, 0, 0), 1);
      rectangle(frame, Point(68, 68), Point(135, 135), Scalar(0, 0, 0), 1);
      try 
      {
        string equation_str = part_pred;
        transform(equation_str.begin(), equation_str.end(), equation_str.begin(), ::tolower);
        vector < Point > points;
        for (int x = -68; x < 68; x++) 
        {
          double y = eval(equation_str, x);
          int x_new = x + 68;
          int y_new = static_cast < int > (-y / 3 + 68);
          if (y_new < 135) 
          {
            points.emplace_back(x_new, y_new);
          }
        }
        polylines(frame, points, true, Scalar(0, 0, 255), 2);
      } 
      catch (...) 
      {
        putText(frame, "Error", Point(13, 65), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
      }

      // Draw bounding boxes for all parts
      for (const auto & part: all_parts) 
      {
        Rect rect = boundingRect(part);
        rectangle(frame, rect, Scalar(0, 0, 255), 1);
      }
    }

    // Write the frame to the output video
    out.write(frame);
  }

  // Release resources
  cap.release();
  out.release();
  ocr.End();

  return 0;
}


