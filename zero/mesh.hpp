#ifndef __MESH_H__
#define __MESH_H__

#include <string>
#include <queue>
#include <cstring>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vecmath.h>
// #include "vecmath/Matrix4f.h"
#include <set>
using namespace std;

#define MAX_LEN 1000
#define _pair pair<int, int>
#define t 0.1

class Mesh {
private:
 //Face
 struct Face {
    int vertex3[3]; // index of vertexs 
    int valid;

    Face(int a1, int a2, int a3){
        vertex3[0] = a1-1; // notice shift of 1
        vertex3[1] = a2-1;
        vertex3[2] = a3-1;
        valid = 1;
    }
    bool find(int x){
        for (int j : vertex3) if (x == j) return true;
        return false;
    }
 };

 //Vertex
 struct Vertex {
    Vector3f dim;
    Matrix4f Q;
    float q[16];

    vector<int> faces;
    int valid;
    int visited;

    Vertex(float a1, float a2, float a3){
        dim = Vector3f(a1, a2, a3);
        valid = 1;
        visited = 0;
    }

    Vertex(Vertex& v1, Vertex& v2){
       dim = Vector3f((v1.dim + v2.dim) * 0.5f);
       valid = 1;
    }

    void add_face(int f){
        faces.push_back(f);
    }

 };
 //Pair
 struct Pair {
     int v1, v2;
     double cost;
     Pair(int _v1, int _v2, double c){
         v1 = _v1;
         v2 = _v2;
         cost = c;
     }
     inline void sort() { if (v1 > v2) swap(v1, v2); }
     inline bool operator < (const Pair &b) const { return cost < b.cost;  }

 };

    void calculate_Q();
    void select_pairs();
    void aggregation();
    void get_p(Face& f, Vector4f& p);
    void search_vertex(int origin);
    void remove_pair_heap(int v);
    // void add_kp(const Face &face, double *kp);
    void add_kp(int f, Matrix4f& kp);
 Matrix4f _adding(Matrix4f& m1, Matrix4f& m2);

    vector<Vertex> vertexs;
    // vector< set<int> > edges;
    vector< Face > faces;
    priority_queue<Pair> heap;
    int face_cnt;

public:
    Mesh();
    ~Mesh();

    void simplify(float ratio);
    void load_from_file(string path);
    void write_into_file(string path);
};
Mesh::Mesh() {  face_cnt= 0;  return; }
Mesh::~Mesh() { }

void Mesh::load_from_file(string path){
    ifstream input(path);
    cout << path << endl;
    string buf;
    while ( std::getline(input, buf) ){
        // cout << buf << endl;

        stringstream ss(buf);
        string header;
        ss >> header;
        if (header == "v"){
            float a, b, c;
            ss >> a >> b >> c;
            vertexs.push_back( Vertex(a, b, c) );
        }
        else if (header == "f"){
            int a, b, c;
            ss >> a >> b >> c;
            faces.push_back( Face(a, b, c) );
        }
    }
    // add face for each vertex
    for (int f = 0; f < faces.size(); f++) for (int v : faces[f].vertex3) 
        vertexs[v].add_face(f);

    cout << "load file DONE" << endl;
    cout << "vertex cnt: " << vertexs.size() << endl;
    cout << "faces cnt: " << faces.size() << endl;
    cout << "================" << endl;
}

void Mesh::get_p(Face& f, Vector4f& p){
    Vector3f a =  vertexs[f.vertex3[0]].dim;
    // cout << "vect on face ,a: " << a << endl;
    Vector3f b =  vertexs[f.vertex3[1]].dim;
    // cout << "vect on face ,b: " << b << endl;
    Vector3f c =  vertexs[f.vertex3[2]].dim;
    // cout << "vect on face ,c: " << c << endl;

    Vector3f pv = Vector3f::cross(b-a, c-a).normalized();
    // cout << "pv" << pv << endl;
    // assert(pv);
    p[0] = pv[0];
    p[1] = pv[1];
    p[2] = pv[2];
    p[3] = Vector3f::dot(a, pv) * (-1.0f); // d ? TODO
}
void Mesh:: add_kp(int f, Matrix4f& kp) {
    Vector4f p;
    get_p(faces[f], p);
    //add to q
    Matrix4f m = Vector4f::outProd(p,p);
    kp = kp + m;
    //  memset(kp, 0 ,sizeof(int)*16);
    // for (int k = 0; k < 4; k++){
    //     for (int h = 0; h < 4; h++){
    //         kp[k*4 + h] += m.getRow(k)[h];
    //         // cout << kp[k*4+h] << endl;
    //     }
    // }
}

