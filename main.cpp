

#include "noise_reduction/speex/speex_preprocess.h"
#include "Denoise.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>


#define FRAME_SIZE 22050

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    
    const char * wavReadPath = "./2.pcm";

    std::fstream fin;
    fin.open(wavReadPath);
    if(!fin.is_open()) {
        printf("can't open in file\n");
    }

    
    FILE* fpcm = fopen("./out.pcm", "wb");

    int count = 0;
    
    short nNum = 0;
    short tmp = 0;
    
    short in[FRAME_SIZE] = {0};
    short out[FRAME_SIZE]={0};
    
    SpeexPreprocessState *st;
    st = speex_preprocess_state_init(80, 44100);

    while(1){

        fin.read((char*)&nNum, sizeof(short));
        
        in[count] = nNum;
        
        if ((count+1) % FRAME_SIZE == 0) {
            denoise(in, FRAME_SIZE, out, st);
            for (int ii = 0; ii < FRAME_SIZE; ii++) {
                tmp = out[ii];
                fwrite(&tmp, 2, 1, fpcm);
            }
            count = 0;
            printf("once more\n");
        }

        if(fin.eof()){
            denoise(in, count, out, st);
            for (int ii = 0; ii < count; ii++) {
                tmp = out[ii];
                fwrite(&tmp, 2, 1, fpcm);
            }
            count = 0;
            break;
        }
        count++;
    }
    
    fin.close();
    fclose(fpcm);
    speex_preprocess_state_destroy(st);
    printf("Done\n");
    return 0;
}
