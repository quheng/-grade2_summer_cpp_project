#ifndef HISTOGRAM
#define HISTOGRAM
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../common/common.h"
#include <iostream>
#include <stdio.h>
#include <string>


using namespace std;
using namespace cv;


CHistogram getHistogram(const Mat& src)
{
    CHistogram hist;

    //分割成3个单通道图像 ( R, G 和 B )
    vector<Mat> rgb_planes;
    split( src, rgb_planes );

     /// 设定bin数目
    int histSize = 255;

    /// 设定取值范围 ( R,G,B) )
    float range[] = { 0, 255 } ;
    const float* histRange = { range };

    bool uniform = true;
    bool accumulate = false;

    Mat r_hist, g_hist, b_hist;

    // 计算直方图:
    calcHist( &rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

    // 创建直方图画布
    int hist_w = 400; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    hist.setGreenHistogram(Mat ( hist_w, hist_h, CV_8UC3, Scalar( 240,240,240)));
    hist.setBlueHistogram(Mat ( hist_w, hist_h, CV_8UC3, Scalar( 240,240,240)));
    hist.setRedHistogram(Mat ( hist_w, hist_h, CV_8UC3, Scalar( 240,240,240)));

    /// 将直方图归一化到范围 [ 0, histImage.rows ]
    normalize(r_hist, r_hist, 0,  hist.getRedHistogram().rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0,  hist.getRedHistogram().rows, NORM_MINMAX, -1, Mat() );
    normalize(b_hist, b_hist, 0,  hist.getRedHistogram().rows, NORM_MINMAX, -1, Mat() );

    //在直方图画布上画出直方图
    for( int i = 1; i < histSize; i++ )
    {
      line( hist.getRedHistogram(), Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
      line( hist.getGreenHistogram(), Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( hist.getBlueHistogram(), Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
     }

    // 显示直方图
    //namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    //imshow("calcHist Demo", hist.getRedHistogram() );

    return hist;
 }

#endif // HISTOGRAM

