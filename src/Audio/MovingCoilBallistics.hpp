#pragma once

#include <Settings/SettingsWrapper.hpp>

class MovingCoilBallistics {
  public:
    MovingCoilBallistics(float springConstant, float friction, float mass,
                         float Ts = 1.0 / MAX_FPS)
        : springConstant(springConstant / mass * Ts),
          friction(friction / mass * Ts), Ts(Ts) {}

    float operator()(float input) {
        // float x_dot_new = (1 - friction / mass * Ts) * x_dot -
        //                   springConstant / mass * Ts * (x + input);
        float x_dot_new = (1 - friction) * x_dot + springConstant * (input - x);
        x = Ts * x_dot + x;
        x_dot = x_dot_new;

        DEBUGVAL(x, x_dot);
        DEBUGVAL(friction, springConstant, Ts);

        if (x >= 1) {
            x = 1;
            x_dot = x_dot > 0 ? 0 : x_dot;
        } else if (x < 0) {
            x = 0;
            x_dot = x_dot < 0 ? 0 : x_dot;
        }
        return x;
    }

    static MovingCoilBallistics officialVU() {
        return MovingCoilBallistics(0.16025, 0.0215, 0.001);
    }

    static MovingCoilBallistics responsiveVU(float Tsfactor = 2.0) {
        return MovingCoilBallistics(0.16025, 0.0215, 0.001, Tsfactor / MAX_FPS);
    }

    static MovingCoilBallistics noOvershoot(float Tsfactor = 1.0) {
        return MovingCoilBallistics(1.05, 0.1, 0.002, Tsfactor / MAX_FPS);
    }

  private:
    const float springConstant;
    const float friction;
    const float Ts;

    float x = 0;
    float x_dot = 0;
};