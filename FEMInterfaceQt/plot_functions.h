#ifndef PLOT_FUNCTIONS_H
#define PLOT_FUNCTIONS_H
#include <vector>
#include <matplot/matplot.h>

// draw carcass and scatter vertices from AKT
void plot(const std::vector<std::vector<double>>& AKT, const std::vector<int>& xyzQuantity, matplot::line_handle& handle);

// draw carcass
void plot_carcass(const std::vector<std::vector<double>>& AKT, const std::vector<int>& xyzQuantity, const std::string& carcassColor = "black");

// scatter vertices from AKT
void plot_points(const std::vector<std::vector<double>>& AKT, matplot::line_handle& handle);

// set scatterplot points color based on tensions value
void setPointsColor(const std::vector<std::vector<double>>& tensions, matplot::line_handle& sPlot);

void setAxesLabels(const std::vector<std::string>& labels);

void setAxesLimits(const std::vector<double>& xLim = {-0.1, 2.1}, const std::vector<double>& yLim = {-0.1, 2.1}, const std::vector<double>& zLim = {-0.1, 2.1});

// sets color and face color of all points to <color>
void setPointsColor(const std::string& color, matplot::line_handle& handle);

void setPointsColor(const std::vector<double>& colors, matplot::line_handle& handle);

void setPlotWindowPosition(int x, int y);

void setPlotImageSize(int height, int width);

std::vector<std::vector<int>> connectSquares(const std::vector<int>& xyzQuantity, int offset);

std::vector<std::vector<double>> getCoords(const std::vector<std::vector<double>>& AKT, const std::vector<int>& indices);

std::vector<std::vector<int>> connectPillars(const std::vector<int>& xyzQuantity);

std::vector<std::vector<int>> connectTopBotFacets(const std::vector<int>& xyzQuantity, const std::vector<int>& botLayer);

#endif // PLOT_FUNCTIONS_H
