#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct P {
    double x, y;     
    int CC;     
    double mDIST;
    P() : x(0.0), y(0.0), CC(-1), mDIST(DBL_MAX) {}
    P(double x, double y) : x(x), y(y), CC(-1), mDIST(DBL_MAX) {}    
    double ED(P p) {return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);}
};

void k_means(vector<P>* PUNTOS, int RR, int k) {
    int n = PUNTOS->size();
    vector<P> RANDOMS;
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        RANDOMS.push_back(PUNTOS->at(rand() % n));
    }
    for (int i = 0; i < RR; ++i) {        
        for (vector<P>::iterator c = begin(RANDOMS); c != end(RANDOMS); ++c) {
            int tag = c - begin(RANDOMS);
            for (vector<P>::iterator it = PUNTOS->begin();
                it != PUNTOS->end(); ++it) {
                P p = *it;
                double dist = c->ED(p);
                if (dist < p.mDIST) {
                    p.mDIST = dist;
                    p.CC = tag;
                }
                *it = p;
            }
        }        
        vector<int> nPoints;
        vector<double> sumX, sumY;
        for (int j = 0; j < k; ++j) {
            nPoints.push_back(0);
            sumX.push_back(0.0);
            sumY.push_back(0.0);
        }
        for (vector<P>::iterator it = PUNTOS->begin(); it != PUNTOS->end();++it) {
            int tag = it->CC;
            nPoints[tag] += 1;
            sumX[tag] += it->x;
            sumY[tag] += it->y;
            it->mDIST = DBL_MAX;
        }        
        for (vector<P>::iterator c = begin(RANDOMS); c != end(RANDOMS);
            ++c) {
            int tag = c - begin(RANDOMS);
            c->x = sumX[tag] / nPoints[tag];
            c->y = sumY[tag] / nPoints[tag];
        }
    }    
    ofstream myfile;
    myfile.open("output.csv");
    myfile << "X , Y, COLOR" << endl;
    for (vector<P>::iterator it = PUNTOS->begin(); it != PUNTOS->end();++it) {
        myfile << it->x << "," << it->y << "," << it->CC << endl;}
    myfile.close();
}

int main() {    
    string temp;    
    vector<P> PUNTOS;
    ifstream file("yellow_tripdata_2009-12.csv");    
    if (!file.is_open()) {cout << "error\n";}
    for (int i = 0; i < 2; i++) {getline(file, temp, '\n');}    
    for (int j = 0; j < 5000000; j++) {
        double x, y;
        string XX, YY;
        for (int k = 0; k < 5; k++) {
            getline(file, temp, ',');
        }
        getline(file, XX, ',');
        getline(file, YY, ',');
        getline(file, temp, '\n');
        x = stod(XX);
        y = stod(YY);
        PUNTOS.push_back(P(x, y));
    }   
    file.close();
    k_means(&PUNTOS, 10, 5);
    return 0;
}