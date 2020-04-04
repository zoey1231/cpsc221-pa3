#include <cmath>
#include "stats.h"

/* check if given cordinate(x,y) is within the bound of image im
 * 
*/
bool stats::in_bound(int width, int height, int x, int y) 
//modification:passing PNG im as parameter need to copy the image each time we call the function, greatly slow down the process
{
    return !(x < 0 || x > width || y < 0 || y > height);
}

// initialize the private vectors so that, for each color,  entry
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG &im)
{

    // YOUR CODE HERE!!

    // //create vector for each row
    // for (unsigned int i = 0; i < im.width(); i++)
    // {
    //     vector<long> temp;
    //     sumRed.push_back(temp);
    //     sumGreen.push_back(temp);
    //     sumBlue.push_back(temp);
    //     sumsqRed.push_back(temp);
    //     sumsqGreen.push_back(temp);
    //     sumsqBlue.push_back(temp);
    // }
    //initialize each entry with corresponding coordinate's color value
    for (unsigned int i = 0; i < im.width(); i++)
    {
        sumRed.push_back(vector<long>());
        sumGreen.push_back(vector<long>());
        sumBlue.push_back(vector<long>());
        sumsqRed.push_back(vector<long>());
        sumsqGreen.push_back(vector<long>());
        sumsqBlue.push_back(vector<long>());

        for (unsigned int j = 0; j < im.height(); j++)
        {
            sumRed[i].push_back(im.getPixel(i, j)->r);
            sumGreen[i].push_back(im.getPixel(i, j)->g);
            sumBlue[i].push_back(im.getPixel(i, j)->b);
            sumsqRed[i].push_back(pow(im.getPixel(i, j)->r, 2));
            sumsqGreen[i].push_back(pow(im.getPixel(i, j)->g, 2));
            sumsqBlue[i].push_back(pow(im.getPixel(i, j)->b, 2));
        }
    }

    //fill in each entry column by column
    for (int i = 0; i < (int)im.width(); i++)
    {
        for (int j = 0; j < (int)im.height(); j++)
        {
            //check the left cell
            if (in_bound(im.width(), im.height(), i - 1, j))
            {
                sumRed[i][j] += sumRed[i - 1][j];
                sumGreen[i][j] += sumGreen[i - 1][j];
                sumBlue[i][j] += sumBlue[i - 1][j];
                sumsqRed[i][j] += sumsqRed[i - 1][j];
                sumsqGreen[i][j] += sumsqGreen[i - 1][j];
                sumsqBlue[i][j] += sumsqBlue[i - 1][j];
            }
            //check the top cell
            if (in_bound(im.width(), im.height(), i, j - 1))
            {
                sumRed[i][j] += sumRed[i][j - 1];
                sumGreen[i][j] += sumGreen[i][j - 1];
                sumBlue[i][j] += sumBlue[i][j - 1];
                sumsqRed[i][j] += sumsqRed[i][j - 1];
                sumsqGreen[i][j] += sumsqGreen[i][j - 1];
                sumsqBlue[i][j] += sumsqBlue[i][j - 1];
            }
            //check the top-left cell
            if (in_bound(im.width(), im.height(), i - 1, j - 1))
            {
                sumRed[i][j] -= sumRed[i - 1][j - 1];
                sumGreen[i][j] -= sumGreen[i - 1][j - 1];
                sumBlue[i][j] -= sumBlue[i - 1][j - 1];
                sumsqRed[i][j] += sumsqRed[i - 1][j - 1];
                sumsqGreen[i][j] += sumsqGreen[i - 1][j - 1];
                sumsqBlue[i][j] += sumsqBlue[i - 1][j - 1];
            }
        }
    }
}

/* returns the sums of all pixel values across all color channels.
	* useful in computing the score of a rectangle
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr)
{
    //reference: https://piazza.com/class/k4esgvw2pp64oh?cid=2627
    // YOUR CODE HERE!!
    long return_value = -1;
    switch (channel)
    {
    case 'r':
        return_value = sumRed[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumRed[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumRed[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumRed[ul.first - 1][ul.second - 1] : 0);
        break;
    case 'g':
        return_value = sumGreen[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumGreen[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumGreen[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumGreen[ul.first - 1][ul.second - 1] : 0);
        break;
    case 'b':
        return_value = sumBlue[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumBlue[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumBlue[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumBlue[ul.first - 1][ul.second - 1] : 0);
        break;
    default:
        break;
    }
    return return_value;
}

/* returns the sums of squares of all pixel values across all color channels.
	* useful in computing the score of a rectangle
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr)
{

    // YOUR CODE HERE!!
    long return_value = -1;
    switch (channel)
    {
    case 'r':
        return_value = sumsqRed[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumsqRed[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumsqRed[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumsqRed[ul.first - 1][ul.second - 1] : 0);
        break;
    case 'g':
        return_value = sumsqGreen[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumsqGreen[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumsqGreen[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumsqGreen[ul.first - 1][ul.second - 1] : 0);
        break;
    case 'b':
        return_value = sumsqBlue[lr.first][lr.second] - ((ul.second - 1) >= 0 ? sumsqBlue[lr.first][ul.second - 1] : 0) - ((ul.first - 1) >= 0 ? sumsqBlue[ul.first - 1][lr.second] : 0) + (((ul.first - 1) >= 0 && (ul.second - 1) >= 0) ? sumsqBlue[ul.first - 1][ul.second - 1] : 0);
        break;
    default:
        break;
    }
    return return_value;
}

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int, int> ul, pair<int, int> lr)
{
    // YOUR CODE HERE!!
    return abs(lr.first - ul.first + 1) * abs(lr.second - ul.second + 1);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getScore(pair<int, int> ul, pair<int, int> lr)
{

    // YOUR CODE HERE!!
    //Σx∈R(x−x¯)^2=Σx∈Rx^2−(Σx∈Rx)^2/|R|
    long score_red = getSumSq('r', ul, lr) - getSum('r', ul, lr) * getSum('r', ul, lr) / rectArea(ul, lr);
    long score_green = getSumSq('g', ul, lr) - getSum('g', ul, lr) * getSum('g', ul, lr) / rectArea(ul, lr);
    long score_blue = getSumSq('b', ul, lr) - getSum('b', ul, lr) * getSum('b', ul, lr) / rectArea(ul, lr);
    return score_red + score_green + score_blue;
}

// given a rectangle, return the average color value over the rectangle as a pixel.
/* Each color component of the pixel is the average value of that component over
* the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr)
{
    // YOUR CODE HERE!!
    int red = getSum('r', ul, lr) / rectArea(ul, lr);
    int green = getSum('g', ul, lr) / rectArea(ul, lr);
    int blue = getSum('b', ul, lr) / rectArea(ul, lr);
    RGBAPixel pixel(red, green, blue);
    return pixel;
}
