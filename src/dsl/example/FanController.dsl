peripheral FanController {
    int fanPin;
    SharedVarRO int currentRpm;
    SharedVar uint8_t powerLevel;

    Service void setPower(uint8_t power);
    Service void emergencyStop();
}