#ifndef ENCODERSCLASS_H
#define ENCODERSCLASS_H

#include <Arduino.h>

class EncodersClass {
private:
    int pinA1, pinB1, pinA2, pinB2;
    volatile long position[2] = {0, 0};
    volatile int lastState1, currentState1, lastState2, currentState2;
    static EncodersClass* instance;

public:
    EncodersClass(int a1, int b1, int a2, int b2) {
        pinA1 = a1;
        pinB1 = b1;
        pinA2 = a2;
        pinB2 = b2;
        pinMode(pinA1, INPUT);
        pinMode(pinB1, INPUT);
        pinMode(pinA2, INPUT);
        pinMode(pinB2, INPUT);
        lastState1 = digitalRead(pinA1);
        lastState2 = digitalRead(pinA2);
        instance = this;
        attachInterrupt(digitalPinToInterrupt(pinA1), isWrapper1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinA2), isWrapper2, CHANGE);
    }
    static void isWrapper1(){
      if (instance){
        instance -> update1();
      }
    }
    static void isWrapper2(){
      if (instance){
        instance -> update2();
      }
    }
    void update1() {
        currentState1 = digitalRead(pinA1);
        if (currentState1 != lastState1 && currentState1 == 1) {
            if (digitalRead(pinB1) != currentState1) {
                position[0]++;
            } else {
                position[0]--;
            }
        }
        lastState1 = currentState1;
    }
    void update2() {
        currentState2 = digitalRead(pinA2);
        if (currentState2 != lastState2 && currentState2 == 1) {
            if (digitalRead(pinB2) != currentState2) {
                position[1]++;
            } else {
                position[1]--;
            }
        }
        lastState2 = currentState2;
    }

    volatile long* getPosition() {
        return position;
    }

    void reset() {
        position[0] = 0;
        position[1] = 0;
    }
};

EncodersClass* EncodersClass::instance = nullptr;

#endif
