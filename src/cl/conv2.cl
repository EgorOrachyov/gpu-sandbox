#include "common_def.cl"

#define HALF_SIZE 2

__kernel void filter(read_only image2d_t  input,
                     write_only image2d_t output,
                     sampler_t            sampler,
                     int                  w,
                     int                  h,
                     float                factor,
                     __constant float*    ker1,
                     __constant float*    ker2) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i < h && j < w) {
        float3 result1 = (float3) (0, 0, 0);
        float3 result2 = (float3) (0, 0, 0);

        int k = 0;

        for (int y = i - HALF_SIZE; y <= i + HALF_SIZE; y++) {
            for (int x = j - HALF_SIZE; x <= j + HALF_SIZE; x++, k++) {
                float3 color = read_imagef(input, sampler, (int2) (x, y)).rgb;
                result1 += color * ker1[k];
                result2 += color * ker2[k];
            }
        }

        result1 /= factor;
        result2 /= factor;

        float3 result = sqrt(result1 * result1 + result2 * result2);
        result        = clamp(result, (float3) (0, 0, 0), (float3) (1, 1, 1));

        write_imagef(output, (int2) (j, i), (float4) (result, 1.0f));
    }
}