//  Matrix4f Mesh::_adding(Matrix4f& m1, Matrix4f& m2){
//     Matrix4f ans = ( 
//             0.0, 0.0, 0.0, 0.0,
//             0.0, 0.0, 0.0, 0.0,
//             0.0, 0.0, 0.0, 0.0,
//             0.0, 0.0, 0.0, 0.0
//         );
//     for (int i = 0; i < 4; i++){
//         for (int j = 0; j < 4; j++){
//             ans.s = m1[i][j] + m2[i][j];
//         }
//     }
//     return ans;
// }
void Mesh::calculate_Q() {
    cout << "calculate Q" << endl;
    for (int vi = 0; vi < vertexs.size(); vi++){
    
        for (int f: vertexs[vi].faces){
        // cout << "vQ" << v.Q.getRow(0).x() << endl;
        // cout << "m" << m.getRow(0).x() << endl;
        // do for q
        // add_Kp
            add_kp(f, vertexs[vi].Q);
        }

        // cout << "vQ" << v.Q.getRow(0).x() << endl;
    }

}

// void Mesh::search_vertex(int origin){
//     //bfs colect one
//     for (int f: vertexs[origin].faces){
//         for (int v : faces[f].vertex3) if (origin != v && vertexs[v].valid && vertexs[v].visited == 0){
//             if ((vertexs[v].dim - vertexs[origin].dim).abs() < t ) heap.push(Pair(origin, v));
//             else search_vertex(v);
//         }
//     }
// }

inline float calculate_cost(Vector4f& p, Matrix4f Q){
    // assert(0);
    float p1 = Vector4f::dot(p, Q.getCol(0));
    float p2 = Vector4f::dot(p, Q.getCol(1));
    float p3 = Vector4f::dot(p, Q.getCol(2));
    float p4 = Vector4f::dot(p, Q.getCol(3));

    Vector4f pp(p1,p2,p3,p4);
    float ans = Vector4f::dot(pp,p);
    // assert(0);
    cout << "cost" << ans << endl;
    return ans;
}

void Mesh::select_pairs(){
    cout << "select pairs" << endl;
    int vis[vertexs.size()];
    // how to loop?
    for (int i = 0; i < vertexs.size(); i++) if (vertexs[i].valid){ 
        for (int ff: vertexs[i].faces) if (faces[ff].valid){
            Face f = faces[ff];
            for (int k : f.vertex3) if (i < k) { // preventing cal twice
                int v1 = i, v2 = k;

                // using mid point 
                Vertex mid = Vertex(vertexs[i], vertexs[k]);
                Vector4f mid_p(mid.dim, 1.0f);
                // cout << "mid:" << mid.dim << endl;
                Matrix4f Q1 = vertexs[i].Q;
                Matrix4f Q2 = vertexs[k].Q;
                // cout << "Q1" << Q1.getRow(0).x() << endl;
                // assert(0);

                float cost = calculate_cost(mid_p , Q1 + Q2);
                // cout << "cost:" << cost << endl;
                // assert(cost >= 0);
                heap.push(Pair(v1, v2, cost));
            }
        }
        // what about || < thold
        // search_vertex(i);
    }
    cout << "valid pair num:" << heap.size() << endl;
}

void Mesh::remove_pair_heap(int v){

}

void Mesh::aggregation() {
    Pair pair = heap.top(); heap.pop();
    cout << "this pair cost " << pair.cost  << endl;
    int v1 = pair.v1, v2 = pair.v2;
    // revise v2
    vertexs[v2].dim = (vertexs[v1].dim + vertexs[v2].dim) / 2;
    vertexs[v1].valid = 0; // remove v1

    // let faces pointed to v2
    for (int f: vertexs[v1].faces) {
        if (faces[f].valid == 0) continue;
        if (faces[f].find(v2)) faces[f].valid = 0; face_cnt--; // for non-edge this wont happen
        for (int j = 0; j < 3; j++) {
            if ( faces[f].vertex3[j] == v1) {
                faces[f].vertex3[j] = v2;

                // v2 add v1's face
                vertexs[v2].faces.push_back(f);
            }
        }
    }

    //heap remove pair contain v1;
    remove_pair_heap(v1);
}

void Mesh::simplify(float ratio){
    cout << "simplify " << endl;
    calculate_Q();
    // for (Vertex v: vertexs){
    //     cout << v.Q.getRow(0)[0] << endl;
    // }
    select_pairs();
    face_cnt = faces.size();
    cout << "facecnt: " << face_cnt << endl;

    while (face_cnt >= faces.size() * ratio){
        // do aggregation
        // cout << "face_cnt:" << face_cnt << endl;
        aggregation();
    }
}


void Mesh::write_into_file(string path){
    cout << "write to file " << endl;
    ofstream out;
    out.open(path);
    for (Vertex v: vertexs) if (v.valid){
        out << "v " << v.dim[0] << " " << v.dim[1] << " " << v.dim[2] << endl;
    }
    for (Face f: faces) if (f.valid){
        out << "f " << f.vertex3[0] << " " << f.vertex3[1]  << " " << f.vertex3[2] << endl;
    }
    out.close();
}

#endif