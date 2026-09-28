#include<algorithm>
#include<pybind11/pybind11.h>
#include<pybind11/stl.h>
#include<vector>
#include<cmath>
#include<random>
#include<iostream>

namespace py = pybind11;
using namespace std;
random_device rd;
mt19937 g_rng(rd());

class Particle {
public:
    Particle(int n_particles, double Dp, double Dt, double radius, pair<int,int> bounds)
        : n_particles(n_particles),
          radius(radius),
          Dp(Dp),
          Dt(Dt),
          positions(n_particles, vector<double>(2)),
          bounds(bounds),
          active_particles(n_particles, true),
          velocities(n_particles, vector<double>(2)),
          bam(0)
         {
             uniform_real_distribution<double> udb(-4.0,4.0);
             for(int i = 0; i < n_particles; i++) {
                 positions[i][0] = udb(g_rng);
                 positions[i][1] = udb(g_rng);
                 velocities[i][0] = udb(g_rng);
                 velocities[i][1] = udb(g_rng);
             }
         }
    void step() {
        double sigma = sqrt(2*Dp*Dt);
        normal_distribution<double> db(0.0, sigma);
        for(int i = 0; i < n_particles; i++){
            if(active_particles[i]) {
                double brx = db(g_rng);
                double bry = db(g_rng);
                positions[i][0] = positions[i][0] + velocities[i][0]*Dt + brx;
                positions[i][1] = positions[i][1] + velocities[i][1]*Dt + bry;
                if(positions[i][0] < bounds.first || positions[i][0] > bounds.second || positions[i][1] < bounds.first || positions[i][1] > bounds.second) {
                    active_particles[i] = false;
                }
            }
        }
    }
    void check_collisions() {
        for(int i = 0; i < n_particles; i++) {
            for(int j = i+1; j < n_particles; j++) {
                if(!active_particles[i] || !active_particles[j]) {
                    continue;
                }
                double dx = positions[i][0] - positions[j][0];
                double dy = positions[i][1] - positions[j][1];
                double dist = sqrt(dx*dx + dy*dy);
                if(dist < 2*radius) {
                    swap(velocities[i], velocities[j]);
                    bam++;
                }
            }
        }
    }

    void simulate(int n_steps) {
        for(int i = 0; i < n_steps; i++) {
            if(i%1000 == 0) {
                cout<<i<<endl;
            }
            step();
            check_collisions();
        }
        auto cnt = count(active_particles.begin(),active_particles.end(),true);
        cout<<"Active particles: "<<cnt<<endl;
        cout<<"Absorbed at boundary: "<<(n_particles - cnt)<<endl;
        cout<<"bam: "<<bam<<endl;
    }

    vector<vector<double>> get_positions() {
        return positions;
    }
private:
    int n_particles;
    double Dp, Dt, radius;
    vector<vector<double>> positions;
    pair<int,int> bounds;
    vector<bool> active_particles;
    vector<vector<double>> velocities;
    int bam;
};

PYBIND11_MODULE(particle,m) {

    py::class_<Particle>(m,"Particle").def(py::init<int, double, double, double, pair<int,int>>())
        .def("step", &Particle::step)
        .def("simulate", &Particle::simulate)
        .def("get_positions", &Particle::get_positions);
}
