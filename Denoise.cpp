//
//  Created by ksel on 2017/7/17.
//  Copyright © 2017年 ksel. All rights reserved.
//


#include "noise_reduction/speex/speex_preprocess.h"
#include <stdio.h>

#define NN 80

int denoise(const short* input, const int input_len, short* output, SpeexPreprocessState *st)
{

    short in[NN];

    int i;
    float f;
    

    i=1;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);
    i=0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
    i=44100;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
    i=0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
    f=.0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
    f=.0;
    speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);

    int count = 0;
    while (1)
    {
        if (input_len - count < NN) {
            break;
        }
        int vad;
        for (i=0; i<NN; i++){
            in[i] = input[count+i];
            //printf("Origin: %d\n", in[i]);
        }
        vad = speex_preprocess_run(st, in);
        /*fprintf (stderr, "%d\n", vad);*/
        for (i=0; i<NN; i++){
            //printf("Denoised: %d\n", in[i]);
            output[count+i] = in[i];
        }
        count = count + NN;
    }
    return 0;
}
