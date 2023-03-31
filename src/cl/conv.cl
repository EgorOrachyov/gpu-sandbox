#include "common_def.cl"

#define HALF_SIZE 2

__kernel void filter(read_only image2d_t  input,
                     write_only image2d_t output,
                     sampler_t            sampler,
                     int                  w,
                     int                  h,
                     float                factor,
                     __constant float*    ker) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i < h && j < w) {
        float3 result = (float3) (0, 0, 0);

        int k = 0;

        for (int y = i - HALF_SIZE; y <= i + HALF_SIZE; y++) {
            for (int x = j - HALF_SIZE; x <= j + HALF_SIZE; x++, k++) {
                result += read_imagef(input, sampler, (int2) (x, y)).rgb * ker[k];
            }
        }

        result /= factor;
        result = clamp(result, (float3) (0, 0, 0), (float3) (1, 1, 1));
        write_imagef(output, (int2) (j, i), (float4) (result, 1.0f));
    }
}