#include <fstream>
#include <sstream>
#include "math/numeric.h"
#include "keypoint/keypoint.h"
#include "keypoint/sift.h"
#include "matching/matcher_brute_force.h"
#include "matching/matcher_flann.h"
#include "estimator/fundamental.h"
#include "estimator/param_estimator.h"
#include "estimator/ransac.h"
#include "io/match_io.h"
#include "io/descriptor_io.h"
#include "io/keypoint_io.h"
#include "viz/plot.h"

using std::string;
using std::ifstream;
using std::istringstream;
using namespace open3DCV;

void load_pts(const string fname, vector<Vec2f>& x1, vector<Vec2f>& x2)
{
    x1.clear();
    x2.clear();
    ifstream ifstr(fname.c_str());
    bool good = ifstr.is_open();
    string line;
    while (getline(ifstr, line))
    {
        istringstream iss(line);
        Vec2f y1, y2;
        iss >> y1(0) >> y1(1) >> y2(0) >> y2(1);
        x1.push_back(y1);
        x2.push_back(y2);
    }
    if (!good)
    {
        std::cerr << "Error loading points." << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    // --------------------- preparing data from text file
//    string fname = "/Users/BlacKay/Documents/Projects/open3DCV/test/fundamental/matches.txt";
//    vector<Vec2f> x1, x2;
//    load_pts(fname, x1, x2);
    
    // --------------------- test: eight point algorithm (PASSED)
//    vector<std::pair<Vec2f, Vec2f> > data;
//    for (int i = 0; i < 8; ++i)
//    {
//        std::pair<Vec2f, Vec2f> pair_data;
//        pair_data.first = x1[i];
//        pair_data.second = x2[i];
//        data.push_back(pair_data);
//    }
//    vector<float> params(9);
//    Fundamental_Estimator fund_est(0);
//    fund_est.ls_estimate(data, params);
//    Mat3f F;
//    F << params[0], params[3], params[6],
//         params[1], params[4], params[7],
//         params[2], params[5], params[8];
//    std::cout << F << std::endl;
    
    // --------------------- test: seven point algorithm (PASSED)
//    x1.push_back(Vec2f(6.3625700e+02, 1.3106300e+02));
//    x1.push_back(Vec2f(8.5213700e+02, 3.2111900e+02));
//    x1.push_back(Vec2f(7.7062600e+02, 3.9947900e+02));
//    x1.push_back(Vec2f(6.4878000e+02, 4.0994600e+02));
//    x1.push_back(Vec2f(3.5936200e+02, 4.6630600e+02));
//    x1.push_back(Vec2f(1.9228200e+02, 4.7509400e+02));
//    x1.push_back(Vec2f(1.8743200e+02, 4.8332300e+02));
//    x2.push_back(Vec2f(6.6924900e+02, 1.4101000e+02));
//    x2.push_back(Vec2f(8.4664200e+02, 3.3387300e+02));
//    x2.push_back(Vec2f(7.7101400e+02, 4.1989500e+02));
//    x2.push_back(Vec2f(6.5590300e+02, 4.3190600e+02));
//    x2.push_back(Vec2f(3.4788100e+02, 4.8860200e+02));
//    x2.push_back(Vec2f(1.6275400e+02, 4.9520500e+02));
//    x2.push_back(Vec2f(1.5764300e+02, 5.0328800e+02));
//    
//    Fundamental_Estimator fund_est(0);
//    vector<Mat3f> F;
//    fund_est.fund_seven_pts(x1, x2, F);
//    
//    for (int i = 0; i < F.size(); ++i)
//    {
//        std::cout << "F" << i << ": " << std::endl << F[i] << std::endl;
//    }
    
    // --------------------- test: ransac estimation using correspondences
//    vector<std::pair<Vec2f, Vec2f> > data;
//    for (int i = 0; i < x1.size(); ++i)
//    {
//        std::pair<Vec2f, Vec2f> pair_data;
//        pair_data.first = x1[i];
//        pair_data.second = x2[i];
//        data.push_back(pair_data);
//    }
//    
//    vector<float> params(9);
//    open3DCV::Param_Estimator<std::pair<Vec2f, Vec2f>, float>* fund_esti = new open3DCV::Fundamental_Estimator(0.5f);
//    open3DCV::Ransac<std::pair<Vec2f, Vec2f>, float>::estimate(fund_esti, data, params, 0.95);
//
//    Mat3f F;
//    F << params[0], params[1], params[2],
//         params[3], params[4], params[5],
//         params[6], params[7], params[8];
//    std::cout << F << std::endl;
    
    // --------------------- test: estimate F from two images
    string iname1 = "/Users/BlacKay/Documents/Projects/Images/test/B21.jpg";
    string iname2 = "/Users/BlacKay/Documents/Projects/Images/test/B22.jpg";
    Image image1(iname1), image2(iname2);
    image1.read(iname1);
    image2.read(iname2);
    
    vector<Keypoint> keys1, keys2;
    vector<Vecf> desc1, desc2;
    
//    Detector::read_keypoints(keys1, "key1.txt");
//    Detector::read_keypoints(keys2, "key2.txt");
//    vector<Match> matches;
//    Matcher::read_matches(matches, "matches.txt");
    
    // --------------------- test: estimate F from two images
    Sift_Params sift_params(3, 3, 0, 10.0f, 7.0f, -INFINITY, 3, 2);
    Sift sift(sift_params);
    
    sift.detect_keypoints(image1, keys1, 0);
    draw_cross(image1, keys1, "sift_1");
    sift.extract_descriptors(image1, keys1, desc1);
    write_keypoints(keys1, "key1.txt");
//    write_descriptors(desc1, "desc1.txt");
    sift.clear();
    
    sift_params.peak_thresh_ = 7.0f;
    sift.set_params(sift_params);
    
    sift.detect_keypoints(image2, keys2, 0);
    draw_cross(image2, keys2, "sift_2");
    sift.extract_descriptors(image2, keys2, desc2);
    write_keypoints(keys1, "key2.txt");
//    write_descriptors(desc1, "desc2.txt");
    sift.clear();
    
    vector<Match> matches;
    Matcher_Param matcher_param(0.6, 128, 3, 10);
    
    Matcher_Brute_Force matcher(matcher_param);
    //Matcher_Flann matcher(matcher_param);
    matcher.match(desc1, desc2, matches);
    draw_matches(image1, keys1, image2, keys2, matches, "matching");
    write_matches(matches, "matches.txt");
    
    // --------------------- test: estimate F without ransac
//    Fundamental_Estimator fund_est(0);
//    vector<std::pair<Vec2f, Vec2f> > data;
//    for (int i = 1; i < matches.size(); ++i)
//    {
//        Vec2f y1 = keys1[matches[i].ikey1_].coords();
//        Vec2f y2 = keys2[matches[i].ikey2_].coords();
//        std::pair<Vec2f, Vec2f> pair_data;
//        pair_data.first = y1;
//        pair_data.second = y2;
//        data.push_back(pair_data);
//    }
//    vector<float> params;
//    fund_est.ls_estimate(data, params);
//    Mat3f F;
//    F << params[0], params[1], params[2],
//         params[3], params[4], params[5],
//         params[6], params[7], params[8];
//    std::cout << F << std::endl;
    
    // --------------------- test: ransac estimation on two images
    vector<std::pair<Vec2f, Vec2f> > data;
    for (int i = 0; i < matches.size(); ++i)
    {
        Vec2f x1 = keys1[matches[i].ikey1_].coords();
        Vec2f x2 = keys2[matches[i].ikey2_].coords();
        std::pair<Vec2f, Vec2f> pair_data;
        pair_data.first = x1;
        pair_data.second = x2;
        data.push_back(pair_data);
    }
    
    vector<float> params(9);
    open3DCV::Param_Estimator<std::pair<Vec2f, Vec2f>, float>* fund_esti = new open3DCV::Fundamental_Estimator(0.5f);
    open3DCV::Ransac<std::pair<Vec2f, Vec2f>, float>::estimate(fund_esti, data, params, 0.95);
    
    Mat3f F;
    F << params[0], params[3], params[6],
         params[1], params[4], params[7],
         params[2], params[5], params[8];
    std::cout << F << std::endl;

    // --------------------- estimate epipoles
    Mat3f Ft = F.transpose().eval();
    Vec3f e1, e2, slope;
    nullspace<Mat3f, Vec3f>(&F, &e1);
    nullspace<Mat3f, Vec3f>(&Ft, &e2);
    e1 = e1.array() / e1(2);
    e2 = e2.array() / e2(2);
    
    for (int i = 0; i < matches.size(); ++i)
    {
        slope = pt2slope<Vec2f>(data[i].first, e1.block<2, 1>(0, 0));
        draw_line(image1, slope);
        slope = F * data[i].first.homogeneous();
        draw_line(image2, slope);
    }
    image1.write("left.jpg");
    image2.write("right.jpg");

    return 0;
}
