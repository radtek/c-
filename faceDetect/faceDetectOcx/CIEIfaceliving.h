#ifndef CIEIFACELIVING_H
#define CIEIFACELIVING_H


// faceliving detector parameters
typedef struct{
    int type;
    int act_type;
    int batch;
    int normalize;

    int inputs;
    int outputs;

    int h, w, c;
    int out_h, out_w, out_c;

    int n;
    int ksize;
    int stride;
    int pad;

    float *scales;
    float *rolling_mean;
    float *rolling_variance;

    float *alpha;
    float *beta;

    float *col_image;
    float *output;
}params;

class _declspec(dllexport)  CIEIfaceliving
{

public:
    CIEIfaceliving();
    ~CIEIfaceliving();

    void setup_detector();
    bool load_weight(const char* path,const char* path_key);
    bool detector(float* face_img);

private:
    int face_w;
    int face_h;
    int face_c;
    int n;
	int flag;
    params* p;
};

#endif // CIEIFACELIVING_H
