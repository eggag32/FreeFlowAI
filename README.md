# FreeFlow AI
This is my attempt at getting an AI to play the game [FreeFlow](https://en.wikipedia.org/wiki/Flow_Free). 

## CNN
The convolutional neural network is trained on randomly generated levels (gen.cpp). For less sophisticated generation of levels it can achieve up to 70% accuracy (when evaluated on different data generated the same way as training data). However, that accuracy drops to about 30% on more complicated levels, and its performance on the actual game levels is not great -- most likely because the levels can vary quite a lot, and actual game is rather difficult, so a more sophisticated approach is required.