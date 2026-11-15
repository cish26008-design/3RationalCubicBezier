#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // value_ptr
#include <QtOpenGL>
#include <cmath>
#include <iostream>

class CubicBezier
{
public:
    std::vector<glm::vec2> controlPoint;
    std::vector<glm::vec2> bezpt;
    std::vector<glm::vec2> Comb;
    std::vector<glm::vec2> Combspike;
    const int ptBezNum = 1000;
    float phi1;
    float phi2;
    bool calculated = false;
    float K1min;
    float K0max;

    std::vector<float>curvatureY;
    std::vector<float>curvatureX;

public:
    glm::vec2 circleCenter0;
    float circleRadius0 = 0.0f;

    glm::vec2 circleCenter1;
    float circleRadius1 = 0.0f;

public:
    float w0 = 1.f;
    float w1 = 1.f;
    float w2 = 1.f;
    float w3 = 1.f;


    CubicBezier() {
        bezpt.resize(ptBezNum);
    }
    std::vector<glm::vec2> &GetCpt() { return controlPoint; }
    std::vector<glm::vec2> &GetPoints() {return bezpt; }
    std::vector<glm::vec2> &GetComb() {return Comb;}
    std::vector<glm::vec2> &GetCombspike() {return Combspike; }
    int GetCptSize() { return controlPoint.size(); }
    int GetCombSize() {return Comb.size();}

