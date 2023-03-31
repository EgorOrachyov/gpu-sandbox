#include "common_def.cl"

__kernel void filter1(__global const float4* input,
                      __global float4*       output,
                      int                    w,
                      int                    h) {
    uint i = get_global_id(0);
    uint j = get_global_id(1);

    if (i < h && j < w) {
        float3 weights    = (float3) (0.299f, 0.587f, 0.114f);
        float  gray       = dot(input[i * w + j].rgb, weights);
        output[i * w + j] = (float4) (gray, gray, gray, 1.0f);
    }
}

__kernel void filter2(read_only image2d_t  input,
                      write_only image2d_t output,
                      sampler_t            sampler,
                      int                  w,
                      int                  h) {
    uint i = get_global_id(0);
    uint j = get_global_id(1);

    if (i < h && j < w) {
        float3 weights = (float3) (0.299f, 0.587f, 0.114f);
        float3 color   = read_imagef(input, sampler, (int2) (j, i)).rgb;
        float  gray    = dot(color, weights);
        write_imagef(output, (int2) (j, i), (float4) (gray, gray, gray, 1.0f));
    }
}