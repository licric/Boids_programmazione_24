#include "Boid.hpp"
#include <cmath>
#include <stdexcept>

double Boid::distSquared(Boid const& other) const
{
  return (pos.getX() - other.pos.getX()) * (pos.getX() - other.pos.getX())
       + (pos.getY() - other.pos.getY()) * (pos.getY() - other.pos.getY());
}

bool Boid::boidCanSee(Boid const& other, double const& angleOfVision,
                      double const& radOfVision) const
{
  if (angleOfVision < 0 || radOfVision < 0) {
    throw std::runtime_error{"angle & radius of vision have to be positive"};
  }
  if (distSquared(other) < (radOfVision * radOfVision)
      && vel.angleBetween(other.pos - pos) < angleOfVision) {
    return true;
  } else {
    return false;
  }
}

// funzione per determinare se due boid sono troppo vicini
bool Boid::isTooClose(Boid const& other, double radTooClose) const
{
  if (radTooClose < 0) {
    throw std::runtime_error{"radius tooClose has to be positive"};
  }
  return distSquared(other) < radTooClose * radTooClose;
}

void Boid::limitVelMaxMin()
{
  if (vel.magnitude() > maxVel) {
    vel *= maxVel / vel.magnitude();
  } else if (vel.magnitude() < minVel) {
    vel *= minVel / vel.magnitude();
  }
}

// limitazione della velocità di sterzo
void Boid::naturalVeer(Vec2D& sumCorr)
{
  double theta = vel.angleBetween(sumCorr);
  if (theta > 4. && theta < 172.) {
    sumCorr = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(4. * M_PI / 180.) / std::abs(std::sin(theta * M_PI / 180.));
  } else if (theta >= 172. && theta <= 180.) {
    sumCorr.rotate(-15.);
    double phi = vel.angleBetween(sumCorr);
    sumCorr    = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(4. * M_PI / 180.) / std::abs(std::sin(phi * M_PI / 180.));
  }
}