    void ComputePoints()
    {

        Comb.clear();
        Combspike.clear();

        curvatureY.clear();//y軸用
        curvatureX.clear(); //ｘ軸用


        if(controlPoint.size() == 3){
        for (int i = 0; i < ptBezNum; i++)
        {
            GLfloat t = (GLfloat)i / (ptBezNum-1);

            glm::vec2 p0 = controlPoint[0];
            glm::vec2 p1 = controlPoint[1];
            glm::vec2 p2 = controlPoint[2];

            //3次有理Bezierの式
            bezpt[i] = glm::vec2(((1-t)*(1-t)*w0*p0 + 2*(1-t)*t*w1*p1 + t*t*w2*p2) /
                            ((1-t)*(1-t)*w0+ 2*(1-t)*t*w1+ t*t*w2));

        }
        }
        if(controlPoint.size() == 4){
            for (int i = 0; i < ptBezNum; i++)
            {

                GLfloat t = (GLfloat)i / (ptBezNum-1);

                glm::vec2 p0 = controlPoint[0];
                glm::vec2 p1 = controlPoint[1];
                glm::vec2 p2 = controlPoint[2];
                glm::vec2 p3 = controlPoint[3];


                float mt = 1.0f - t;    // (1-t)
                float mt2 = mt * mt;    // (1-t)^2
                float t2 = t * t;       // t^2

                //位置 P(t)
                glm::vec2 N = mt*mt2 * w0*p0 + 3.0f*mt2*t * w1*p1 + 3.0f*mt*t2 * w2*p2 + t*t2 * w3*p3;
                float D     = mt*mt2 * w0    + 3.0f*mt2*t * w1    + 3.0f*mt*t2 * w2    + t*t2 * w3;

                glm::vec2 P = N / D;
                bezpt[i] = P;

                //1階微分 P'(t) の計算
                glm::vec2 dN = 3.0f*mt2 * (w1*p1 - w0*p0) + 6.0f*mt*t * (w2*p2 - w1*p1) + 3.0f*t2 * (w3*p3 - w2*p2);
                float dD     = 3.0f*mt2 * (w1 - w0)       + 6.0f*mt*t * (w2 - w1)       + 3.0f*t2 * (w3 - w2);

                glm::vec2 dtp = (dN - P * dD) / D; // 1階微分（接線ベクトルの方向・速度）

                //2階微分 P''(t) の計算
                glm::vec2 ddN = 6.0f*mt * (w2*p2 - 2.0f*w1*p1 + w0*p0) + 6.0f*t * (w3*p3 - 2.0f*w2*p2 + w1*p1);
                float ddD     = 6.0f*mt * (w2 - 2.0f*w1 + w0)          + 6.0f*t * (w3 - 2.0f*w2 + w1);

                glm::vec2 ddtp = (ddN - 2.0f * dtp * dD - P * ddD) / D; // 2階微分（曲率計算などに使用する加速度）

                //法線ベクトルの計算
                glm::vec2 n = glm::normalize(glm::vec2(-(dtp.y), dtp.x));

                //外積の計算
                float crossp = (dtp.x)*(ddtp.y) - (dtp.y)*(ddtp.x);

                //絶対値の計算
                float len = glm::length(dtp);
                float normb = len * len * len; //分母の計算

                //K曲率の計算
                float K = crossp / normb;

                curvatureX.push_back(t);
                curvatureY.push_back(K);

                //Comb
                float scale = 0.05f;
                glm::vec2 comb = P - K * n * scale ; //先端の位置

                float epsilon = 1e-5f;

                //始点（ｔ＝０）の曲率円
                if(i == 0) {
                    if(std::abs(K) > epsilon) {
                        circleRadius0 = 1.0f / K;
                        circleCenter0 = P + circleRadius0 * n; // 中心 = 位置 + 半径 * 法線
                    } else {
                        circleRadius0 = 0.0f; // 曲率0（直線）の時は円を描かない
                    }
                }

                // 終点 (ｔ＝１) の曲率円
                if(i == ptBezNum - 1) {
                    if(std::abs(K) > epsilon) {
                        circleRadius1 = 1.0f / K;
                        circleCenter1 = P + circleRadius1 * n;
                    } else {
                        circleRadius1 = 0.0f;
                    }
                }

                Comb.push_back(comb);

                //Combの数
                if(i == 0){
                    Combspike.push_back(comb);
                    Combspike.push_back(P);
                }
                if(Comb.size() % 50 == 0){
                    Combspike.push_back(comb);
                    Combspike.push_back(P);
                }

            }


            //------------Dietz----------------

            glm::vec2 p0 = controlPoint[0];
            glm::vec2 p1 = controlPoint[1];
            glm::vec2 p2 = controlPoint[2];
            glm::vec2 p3 = controlPoint[3];

            float K0 = curvatureY[0];
            float K1 = curvatureY.back();


            if(K0 < K1){
                //phiの計算
                glm::vec2 v03 = p3 - p0;
                float len03 = glm::length(v03); //絶対値の計算

                glm::vec2 v01 = p1 - p0;
                float len01 = glm::length(v01);//絶対値の計算

                //0にならないように
                if (len03 > 1e-5f && len01 > 1e-5f) {
                    //cosθ を求める
                    float cosPhi1 = glm::dot(v03, v01) / (len03 * len01);

                    cosPhi1 = glm::clamp(cosPhi1, -1.0f, 1.0f);

                    phi1 = glm::degrees(std::acos(cosPhi1));

                }

                glm::vec2 v23 = p3 - p2; // P2 から P3 へのベクトル
                float len23 = glm::length(v23);


                if (len23 > 1e-5f && len03 > 1e-5f) {
                    float cosPhi2 = glm::dot(v23, v03) / (len23 * len03);
                    cosPhi2 = glm::clamp(cosPhi2, -1.0f, 1.0f);
                    phi2 = glm::degrees(std::acos(cosPhi2));

                    calculated = true;
                }

                float phi12 = phi1 + phi2;
                K0 = curvatureY[0] * len03;

                K0max = (2. * std::sin(glm::radians(phi1))) / len03;
                K1min = ((2. * (1. - std::cos(glm::radians(phi12))) - 2. * K0 * std::sin(glm::radians(phi2)))
                         / (2. * std::sin(glm::radians(phi1)) - K0))
                        / len03 ;

                if (calculated) {
                    std::cout << "phi1: " << phi1 << " | phi2: " << phi2 << std::endl;
                    std::cout << "K0max: " << glm::length(K0max )<< "| K1min: " << glm::length(K1min) << std::endl;
                }
            }
            else{
                //phiの計算
                glm::vec2 v03 = p3 - p0;
                float len03 = glm::length(v03); //絶対値の計算

                glm::vec2 v01 = p1 - p0;
                float len01 = glm::length(v01);//絶対値の計算

                //0にならないように
                if (len03 > 1e-5f && len01 > 1e-5f) {
                    //cosθ を求める
                    float cosPhi1 = glm::dot(v03, v01) / (len03 * len01);

                    cosPhi1 = glm::clamp(cosPhi1, -1.0f, 1.0f);

                    phi1 = glm::degrees(std::acos(cosPhi1));

                }

                glm::vec2 v23 = p3 - p2; // P2 から P3 へのベクトル
                float len23 = glm::length(v23);


                if (len23 > 1e-5f && len03 > 1e-5f) {
                    float cosPhi2 = glm::dot(v23, v03) / (len23 * len03);
                    cosPhi2 = glm::clamp(cosPhi2, -1.0f, 1.0f);
                    phi2 = glm::degrees(std::acos(cosPhi2));

                    calculated = true;
                }

                float phi12 = phi1 + phi2;
                K0 = curvatureY[0] * len03;

                K0max = (2. * std::sin(glm::radians(phi1))) / len03;
                K1min = ((2. * (1. - std::cos(glm::radians(phi12))) - 2. * K0 * std::sin(glm::radians(phi2)))
                         / (2. * std::sin(glm::radians(phi1)) - K0))
                        / len03 ;

                if (calculated) {
                    std::cout << "phi1: " << phi1 << " | phi2: " << phi2 << std::endl;
                    std::cout << "K0max: " << glm::length(K0max )<< "| K1min: " << glm::length(K1min) << std::endl;
                }

            }

        }
    }
};


extern CubicBezier Bezier;


