#include <GripPipeline.hpp>

namespace grip {

	GripPipeline::GripPipeline() {

	}
	/**
	* Runs an iteration of the pipeline and updates outputs.
	*/
	void GripPipeline::Process(cv::Mat& source0){
		//Step RGB_Threshold0:
		//input
		cv::Mat rgbThresholdInput = source0;
		double rgbThresholdRed[] = {0.0, 255.0};
		double rgbThresholdGreen[] = {213.26438848920864, 255.0};
		double rgbThresholdBlue[] = {0.0, 255.0};
		rgbThreshold(rgbThresholdInput, rgbThresholdRed, rgbThresholdGreen, rgbThresholdBlue, this->rgbThresholdOutput);
		//Step Find_Contours0:
		//input
		cv::Mat findContoursInput = rgbThresholdOutput;
		const bool findContoursExternalOnly = false;  // default Boolean
		findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);
		//Step Filter_Contours0:
		//input
		std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;
		const double filterContoursMinArea = 1000.0;  // default Double
		const double filterContoursMinPerimeter = 0;  // default Double
		const double filterContoursMinWidth = 0;  // default Double
		const double filterContoursMaxWidth = 1000;  // default Double
		const double filterContoursMinHeight = 0;  // default Double
		const double filterContoursMaxHeight = 1000;  // default Double
		double filterContoursSolidity[] = {0.0, 100.0};
		const double filterContoursMaxVertices = 1000000;  // default Double
		const double filterContoursMinVertices = 0;  // default Double
		const double filterContoursMinRatio = 0.0;  // default Double
		const double filterContoursMaxRatio = 1.0;  // default Double
		filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
	}

	/**
	 * This method is a generated getter for the output of a RGB_Threshold.
	 * @return Mat output from RGB_Threshold.
	 */
	cv::Mat* GripPipeline::GetRgbThresholdOutput(){
		return &(this->rgbThresholdOutput);
	}
	/**
	 * This method is a generated getter for the output of a Find_Contours.
	 * @return ContoursReport output from Find_Contours.
	 */
	std::vector<std::vector<cv::Point> >* GripPipeline::GetFindContoursOutput(){
		return &(this->findContoursOutput);
	}
	/**
	 * This method is a generated getter for the output of a Filter_Contours.
	 * @return ContoursReport output from Filter_Contours.
	 */
	std::vector<std::vector<cv::Point> >* GripPipeline::GetFilterContoursOutput(){
		return &(this->filterContoursOutput);
	}
	/**
	 * Segment an image based on color ranges.
	 *
	 * @param input The image on which to perform the RGB threshold.
	 * @param red The min and max red.
	 * @param green The min and max green.
	 * @param blue The min and max blue.
	 * @param output The image in which to store the output.
	 */
	void GripPipeline::rgbThreshold(cv::Mat &input, double red[], double green[], double blue[], cv::Mat &output) {
		cv::cvtColor(input, output, cv::COLOR_BGR2RGB);
		cv::inRange(output, cv::Scalar(red[0], green[0], blue[0]), cv::Scalar(red[1], green[1], blue[1]), output);
	}

	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void GripPipeline::findContours(cv::Mat &input, const bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		const int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		const int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}


	/**
	 * Filters through contours.
	 * @param inputContours is the input vector of contours.
	 * @param minArea is the minimum area of a contour that will be kept.
	 * @param minPerimeter is the minimum perimeter of a contour that will be kept.
	 * @param minWidth minimum width of a contour.
	 * @param maxWidth maximum width.
	 * @param minHeight minimum height.
	 * @param maxHeight  maximimum height.
	 * @param solidity the minimum and maximum solidity of a contour.
	 * @param minVertexCount minimum vertex Count of the contours.
	 * @param maxVertexCount maximum vertex Count.
	 * @param minRatio minimum ratio of width to height.
	 * @param maxRatio maximum ratio of width to height.
	 * @param output vector of filtered contours.
	 */
	void GripPipeline::filterContours(std::vector<std::vector<cv::Point> > &inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, std::vector<std::vector<cv::Point> > &output) {
		std::vector<cv::Point> hull;
		output.clear();
		for (std::vector<cv::Point> contour: inputContours) {
			cv::Rect bb = boundingRect(contour);
			if (bb.width < minWidth || bb.width > maxWidth) continue;
			if (bb.height < minHeight || bb.height > maxHeight) continue;
			double area = cv::contourArea(contour);
			if (area < minArea) continue;
			if (arcLength(contour, true) < minPerimeter) continue;
			cv::convexHull(cv::Mat(contour, true), hull);
			double solid = 100 * area / cv::contourArea(hull);
			if (solid < solidity[0] || solid > solidity[1]) continue;
			if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
			double ratio = (double) bb.width / (double) bb.height;
			if (ratio < minRatio || ratio > maxRatio) continue;
			output.push_back(contour);
		}
	}



} // end grip namespace

