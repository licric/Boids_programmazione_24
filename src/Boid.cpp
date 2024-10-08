#include "../headers/Boid.hpp"
#include <stdexcept>

double Boid::distSquared(Boid const& other) const
{
  return (pos.getX() - other.pos.getX()) * (pos.getX() - other.pos.getX())
       + (pos.getY() - other.pos.getY()) * (pos.getY() - other.pos.getY());
}

bool Boid::boidCanSee(Boid const& other, double angleOfVision,
                      double radOfVision) const
{
  if (distSquared(other) < (radOfVision * radOfVision)
      && vel.angleBetween(other.pos - pos) < angleOfVision) {
    return true;
  } else {
    return false;
  }
}

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

// limitation of steer velocity
void Boid::naturalVeer(Vec2D& sumCorr)
{
  double theta = vel.angleBetween(sumCorr);
  if (theta > 3. && theta < 175.) {
    sumCorr = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(3. * M_PI / 180.) / std::abs(std::sin(theta * M_PI / 180.));
  } else if (theta >= 175. && theta <= 180.) {
    sumCorr.rotate(-10.);
    double phi = vel.angleBetween(sumCorr);
    sumCorr    = sumCorr * (vel.magnitude() / sumCorr.magnitude()) *=
        std::sin(3. * M_PI / 180.) / std::abs(std::sin(phi * M_PI / 180.));
  }
}