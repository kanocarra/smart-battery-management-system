// Model for a cell
struct Cell {
    float voltage; //in volts
    float internal_resistance; // in milliohms
    float capacity; // in mAhr
    float state_of_charge; // in %

}

// Model for parameters of the battery pack
struct Battery {
    float time_elapsed;
    float current;
    bool is_charging;
    float state_of_charge;
}

const struct Bms_Params {
    
}

