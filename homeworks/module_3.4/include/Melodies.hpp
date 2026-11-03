#pragma once
#include "Pitches.hpp"

namespace Melodies {

    //Mario
    inline const Note mario[] = {
        {NOTE_E5, 2}, {NOTE_E5, 2}, {NOTE_PAUSE, 2}, {NOTE_E5, 2}, 
        {NOTE_PAUSE, 2}, {NOTE_C5, 2}, {NOTE_E5, 4}, 
        {NOTE_G5, 4}, {NOTE_PAUSE, 4}, {NOTE_G4, 4}, {NOTE_PAUSE, 4},

        {NOTE_C5, 4}, {NOTE_PAUSE, 2}, {NOTE_G4, 2}, {NOTE_PAUSE, 4}, {NOTE_E4, 4}, 
        {NOTE_PAUSE, 2}, {NOTE_A4, 2}, {NOTE_PAUSE, 2}, {NOTE_B4, 2}, {NOTE_PAUSE, 2}, 
        {NOTE_AS4, 2}, {NOTE_A4, 4},

        {NOTE_G4, 2}, {NOTE_E5, 2}, {NOTE_G5, 2}, {NOTE_A5, 4}, {NOTE_F5, 2}, {NOTE_G5, 2}, 
        {NOTE_PAUSE, 2}, {NOTE_E5, 2}, {NOTE_PAUSE, 2}, {NOTE_C5, 2}, {NOTE_D5, 2}, {NOTE_B4, 4},

        {NOTE_C5, 4}, {NOTE_PAUSE, 2}, {NOTE_G4, 2}, {NOTE_PAUSE, 4}, {NOTE_E4, 4}, 
        {NOTE_PAUSE, 2}, {NOTE_A4, 2}, {NOTE_PAUSE, 2}, {NOTE_B4, 2}, {NOTE_PAUSE, 2}, 
        {NOTE_AS4, 2}, {NOTE_A4, 4},

        {NOTE_G4, 2}, {NOTE_E5, 2}, {NOTE_G5, 2}, {NOTE_A5, 4}, {NOTE_F5, 2}, {NOTE_G5, 2}, 
        {NOTE_PAUSE, 2}, {NOTE_E5, 2}, {NOTE_PAUSE, 2}, {NOTE_C5, 2}, {NOTE_D5, 2}, {NOTE_B4, 4},
        
        {NOTE_PAUSE, 4}, {NOTE_G5, 2}, {NOTE_FS5, 2}, {NOTE_F5, 2}, {NOTE_DS5, 4}, {NOTE_E5, 2},
        {NOTE_PAUSE, 2}, {NOTE_G4, 2}, {NOTE_A4, 2}, {NOTE_C5, 2}, {NOTE_PAUSE, 2}, {NOTE_A4, 2}, {NOTE_C5, 2}, {NOTE_D5, 2},

        {NOTE_PAUSE, 4}, {NOTE_G5, 2}, {NOTE_FS5, 2}, {NOTE_F5, 2}, {NOTE_DS5, 4}, {NOTE_E5, 2},
        {NOTE_PAUSE, 2}, {NOTE_C6, 2}, {NOTE_PAUSE, 2}, {NOTE_C6, 2}, {NOTE_C6, 4},

        {0, 0}
    };

    //Win Sound
    inline const Note win[] = {
        {NOTE_C5, 2}, {NOTE_E5, 2}, {NOTE_G5, 2}, {NOTE_C6, 4},
        {0, 0}
    };

    // Error / Alarm
    inline const Note error[] = {
        {NOTE_A4, 3}, {NOTE_PAUSE, 1}, {NOTE_A4, 3}, {NOTE_PAUSE, 1},
        {NOTE_DS4, 6},
        {0, 0}
    };

        //Sigma Face Theme
    inline const Note sigma[] = {
        {NOTE_E5, 4}, {NOTE_G5, 4}, {NOTE_A5, 4}, {NOTE_B5, 8},
        {NOTE_PAUSE, 2},
        {NOTE_B5, 2}, {NOTE_A5, 2}, {NOTE_B5, 2}, {NOTE_A5, 2}, {NOTE_G5, 8},
        {NOTE_PAUSE, 4},
        {NOTE_D5, 4}, {NOTE_E5, 4}, {NOTE_G5, 4}, {NOTE_A5, 8},
        {NOTE_PAUSE, 2},
        {NOTE_A5, 2}, {NOTE_G5, 2}, {NOTE_A5, 2}, {NOTE_G5, 2}, {NOTE_E5, 10},
        {NOTE_PAUSE, 12},
        {0, 0}
    };

    // GIGACHAD THEME (Can You Feel My Heart)
    inline const Note gigachad[] = {
        
        {NOTE_B4, 3}, {NOTE_PAUSE, 1}, {NOTE_B4, 2}, {NOTE_B4, 2}, {NOTE_D5, 4},
        {NOTE_A4, 3}, {NOTE_PAUSE, 1}, {NOTE_A4, 2}, {NOTE_A4, 2}, {NOTE_FS4, 4},
        {NOTE_G4, 3}, {NOTE_PAUSE, 1}, {NOTE_G4, 2}, {NOTE_G4, 2}, {NOTE_B4, 4},
        {NOTE_FS4, 6}, {NOTE_D4, 6},
        
        
        {NOTE_B4, 3}, {NOTE_PAUSE, 1}, {NOTE_B4, 2}, {NOTE_B4, 2}, {NOTE_D5, 4},
        {NOTE_A4, 3}, {NOTE_PAUSE, 1}, {NOTE_A4, 2}, {NOTE_A4, 2}, {NOTE_FS5, 4},
        {NOTE_G4, 3}, {NOTE_PAUSE, 1}, {NOTE_G4, 2}, {NOTE_G4, 2}, {NOTE_B4, 4},
        {NOTE_FS4, 12},
        {NOTE_PAUSE, 10},
        {0, 0}
    };

    // CHIPI CHIPI CHAPA CHAPA
    inline const Note chipi[] = {
        {NOTE_E5, 2}, {NOTE_E5, 2}, {NOTE_E5, 2}, {NOTE_E5, 2}, 
        {NOTE_D5, 2}, {NOTE_D5, 2}, {NOTE_D5, 2}, {NOTE_D5, 2}, 
        {NOTE_C5, 2}, {NOTE_C5, 2}, {NOTE_C5, 2}, {NOTE_C5, 2},
        {NOTE_B4, 2}, {NOTE_B4, 2}, {NOTE_E5, 4},
        
        {NOTE_A5, 2}, {NOTE_A5, 2}, {NOTE_A5, 2}, {NOTE_A5, 2},
        {NOTE_G5, 2}, {NOTE_G5, 2}, {NOTE_G5, 2}, {NOTE_G5, 2},
        {NOTE_F5, 4}, {NOTE_E5, 4}, {NOTE_D5, 4}, {NOTE_C5, 4},
        {NOTE_B4, 4}, {NOTE_E5, 4}, {NOTE_A4, 8},
        {NOTE_PAUSE, 16},
        {0, 0}
    };
}
