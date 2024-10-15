#include "plot_functions.h"
#include <algorithm>

void plot(const std::vector<std::vector<double>>& AKT, const std::vector<int>& xyzQuantity, matplot::line_handle& handle)
{
    plot_carcass(AKT, xyzQuantity);
    plot_points(AKT, handle);
}

void plot_carcass(const std::vector<std::vector<double>>& AKT, const std::vector<int>& xyzQuantity, const std::string& carcassColor)
{
    std::vector<std::vector<int>> pointsStorageHorizontal = connectSquares(xyzQuantity, 0);
    std::vector<std::vector<int>> pillarsIndices = connectPillars(xyzQuantity);
    std::vector<std::vector<int>> topBotFacets = connectTopBotFacets(xyzQuantity, pointsStorageHorizontal[0]);

    matplot::gca()->next_plot_replace(false);
    matplot::gcf()->reactive_mode(false);
    // Plot the cube edges
    for (int i = 0; i < pointsStorageHorizontal.size(); i++)
    {
        auto points = getCoords(AKT, pointsStorageHorizontal[i]);
        auto h = matplot::plot3(points[0], points[1], points[2]);
        h->line_width(2);
        h->color(carcassColor);
    }
    for (int i = 0; i < pillarsIndices.size(); i++)
    {
        auto points = getCoords(AKT, pillarsIndices[i]);
        auto h = matplot::plot3(points[0], points[1], points[2]);
        h->line_width(2);
        h->color(carcassColor);
    }
    for (int i = 0; i < topBotFacets.size(); i++)
    {
        auto points = getCoords(AKT, topBotFacets[i]);
        auto f1 = matplot::plot3(points[0], points[1], points[2]);
        f1->line_width(2);
        f1->color(carcassColor);
    }
}

void plot_points(const std::vector<std::vector<double>>& AKT, matplot::line_handle& handle)
{
    handle = matplot::scatter3(AKT[0], AKT[1], AKT[2]);
    handle->marker_size(16);
}

void setPointsColor(const std::vector<std::vector<double>>& tensions, matplot::line_handle& sPlot)
{
    auto [min_it, max_it] = std::minmax_element(tensions.begin(), tensions.end(), [](const std::vector<double>& el1, const std::vector<double>& el2){ return el1[0] < el2[0];});
    double minTension = (*min_it)[0];
    double maxTension = (*max_it)[0];

    // Normalize tensions to [0, 1] range
    std::vector<double> normalizedTensions;
    for (const std::vector<double>& t : tensions)
    {
        normalizedTensions.push_back((t[0] - minTension) / (maxTension - minTension));
    }
    std::vector<double> colors(tensions.size());
    int i = -1;
    matplot::colormap(matplot::palette::viridis());
    std::generate(colors.begin(), colors.end(),
    [&normalizedTensions, &i]()
    {
        i++; return normalizedTensions[i] * 4;
    });
    sPlot->marker_colors(colors);
    sPlot->marker_face_color({0,0,0});
}

void setAxesLabels(const std::vector<std::string>& labels)
{
    matplot::xlabel(labels[0]);
    matplot::ylabel(labels[1]);
    matplot::zlabel(labels[2]);
}

void setAxesLimits(const std::vector<double>& xLim, const std::vector<double>& yLim, const std::vector<double>& zLim)
{
    matplot::xlim({xLim[0], xLim[1]});
    matplot::ylim({yLim[0], yLim[1]});
    matplot::zlim({zLim[0], zLim[1]});
}

void setPointsColor(const std::string& color, matplot::line_handle& handle)
{
    handle->marker_color(color);
    handle->marker_face_color(color);
}

void setPointsColor(const std::vector<double>& colors, matplot::line_handle& handle)
{
    handle->marker_colors(colors);
    handle->marker_face_color({0,0,0});
}

void setPlotWindowPosition(int x, int y)
{
    matplot::gcf()->backend()->position_x(x);
    matplot::gcf()->backend()->position_y(y);
}

void setPlotImageSize(int height, int width)
{
    matplot::gcf()->backend()->height(height);
    matplot::gcf()->backend()->width(width);
}

std::vector<std::vector<int>> connectSquares(const std::vector<int>& xyzQuantity, int offset)
{
    std::vector<std::vector<int>> res(xyzQuantity[2] + 1, std::vector<int>());

    //first layer ( z = 0 )
    int longXPointsCount = xyzQuantity[0] * 2 + 1;
    int shortXPointsCount = xyzQuantity[0] + 1;
    for (int i = 0; i < longXPointsCount; i++)
    {
        res[0].push_back(i + offset);
    }

    int pointNumber = longXPointsCount - 1;
    int longYPointsCount = xyzQuantity[1] * 2 + 1;
    for (int i = 1; i < longYPointsCount; i++)
    {
        pointNumber += i % 2 == 0 ? longXPointsCount : shortXPointsCount;
        res[0].push_back(pointNumber + offset);
    }

    int totalBigPlanePointsCount = longXPointsCount + (longXPointsCount + shortXPointsCount) * xyzQuantity[1];
    int lastPlanePointIndex = totalBigPlanePointsCount - 1;
    for (int i = 1; i < longXPointsCount; i++)
    {
        res[0].push_back(lastPlanePointIndex - i + offset);
    }

    pointNumber = (longXPointsCount + shortXPointsCount) * xyzQuantity[1];
    for (int i = 0; i < longYPointsCount - 1; i++)
    {
        pointNumber -= i % 2 == 1 ? longXPointsCount : shortXPointsCount;
        res[0].push_back(pointNumber + offset);
    }

    // the rest of layers which are top facets of finite elements
    int connectedPointsCount = res[0].size();
    int shortYPointsCount = xyzQuantity[1] + 1;
    int totalSmallPlanePointsCount = shortXPointsCount * shortYPointsCount;
    for (int i = 1; i < xyzQuantity[2] + 1; i++)
    {
        res[i] = std::vector<int>(connectedPointsCount);
        for (int j = 0; j < connectedPointsCount; j++)
        {
            res[i][j] = res[i - 1][j] + totalBigPlanePointsCount + totalSmallPlanePointsCount;
        }
    }
    return res;
}

// create new vector from AKTi choosing indices
std::vector<std::vector<double>> getCoords(const std::vector<std::vector<double>>& AKT, const std::vector<int>& indices)
{
    std::vector<std::vector<double>> res(3, std::vector<double>(indices.size()));
    for (int i = 0; i < res[0].size(); i++)
    {
        res[0][i] = AKT[0][indices[i]];
        res[1][i] = AKT[1][indices[i]];
        res[2][i] = AKT[2][indices[i]];
    }
    return res;
}

std::vector<std::vector<int>> connectPillars(const std::vector<int>& xyzQuantity)
{
    // some point counts
    int longXPointsCount = xyzQuantity[0] * 2 + 1;
    int shortXPointsCount = xyzQuantity[0] + 1;
    int totalBigPlanePointsCount = longXPointsCount + (longXPointsCount + shortXPointsCount) * xyzQuantity[1];
    int shortYPointsCount = xyzQuantity[1] + 1;
    int totalSmallPlanePointsCount = shortXPointsCount * shortYPointsCount;
    int longZPointsCount = xyzQuantity[2] * 2 + 1;
    int remainingBigX = totalBigPlanePointsCount - longXPointsCount;
    int remainingSmallX = totalSmallPlanePointsCount - shortXPointsCount;
    int topLeftIdx = xyzQuantity[0] * 2 + xyzQuantity[1];

    // output
    std::vector<std::vector<int>> res((xyzQuantity[0] + xyzQuantity[1]) * 2, std::vector<int>(longZPointsCount));

    // offset lambdas for transform
    int c = -1;
    auto lambdaX = [&c](int elem)
    {
        c++;
        return elem + (c % 2 == 0 ? 2 : 1);
    };
    auto lambdaY = [&c, &longXPointsCount, &shortXPointsCount](int elem)
    {
        c++;
        return elem + shortXPointsCount + (c % 2 == 0 ? longXPointsCount : 0);
    };

    res[0][0] = 0;
    res[topLeftIdx][0] = remainingBigX;
    for (int i = 1; i < longZPointsCount; i++)
    {
        res[0][i] = res[0][i - 1] + (i % 2 == 1 ? totalBigPlanePointsCount : totalSmallPlanePointsCount);
        res[topLeftIdx][i] = res[topLeftIdx][i - 1] + (i % 2 == 1 ? longXPointsCount + remainingSmallX : shortXPointsCount + remainingBigX);
    }

    int resIdx = 1;
    for (int j = 0; j < xyzQuantity[0]; j++, resIdx++)
    {
        c = -1;
        std::transform(res[resIdx - 1].begin(), res[resIdx - 1].end(), res[resIdx].begin(), lambdaX);
        c = -1;
        std::transform(res[topLeftIdx - j].begin(), res[topLeftIdx - j].end(), res[topLeftIdx - 1 - j].begin(), lambdaX);
    }

    int colCount = res.size();
    for (int j = 0; j < xyzQuantity[1] - 1; j++, resIdx++)
    {
        c = -1;
        std::transform(res[resIdx - 1].begin(), res[resIdx - 1].end(), res[resIdx].begin(), lambdaY);
        c = -1;
        if (j == 0)
        {
            std::transform(res[0].begin(), res[0].end(), res[colCount - 1].begin(), lambdaY);
        }
        else
        {
            std::transform(res[colCount - j].begin(), res[colCount - j].end(), res[colCount - 1 - j].begin(), lambdaY);
        }
    }
    return res;
}

std::vector<std::vector<int>> connectTopBotFacets(const std::vector<int>& xyzQuantity, const std::vector<int>& botLayer)
{
    int longXPointsCount = xyzQuantity[0] * 2 + 1;
    int longYPointsCount = xyzQuantity[1] * 2 + 1;
    int shortXPointsCount = xyzQuantity[0] + 1;
    int totalBigPlanePointsCount = longXPointsCount + (longXPointsCount + shortXPointsCount) * xyzQuantity[1];
    int shortYPointsCount = xyzQuantity[1] + 1;
    int totalSmallPlanePointsCount = shortXPointsCount * shortYPointsCount;
    int offset = (totalBigPlanePointsCount + totalSmallPlanePointsCount) * xyzQuantity[2];
    std::vector<std::vector<int>> res((xyzQuantity[0] + xyzQuantity[1] - 2) * 2);
    int middle = res.size() / 2;

    // for offset
    int horizontalCount = xyzQuantity[1] - 1;

    // horizontal constant y lines
    for (int i = 0; i < horizontalCount; i++)
    {
        res[i] = std::vector<int>(longXPointsCount);
        res[i][0] = botLayer[0] + (longXPointsCount + shortXPointsCount) * (i + 1);
        for (int j = 1; j < longXPointsCount; j++)
        {
            res[i][j] = res[i][j - 1] + 1;
        }
    }

    // vertical constant x lines
    int topLeftIndex = longXPointsCount * 2 + longYPointsCount - 3;
    for (int i = 0; i < xyzQuantity[0] - 1; i++)
    {
        res[horizontalCount + i] = std::vector<int>(longYPointsCount);
        for (int j = 0; j < longYPointsCount; j++)
        {
            res[horizontalCount + i][j] = botLayer[topLeftIndex + j] + (j % 2 == 0 ? 2 : 1) * (i + 1);
        }
    }

    // initializing top facet using bot facet + offset;
    for (int i = 0; i < middle; i++)
    {
        res[middle + i] = std::vector<int>(res[i]);
        std::transform(res[middle + i].begin(), res[middle + i].end(), res[middle + i].begin(), [&offset](int elem) { return elem + offset; });
    }

    return res;
}
